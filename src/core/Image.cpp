#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <string>

#include "image_bmp.h"
#include "image_png.h"
#include "image_xyz.h"

extern "C" int uncompress(unsigned char* dest, unsigned long* dest_len, const unsigned char* source, unsigned long source_len);
extern "C" int compress2(unsigned char* dest, unsigned long* dest_len, const unsigned char* source, unsigned long source_len, int level);
extern "C" unsigned long compressBound(unsigned long source_len);

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
    if (w <= 0 || signed_h == 0 || compression != 0 ||
        (bpp != 1 && bpp != 4 && bpp != 8 && bpp != 24 && bpp != 32))
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
    if (bpp <= 8) {
        const unsigned max_palette = 1u << bpp;
        palette_count = Read32(data + 46);
        if (!palette_count)
            palette_count = max_palette;
        if (palette_count > max_palette || palette_offset > len || palette_count > (len - palette_offset) / 4u) {
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
            if (bpp <= 8) {
                unsigned index = 0;
                if (bpp == 8)
                    index = row[x];
                else if (bpp == 4)
                    index = (row[x >> 1] >> ((x & 1) ? 0 : 4)) & 0x0fu;
                else
                    index = (row[x >> 3] >> (7 - (x & 7))) & 1u;
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

bool ValidBitDepth(unsigned color_type, unsigned bit_depth) {
    if (color_type == 0)
        return bit_depth == 1 || bit_depth == 2 || bit_depth == 4 || bit_depth == 8 || bit_depth == 16;
    if (color_type == 2 || color_type == 4 || color_type == 6)
        return bit_depth == 8 || bit_depth == 16;
    if (color_type == 3)
        return bit_depth == 1 || bit_depth == 2 || bit_depth == 4 || bit_depth == 8;
    return false;
}

unsigned ChannelCount(unsigned color_type) {
    if (color_type == 0 || color_type == 3)
        return 1;
    if (color_type == 2)
        return 3;
    if (color_type == 4)
        return 2;
    if (color_type == 6)
        return 4;
    return 0;
}

unsigned PassExtent(unsigned size, unsigned start, unsigned step) {
    return size <= start ? 0 : 1u + (size - 1u - start) / step;
}

unsigned ReadSample(const unsigned char* row, unsigned sample, unsigned bit_depth) {
    if (bit_depth == 8)
        return row[sample];
    if (bit_depth == 16) {
        const unsigned offset = sample * 2u;
        return (static_cast<unsigned>(row[offset]) << 8) | row[offset + 1u];
    }
    const unsigned bit_offset = sample * bit_depth;
    const unsigned shift = 8u - bit_depth - (bit_offset & 7u);
    return (row[bit_offset >> 3] >> shift) & ((1u << bit_depth) - 1u);
}

unsigned char ScaleSample(unsigned value, unsigned bit_depth) {
    if (bit_depth == 16)
        return static_cast<unsigned char>(value >> 8);
    if (bit_depth == 8)
        return static_cast<unsigned char>(value);
    const unsigned maximum = (1u << bit_depth) - 1u;
    return static_cast<unsigned char>((value * 255u + maximum / 2u) / maximum);
}

bool UnfilterRow(unsigned char* destination,
                 const unsigned char* source,
                 const unsigned char* previous,
                 unsigned row_bytes,
                 unsigned bytes_per_pixel,
                 unsigned filter) {
    if (filter > 4)
        return false;
    for (unsigned x = 0; x < row_bytes; ++x) {
        const unsigned char left = x >= bytes_per_pixel ? destination[x - bytes_per_pixel] : 0;
        const unsigned char up = previous ? previous[x] : 0;
        const unsigned char upper_left = previous && x >= bytes_per_pixel ? previous[x - bytes_per_pixel] : 0;
        unsigned char predictor = 0;
        if (filter == 1)
            predictor = left;
        else if (filter == 2)
            predictor = up;
        else if (filter == 3)
            predictor = static_cast<unsigned char>((static_cast<unsigned>(left) + static_cast<unsigned>(up)) >> 1);
        else if (filter == 4)
            predictor = Paeth(left, up, upper_left);
        destination[x] = static_cast<unsigned char>(source[x] + predictor);
    }
    return true;
}

bool DecodePixel(const unsigned char* row,
                 unsigned x,
                 unsigned color_type,
                 unsigned bit_depth,
                 const unsigned char* palette,
                 unsigned palette_bytes,
                 bool transparent,
                 unsigned char* destination) {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;
    const unsigned channels = ChannelCount(color_type);
    const unsigned sample = x * channels;

    if (color_type == 0) {
        const unsigned char gray = ScaleSample(ReadSample(row, sample, bit_depth), bit_depth);
        r = gray;
        g = gray;
        b = gray;
        if (transparent && gray == 0)
            a = 0;
    } else if (color_type == 2) {
        r = ScaleSample(ReadSample(row, sample, bit_depth), bit_depth);
        g = ScaleSample(ReadSample(row, sample + 1u, bit_depth), bit_depth);
        b = ScaleSample(ReadSample(row, sample + 2u, bit_depth), bit_depth);
    } else if (color_type == 3) {
        const unsigned index = ReadSample(row, x, bit_depth);
        const unsigned palette_count = palette_bytes / 3u;
        if (!palette || index >= palette_count)
            return false;
        r = palette[index * 3u];
        g = palette[index * 3u + 1u];
        b = palette[index * 3u + 2u];
        if (transparent && index == 0)
            a = 0;
    } else if (color_type == 4) {
        const unsigned char gray = ScaleSample(ReadSample(row, sample, bit_depth), bit_depth);
        r = gray;
        g = gray;
        b = gray;
        a = ScaleSample(ReadSample(row, sample + 1u, bit_depth), bit_depth);
    } else if (color_type == 6) {
        r = ScaleSample(ReadSample(row, sample, bit_depth), bit_depth);
        g = ScaleSample(ReadSample(row, sample + 1u, bit_depth), bit_depth);
        b = ScaleSample(ReadSample(row, sample + 2u, bit_depth), bit_depth);
        a = ScaleSample(ReadSample(row, sample + 3u, bit_depth), bit_depth);
    } else {
        return false;
    }

    destination[0] = r;
    destination[1] = g;
    destination[2] = b;
    destination[3] = a;
    return true;
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

    if (!saw_ihdr || !saw_iend || !w || !h || w > 4096 || h > 4096 || interlace > 1 ||
        !ValidBitDepth(color_type, bit_depth) || compressed_size == 0)
        return false;
    if (color_type == 3 && (!palette || palette_bytes == 0 || palette_bytes % 3 != 0 || palette_bytes > 768))
        return false;
    if (h > 0xffffffffu / w)
        return false;
    const unsigned channels = ChannelCount(color_type);
    const unsigned bits_per_pixel = channels * bit_depth;
    if (!channels || !bits_per_pixel)
        return false;

    static const unsigned pass_x[7] = {0, 4, 0, 2, 0, 1, 0};
    static const unsigned pass_y[7] = {0, 0, 4, 0, 2, 0, 1};
    static const unsigned pass_dx[7] = {8, 8, 4, 4, 2, 2, 1};
    static const unsigned pass_dy[7] = {8, 8, 8, 4, 4, 2, 2};
    const unsigned pass_count = interlace ? 7u : 1u;
    unsigned long long decoded_size64 = 0;
    for (unsigned pass = 0; pass < pass_count; ++pass) {
        const unsigned start_x = interlace ? pass_x[pass] : 0;
        const unsigned start_y = interlace ? pass_y[pass] : 0;
        const unsigned step_x = interlace ? pass_dx[pass] : 1;
        const unsigned step_y = interlace ? pass_dy[pass] : 1;
        const unsigned pass_width = PassExtent(w, start_x, step_x);
        const unsigned pass_height = PassExtent(h, start_y, step_y);
        if (!pass_width || !pass_height)
            continue;
        const unsigned long long row_bits = static_cast<unsigned long long>(pass_width) * bits_per_pixel;
        const unsigned long long row_bytes = (row_bits + 7u) >> 3;
        decoded_size64 += static_cast<unsigned long long>(pass_height) * (row_bytes + 1u);
        if (decoded_size64 > 0xffffffffu)
            return false;
    }
    if (!decoded_size64)
        return false;
    const unsigned decoded_size = static_cast<unsigned>(decoded_size64);

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

    const unsigned pixel_count = w * h;
    if (pixel_count > 0xffffffffu / 4u) {
        free(decoded);
        return false;
    }
    unsigned char* out = static_cast<unsigned char*>(malloc(pixel_count * 4u));
    if (!out) {
        free(decoded);
        return false;
    }
    memset(out, 0, pixel_count * 4u);

    const unsigned long long max_row_bits64 = static_cast<unsigned long long>(w) * bits_per_pixel;
    const unsigned long long max_row_bytes64 = (max_row_bits64 + 7u) >> 3;
    if (!max_row_bytes64 || max_row_bytes64 > 0x7fffffffu) {
        free(decoded);
        free(out);
        return false;
    }
    const unsigned max_row_bytes = static_cast<unsigned>(max_row_bytes64);
    unsigned char* rows = static_cast<unsigned char*>(malloc(max_row_bytes * 2u));
    if (!rows) {
        free(decoded);
        free(out);
        return false;
    }

    unsigned decoded_position = 0;
    bool valid = true;
    for (unsigned pass = 0; pass < pass_count && valid; ++pass) {
        const unsigned start_x = interlace ? pass_x[pass] : 0;
        const unsigned start_y = interlace ? pass_y[pass] : 0;
        const unsigned step_x = interlace ? pass_dx[pass] : 1;
        const unsigned step_y = interlace ? pass_dy[pass] : 1;
        const unsigned pass_width = PassExtent(w, start_x, step_x);
        const unsigned pass_height = PassExtent(h, start_y, step_y);
        if (!pass_width || !pass_height)
            continue;
        const unsigned row_bytes = static_cast<unsigned>((static_cast<unsigned long long>(pass_width) * bits_per_pixel + 7u) >> 3);
        unsigned bytes_per_pixel = (bits_per_pixel + 7u) >> 3;
        if (!bytes_per_pixel)
            bytes_per_pixel = 1;
        unsigned char* previous = rows;
        unsigned char* current = rows + max_row_bytes;
        memset(previous, 0, row_bytes);
        for (unsigned y = 0; y < pass_height; ++y) {
            if (decoded_position >= decoded_size || row_bytes > decoded_size - decoded_position - 1u) {
                valid = false;
                break;
            }
            const unsigned filter = decoded[decoded_position++];
            const unsigned char* source = decoded + decoded_position;
            decoded_position += row_bytes;
            if (!UnfilterRow(current, source, y == 0 ? 0 : previous, row_bytes, bytes_per_pixel, filter)) {
                valid = false;
                break;
            }
            const unsigned destination_y = start_y + y * step_y;
            for (unsigned x = 0; x < pass_width; ++x) {
                const unsigned destination_x = start_x + x * step_x;
                unsigned char* destination = out + (destination_y * w + destination_x) * 4u;
                if (!DecodePixel(current, x, color_type, bit_depth, palette, palette_bytes, transparent, destination)) {
                    valid = false;
                    break;
                }
            }
            unsigned char* swap = previous;
            previous = current;
            current = swap;
        }
    }

    free(rows);
    free(decoded);
    if (!valid || decoded_position != decoded_size) {
        free(out);
        return false;
    }
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

unsigned int Crc32(const unsigned char* data, unsigned length) {
    unsigned int crc = 0xffffffffu;
    for (unsigned i = 0; i < length; ++i) {
        crc ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
    return crc ^ 0xffffffffu;
}

void StoreBig32(unsigned char* out, unsigned int value) {
    out[0] = static_cast<unsigned char>(value >> 24);
    out[1] = static_cast<unsigned char>(value >> 16);
    out[2] = static_cast<unsigned char>(value >> 8);
    out[3] = static_cast<unsigned char>(value);
}

bool WriteChunk(std::ostream& stream, const char type[4], const unsigned char* data, unsigned length) {
    unsigned char header[8];
    StoreBig32(header, length);
    memcpy(header + 4, type, 4);
    stream.write(reinterpret_cast<const char*>(header), sizeof(header));
    if (length)
        stream.write(reinterpret_cast<const char*>(data), length);

    unsigned char* crc_data = static_cast<unsigned char*>(malloc(length + 4u));
    if (!crc_data)
        return false;
    memcpy(crc_data, type, 4);
    if (length)
        memcpy(crc_data + 4, data, length);
    const unsigned int crc = Crc32(crc_data, length + 4u);
    free(crc_data);
    unsigned char crc_bytes[4];
    StoreBig32(crc_bytes, crc);
    stream.write(reinterpret_cast<const char*>(crc_bytes), sizeof(crc_bytes));
    return static_cast<bool>(stream);
}

bool WritePNG(std::ostream& stream, uint32_t width, uint32_t height, uint32_t* data) {
    if (!data || width == 0 || height == 0 || width > 4096 || height > 4096)
        return false;
    if (width > (0xffffffffu - 1u) / 4u)
        return false;
    const unsigned row_bytes = width * 4u;
    if (height > 0xffffffffu / (row_bytes + 1u))
        return false;
    const unsigned raw_size = height * (row_bytes + 1u);
    unsigned char* raw = static_cast<unsigned char*>(malloc(raw_size));
    if (!raw)
        return false;

    for (unsigned y = 0; y < height; ++y) {
        unsigned char* row = raw + y * (row_bytes + 1u);
        row[0] = 0;
        for (unsigned x = 0; x < width; ++x) {
            const uint32_t pixel = data[y * width + x];
            const unsigned alpha = (pixel >> 24) & 0xffu;
            unsigned red = (pixel >> 16) & 0xffu;
            unsigned green = (pixel >> 8) & 0xffu;
            unsigned blue = pixel & 0xffu;
            if (alpha) {
                red = std::min(255u, (red * 255u + alpha / 2u) / alpha);
                green = std::min(255u, (green * 255u + alpha / 2u) / alpha);
                blue = std::min(255u, (blue * 255u + alpha / 2u) / alpha);
            } else {
                red = green = blue = 0;
            }
            unsigned char* out = row + 1u + x * 4u;
            out[0] = static_cast<unsigned char>(red);
            out[1] = static_cast<unsigned char>(green);
            out[2] = static_cast<unsigned char>(blue);
            out[3] = static_cast<unsigned char>(alpha);
        }
    }

    unsigned long compressed_capacity = compressBound(raw_size);
    unsigned char* compressed = static_cast<unsigned char*>(malloc(compressed_capacity));
    if (!compressed) {
        free(raw);
        return false;
    }
    unsigned long compressed_size = compressed_capacity;
    const int zlib_result = compress2(compressed, &compressed_size, raw, raw_size, 1);
    free(raw);
    if (zlib_result != 0 || compressed_size > 0xffffffffu) {
        free(compressed);
        return false;
    }

    static const unsigned char signature[8] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    stream.write(reinterpret_cast<const char*>(signature), sizeof(signature));
    unsigned char ihdr[13];
    StoreBig32(ihdr, width);
    StoreBig32(ihdr + 4, height);
    ihdr[8] = 8;
    ihdr[9] = 6;
    ihdr[10] = 0;
    ihdr[11] = 0;
    ihdr[12] = 0;
    const bool okay = WriteChunk(stream, "IHDR", ihdr, sizeof(ihdr)) &&
        WriteChunk(stream, "IDAT", compressed, static_cast<unsigned>(compressed_size)) &&
        WriteChunk(stream, "IEND", NULL, 0);
    free(compressed);
    return okay && static_cast<bool>(stream);
}
}
