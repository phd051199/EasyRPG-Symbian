#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <string>

#include "image_bmp.h"
#include "image_png.h"
#include "image_xyz.h"

extern "C" int uncompress(unsigned char* dest, unsigned long* dest_len, const unsigned char* source, unsigned long source_len);

namespace {
unsigned short Read16(const unsigned char* p) {
    return static_cast<unsigned short>(p[0] | (static_cast<unsigned short>(p[1]) << 8));
}

unsigned int Read32(const unsigned char* p) {
    return static_cast<unsigned int>(p[0]) |
           (static_cast<unsigned int>(p[1]) << 8) |
           (static_cast<unsigned int>(p[2]) << 16) |
           (static_cast<unsigned int>(p[3]) << 24);
}

bool ReadWholeFile(FILE* stream, unsigned char*& data, unsigned& length) {
    data = 0;
    length = 0;
    if (!stream || fseek(stream, 0, SEEK_END) != 0)
        return false;
    long size = ftell(stream);
    if (size <= 0 || fseek(stream, 0, SEEK_SET) != 0)
        return false;
    data = static_cast<unsigned char*>(malloc(static_cast<unsigned>(size)));
    if (!data)
        return false;
    if (fread(data, 1, static_cast<unsigned>(size), stream) != static_cast<unsigned>(size)) {
        free(data);
        data = 0;
        return false;
    }
    length = static_cast<unsigned>(size);
    return true;
}
}

namespace FileFinder {
FILE* fopenUTF8(const std::string& name_utf8, char const* mode) {
    return fopen(name_utf8.c_str(), mode);
}
}

namespace ImageXYZ {
bool ReadXYZ(const uint8_t* data, unsigned len, bool transparent, int& width, int& height, void*& pixels) {
    pixels = 0;
    width = 0;
    height = 0;
    if (!data || len < 9 || memcmp(data, "XYZ1", 4) != 0)
        return false;

    const unsigned w = Read16(data + 4);
    const unsigned h = Read16(data + 6);
    if (!w || !h || w > 4096 || h > 4096)
        return false;
    if (h > 0xffffffffu / w)
        return false;

    const unsigned pixel_count = w * h;
    const unsigned raw_size = 768u + pixel_count;
    unsigned char* raw = static_cast<unsigned char*>(malloc(raw_size));
    if (!raw)
        return false;

    unsigned long decoded = raw_size;
    if (uncompress(raw, &decoded, data + 8, len - 8) != 0 || decoded != raw_size) {
        free(raw);
        return false;
    }

    if (pixel_count > 0xffffffffu / 4u) {
        free(raw);
        return false;
    }
    unsigned char* out = static_cast<unsigned char*>(malloc(pixel_count * 4u));
    if (!out) {
        free(raw);
        return false;
    }

    const unsigned char* palette = raw;
    const unsigned char* indices = raw + 768;
    for (unsigned i = 0; i < pixel_count; ++i) {
        const unsigned index = indices[i];
        out[i * 4 + 0] = palette[index * 3 + 0];
        out[i * 4 + 1] = palette[index * 3 + 1];
        out[i * 4 + 2] = palette[index * 3 + 2];
        out[i * 4 + 3] = transparent && index == 0 ? 0 : 255;
    }

    free(raw);
    width = static_cast<int>(w);
    height = static_cast<int>(h);
    pixels = out;
    return true;
}

bool ReadXYZ(FILE* stream, bool transparent, int& width, int& height, void*& pixels) {
    unsigned char* data = 0;
    unsigned length = 0;
    if (!ReadWholeFile(stream, data, length))
        return false;
    const bool result = ReadXYZ(data, length, transparent, width, height, pixels);
    free(data);
    return result;
}
}

namespace ImageBMP {
bool ReadBMP(const uint8_t* data, unsigned len, bool transparent, int& width, int& height, void*& pixels) {
    pixels = 0;
    width = 0;
    height = 0;
    if (!data || len < 54 || data[0] != 'B' || data[1] != 'M')
        return false;

    const unsigned pixel_offset = Read32(data + 10);
    const unsigned dib_size = Read32(data + 14);
    if (dib_size < 40 || 14u + dib_size > len)
        return false;

    const int w = static_cast<int>(Read32(data + 18));
    const int signed_h = static_cast<int>(Read32(data + 22));
    const unsigned bpp = Read16(data + 28);
    const unsigned compression = Read32(data + 30);
    if (w <= 0 || signed_h == 0 || compression != 0 || (bpp != 8 && bpp != 24 && bpp != 32))
        return false;

    const bool bottom_up = signed_h > 0;
    const unsigned h = static_cast<unsigned>(bottom_up ? signed_h : -signed_h);
    if (h > 0xffffffffu / static_cast<unsigned>(w))
        return false;
    const unsigned pixel_count = static_cast<unsigned>(w) * h;
    if (pixel_count > 0xffffffffu / 4u)
        return false;

    const unsigned row_bytes = ((static_cast<unsigned>(w) * bpp + 31u) / 32u) * 4u;
    if (pixel_offset > len || h > (len - pixel_offset) / row_bytes)
        return false;

    unsigned char* out = static_cast<unsigned char*>(malloc(pixel_count * 4u));
    if (!out)
        return false;

    const unsigned palette_offset = 14u + dib_size;
    unsigned palette_count = 0;
    if (bpp == 8) {
        palette_count = Read32(data + 46);
        if (!palette_count)
            palette_count = 256;
        if (palette_count > 256 || palette_offset > len || palette_count > (len - palette_offset) / 4u) {
            free(out);
            return false;
        }
    }

    for (unsigned y = 0; y < h; ++y) {
        const unsigned sy = bottom_up ? h - 1u - y : y;
        const unsigned char* row = data + pixel_offset + sy * row_bytes;
        for (int x = 0; x < w; ++x) {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a = 255;
            if (bpp == 8) {
                const unsigned index = row[x];
                if (index >= palette_count) {
                    free(out);
                    return false;
                }
                const unsigned char* entry = data + palette_offset + index * 4u;
                b = entry[0];
                g = entry[1];
                r = entry[2];
                if (transparent && index == 0)
                    a = 0;
            } else if (bpp == 24) {
                const unsigned char* entry = row + x * 3;
                b = entry[0];
                g = entry[1];
                r = entry[2];
            } else {
                const unsigned char* entry = row + x * 4;
                b = entry[0];
                g = entry[1];
                r = entry[2];
                a = transparent ? entry[3] : 255;
            }
            unsigned char* dst = out + (y * static_cast<unsigned>(w) + static_cast<unsigned>(x)) * 4u;
            dst[0] = r;
            dst[1] = g;
            dst[2] = b;
            dst[3] = a;
        }
    }

    width = w;
    height = static_cast<int>(h);
    pixels = out;
    return true;
}

bool ReadBMP(FILE* stream, bool transparent, int& width, int& height, void*& pixels) {
    unsigned char* data = 0;
    unsigned length = 0;
    if (!ReadWholeFile(stream, data, length))
        return false;
    const bool result = ReadBMP(data, length, transparent, width, height, pixels);
    free(data);
    return result;
}
}

namespace ImagePNG {
unsigned int ReadBig32(const unsigned char* p) {
    return (static_cast<unsigned int>(p[0]) << 24) |
           (static_cast<unsigned int>(p[1]) << 16) |
           (static_cast<unsigned int>(p[2]) << 8) |
           static_cast<unsigned int>(p[3]);
}

unsigned char Paeth(unsigned char a, unsigned char b, unsigned char c) {
    const int p = static_cast<int>(a) + static_cast<int>(b) - static_cast<int>(c);
    const int pa = p > static_cast<int>(a) ? p - static_cast<int>(a) : static_cast<int>(a) - p;
    const int pb = p > static_cast<int>(b) ? p - static_cast<int>(b) : static_cast<int>(b) - p;
    const int pc = p > static_cast<int>(c) ? p - static_cast<int>(c) : static_cast<int>(c) - p;
    return pa <= pb && pa <= pc ? a : (pb <= pc ? b : c);
}

bool ReadPNGBytes(const uint8_t* data,
                  unsigned len,
                  bool transparent,
                  int& width,
                  int& height,
                  void*& pixels) {
    width = 0;
    height = 0;
    pixels = 0;
    static const unsigned char signature[8] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    if (!data || len < 33 || memcmp(data, signature, 8) != 0)
        return false;

    unsigned w = 0;
    unsigned h = 0;
    unsigned bit_depth = 0;
    unsigned color_type = 0;
    unsigned interlace = 0;
    const unsigned char* palette = 0;
    unsigned palette_bytes = 0;
    const unsigned char* alpha_table = 0;
    unsigned alpha_bytes = 0;
    unsigned compressed_size = 0;
    bool saw_ihdr = false;
    bool saw_iend = false;

    unsigned position = 8;
    while (position <= len - 12) {
        const unsigned chunk_size = ReadBig32(data + position);
        if (chunk_size > len - position - 12)
            return false;
        const unsigned char* type = data + position + 4;
        const unsigned char* chunk = data + position + 8;
        if (memcmp(type, "IHDR", 4) == 0) {
            if (chunk_size != 13 || saw_ihdr)
                return false;
            w = ReadBig32(chunk);
            h = ReadBig32(chunk + 4);
            bit_depth = chunk[8];
            color_type = chunk[9];
            if (chunk[10] != 0 || chunk[11] != 0)
                return false;
            interlace = chunk[12];
            saw_ihdr = true;
        } else if (memcmp(type, "PLTE", 4) == 0) {
            palette = chunk;
            palette_bytes = chunk_size;
        } else if (memcmp(type, "tRNS", 4) == 0) {
            alpha_table = chunk;
            alpha_bytes = chunk_size;
        } else if (memcmp(type, "IDAT", 4) == 0) {
            if (chunk_size > 0xffffffffu - compressed_size)
                return false;
            compressed_size += chunk_size;
        } else if (memcmp(type, "IEND", 4) == 0) {
            saw_iend = true;
            break;
        }
        position += chunk_size + 12;
    }

    if (!saw_ihdr || !saw_iend || !w || !h || w > 4096 || h > 4096 || bit_depth != 8 || interlace != 0)
        return false;
    if (color_type != 0 && color_type != 2 && color_type != 3 && color_type != 6)
        return false;
    if (color_type == 3 && (!palette || palette_bytes == 0 || palette_bytes % 3 != 0 || palette_bytes > 768))
        return false;
    if (h > 0xffffffffu / w)
        return false;

    unsigned channels = color_type == 0 || color_type == 3 ? 1u : (color_type == 2 ? 3u : 4u);
    if (w > 0xffffffffu / channels)
        return false;
    const unsigned row_bytes = w * channels;
    if (row_bytes == 0xffffffffu || h > 0xffffffffu / (row_bytes + 1u))
        return false;
    const unsigned decoded_size = h * (row_bytes + 1u);

    unsigned char* compressed = static_cast<unsigned char*>(malloc(compressed_size));
    unsigned char* decoded = static_cast<unsigned char*>(malloc(decoded_size));
    if (!compressed || !decoded) {
        free(compressed);
        free(decoded);
        return false;
    }

    unsigned write_position = 0;
    position = 8;
    while (position <= len - 12) {
        const unsigned chunk_size = ReadBig32(data + position);
        const unsigned char* type = data + position + 4;
        if (memcmp(type, "IDAT", 4) == 0) {
            memcpy(compressed + write_position, data + position + 8, chunk_size);
            write_position += chunk_size;
        }
        if (memcmp(type, "IEND", 4) == 0)
            break;
        position += chunk_size + 12;
    }

    unsigned long actual_size = decoded_size;
    if (uncompress(decoded, &actual_size, compressed, compressed_size) != 0 || actual_size != decoded_size) {
        free(compressed);
        free(decoded);
        return false;
    }
    free(compressed);

    unsigned char* scanlines = static_cast<unsigned char*>(malloc(row_bytes * h));
    if (!scanlines) {
        free(decoded);
        return false;
    }

    for (unsigned y = 0; y < h; ++y) {
        const unsigned char* src = decoded + y * (row_bytes + 1u) + 1u;
        unsigned char* dst = scanlines + y * row_bytes;
        const unsigned char* previous = y == 0 ? 0 : scanlines + (y - 1u) * row_bytes;
        const unsigned filter = decoded[y * (row_bytes + 1u)];
        if (filter > 4) {
            free(decoded);
            free(scanlines);
            return false;
        }
        for (unsigned x = 0; x < row_bytes; ++x) {
            const unsigned char left = x >= channels ? dst[x - channels] : 0;
            const unsigned char up = previous ? previous[x] : 0;
            const unsigned char upper_left = previous && x >= channels ? previous[x - channels] : 0;
            unsigned char predictor = 0;
            if (filter == 1)
                predictor = left;
            else if (filter == 2)
                predictor = up;
            else if (filter == 3)
                predictor = static_cast<unsigned char>((static_cast<unsigned>(left) + static_cast<unsigned>(up)) >> 1);
            else if (filter == 4)
                predictor = Paeth(left, up, upper_left);
            dst[x] = static_cast<unsigned char>(src[x] + predictor);
        }
    }
    free(decoded);

    const unsigned pixel_count = w * h;
    if (pixel_count > 0xffffffffu / 4u) {
        free(scanlines);
        return false;
    }
    unsigned char* out = static_cast<unsigned char*>(malloc(pixel_count * 4u));
    if (!out) {
        free(scanlines);
        return false;
    }

    for (unsigned i = 0; i < pixel_count; ++i) {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        unsigned char a = 255;
        if (color_type == 3) {
            const unsigned index = scanlines[i];
            const unsigned palette_count = palette_bytes / 3u;
            if (index >= palette_count) {
                free(scanlines);
                free(out);
                return false;
            }
            r = palette[index * 3u + 0u];
            g = palette[index * 3u + 1u];
            b = palette[index * 3u + 2u];
            if (transparent)
                a = index < alpha_bytes ? alpha_table[index] : (index == 0 ? 0 : 255);
        } else if (color_type == 0) {
            r = g = b = scanlines[i];
        } else if (color_type == 2) {
            r = scanlines[i * 3u + 0u];
            g = scanlines[i * 3u + 1u];
            b = scanlines[i * 3u + 2u];
        } else {
            r = scanlines[i * 4u + 0u];
            g = scanlines[i * 4u + 1u];
            b = scanlines[i * 4u + 2u];
            if (transparent)
                a = scanlines[i * 4u + 3u];
        }
        if (a == 0) {
            r = 0;
            g = 0;
            b = 0;
        } else if (a != 255) {
            r = static_cast<unsigned char>((static_cast<unsigned>(r) * a + 127u) / 255u);
            g = static_cast<unsigned char>((static_cast<unsigned>(g) * a + 127u) / 255u);
            b = static_cast<unsigned char>((static_cast<unsigned>(b) * a + 127u) / 255u);
        }
        out[i * 4u + 0u] = r;
        out[i * 4u + 1u] = g;
        out[i * 4u + 2u] = b;
        out[i * 4u + 3u] = a;
    }

    free(scanlines);
    width = static_cast<int>(w);
    height = static_cast<int>(h);
    pixels = out;
    return true;
}

bool ReadPNG(FILE* stream, const void* buffer, bool transparent, int& width, int& height, void*& pixels) {
    if (stream) {
        unsigned char* data = 0;
        unsigned length = 0;
        if (!ReadWholeFile(stream, data, length))
            return false;
        const bool result = ReadPNGBytes(data, length, transparent, width, height, pixels);
        free(data);
        return result;
    }
    width = 0;
    height = 0;
    pixels = 0;
    return buffer ? false : false;
}

bool WritePNG(std::ostream&, uint32_t, uint32_t, uint32_t*) {
    return false;
}
}
