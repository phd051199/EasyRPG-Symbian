#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <string>

#ifndef EASYRPG_SYMBIAN
#include <zlib.h>
#else
typedef unsigned char Bytef;
typedef unsigned int uInt;
typedef unsigned long uLong;
typedef void *voidpf;
typedef voidpf (*alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void (*free_func)(voidpf opaque, voidpf address);
struct z_stream_s
{
    Bytef *next_in;
    uInt avail_in;
    uLong total_in;
    Bytef *next_out;
    uInt avail_out;
    uLong total_out;
    char *msg;
    void *state;
    alloc_func zalloc;
    free_func zfree;
    voidpf opaque;
    int data_type;
    uLong adler;
    uLong reserved;
};
typedef z_stream_s z_stream;
extern "C" const char *zlibVersion(void);
extern "C" int inflateInit_(z_stream *stream, const char *version, int stream_size);
extern "C" int inflate(z_stream *stream, int flush);
extern "C" int inflateEnd(z_stream *stream);
#define Z_NO_FLUSH 0
#define Z_OK 0
#define Z_STREAM_END 1
#define inflateInit(stream) inflateInit_((stream), zlibVersion(), sizeof(z_stream))
#endif

#include "image_bmp.h"
#include "image_png.h"
#include "image_xyz.h"
#include "UpstreamPlayerBridge.h"

extern "C" int uncompress(unsigned char *dest, unsigned long *dest_len, const unsigned char *source, unsigned long source_len);
extern "C" int compress2(unsigned char *dest, unsigned long *dest_len, const unsigned char *source, unsigned long source_len, int level);
extern "C" unsigned long compressBound(unsigned long source_len);

#ifdef EASYRPG_SYMBIAN
extern "C" int EasyRpgPlatformFreeRamBytes(void);
extern "C" int EasyRpgUpstreamPlayerTrimMemory(int pressure_level);
extern "C" int EasyRpgPlatformPrepareAllocation(int bytes);
#endif

namespace
{
unsigned short Read16(const unsigned char *p)
{
    return static_cast<unsigned short>(p[0] | (static_cast<unsigned short>(p[1]) << 8));
}

unsigned int Read32(const unsigned char *p)
{
    return static_cast<unsigned int>(p[0]) |
           (static_cast<unsigned int>(p[1]) << 8) |
           (static_cast<unsigned int>(p[2]) << 16) |
           (static_cast<unsigned int>(p[3]) << 24);
}

void PrepareImageAllocation(unsigned pixel_count, unsigned transient_bytes)
{
#ifdef EASYRPG_SYMBIAN
    if (!pixel_count)
        return;
    const unsigned long long rgba_bytes = static_cast<unsigned long long>(pixel_count) * 4ull;
    const unsigned long long final_bytes = static_cast<unsigned long long>(pixel_count) * 2ull;
    const unsigned long long required = rgba_bytes + final_bytes + transient_bytes + 6ull * 1024ull * 1024ull;
    const int free_ram = EasyRpgPlatformFreeRamBytes();
    if (free_ram < 0 || static_cast<unsigned long long>(free_ram) >= required)
        return;
    const unsigned long long bounded = std::min<unsigned long long>(required, 0x7fffffffull);
    EasyRpgPlatformPrepareAllocation(static_cast<int>(bounded));
#else
    (void)pixel_count;
    (void)transient_bytes;
#endif
}

unsigned DirectPixelBytes(int pixel_format)
{
    if (pixel_format == EEasyRpgDirectImageRgb565 ||
        pixel_format == EEasyRpgDirectImageA1555)
        return 2u;
    if (pixel_format == EEasyRpgDirectImageBgra8888)
        return 4u;
    return 0u;
}

void StoreDirectPixel(unsigned char *destination,
                      int pixel_format,
                      unsigned char r,
                      unsigned char g,
                      unsigned char b,
                      unsigned char a)
{
    if (pixel_format == EEasyRpgDirectImageBgra8888)
    {
        const unsigned pr = (static_cast<unsigned>(r) * a + 127u) / 255u;
        const unsigned pg = (static_cast<unsigned>(g) * a + 127u) / 255u;
        const unsigned pb = (static_cast<unsigned>(b) * a + 127u) / 255u;
        destination[0] = static_cast<unsigned char>(pb);
        destination[1] = static_cast<unsigned char>(pg);
        destination[2] = static_cast<unsigned char>(pr);
        destination[3] = a;
        return;
    }
    unsigned short value = 0;
    if (pixel_format == EEasyRpgDirectImageRgb565)
    {
        value = static_cast<unsigned short>(
            ((static_cast<unsigned>(r) >> 3) << 11) |
            ((static_cast<unsigned>(g) >> 2) << 5) |
            (static_cast<unsigned>(b) >> 3));
    }
    else if (pixel_format == EEasyRpgDirectImageA1555)
    {
        if (a >= 128)
        {
            value = static_cast<unsigned short>(
                0x8000u |
                ((static_cast<unsigned>(r) >> 3) << 10) |
                ((static_cast<unsigned>(g) >> 3) << 5) |
                (static_cast<unsigned>(b) >> 3));
        }
    }
    destination[0] = static_cast<unsigned char>(value & 0xffu);
    destination[1] = static_cast<unsigned char>(value >> 8);
}
} // namespace

namespace ImageXYZ
{
bool ReadXYZ(const uint8_t *data, unsigned len, bool transparent, int &width, int &height, void *&pixels)
{
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
    PrepareImageAllocation(pixel_count, raw_size);
    unsigned char *raw = static_cast<unsigned char *>(malloc(raw_size));
    if (!raw)
        return false;

    unsigned long decoded = raw_size;
    if (uncompress(raw, &decoded, data + 8, len - 8) != 0 || decoded != raw_size)
    {
        free(raw);
        return false;
    }

    if (pixel_count > 0xffffffffu / 4u)
    {
        free(raw);
        return false;
    }
    unsigned char *out = static_cast<unsigned char *>(malloc(pixel_count * 4u));
    if (!out)
    {
        free(raw);
        return false;
    }

    const unsigned char *palette = raw;
    const unsigned char *indices = raw + 768;
    for (unsigned i = 0; i < pixel_count; ++i)
    {
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

bool ReadXYZStreamTarget(FILE *stream,
                         bool transparent,
                         int &width,
                         int &height,
                         void *&pixels,
                         int direct_format,
                         void *direct_pixels,
                         int direct_pitch,
                         int expected_width,
                         int expected_height)
{
    width = 0;
    height = 0;
    pixels = 0;
    if (!stream || fseek(stream, 0, SEEK_SET) != 0)
        return false;

    unsigned char header[8];
    if (fread(header, 1, sizeof(header), stream) != sizeof(header) ||
        memcmp(header, "XYZ1", 4) != 0)
        return false;
    const unsigned w = Read16(header + 4);
    const unsigned h = Read16(header + 6);
    if (!w || !h || w > 4096 || h > 4096 || h > 0xffffffffu / w)
        return false;
    const unsigned pixel_count = w * h;

    unsigned char *out = 0;
    if (direct_format != EEasyRpgDirectImageNone)
    {
        if (!direct_pixels || DirectPixelBytes(direct_format) != 2u ||
            expected_width != static_cast<int>(w) || expected_height != static_cast<int>(h) ||
            direct_pitch < static_cast<int>(w * 2u))
            return false;
    }
    else
    {
        if (pixel_count > 0xffffffffu / 4u)
            return false;
        PrepareImageAllocation(pixel_count, 32u * 1024u);
        out = static_cast<unsigned char *>(malloc(pixel_count * 4u));
        if (!out)
            return false;
    }

    unsigned char palette[768];
    unsigned palette_filled = 0;
    unsigned pixel_position = 0;
    unsigned char input[8192];
    unsigned char decoded[8192];
    z_stream z;
    memset(&z, 0, sizeof(z));
    if (inflateInit(&z) != Z_OK)
    {
        free(out);
        return false;
    }

    bool valid = true;
    bool finished = false;
    while (valid && !finished)
    {
        const size_t bytes = fread(input, 1, sizeof(input), stream);
        if (bytes == 0)
        {
            valid = false;
            break;
        }
        z.next_in = input;
        z.avail_in = static_cast<uInt>(bytes);
        while (valid && z.avail_in > 0)
        {
            z.next_out = decoded;
            z.avail_out = sizeof(decoded);
            const int result = inflate(&z, Z_NO_FLUSH);
            if (result != Z_OK && result != Z_STREAM_END)
            {
                valid = false;
                break;
            }
            const unsigned produced = static_cast<unsigned>(sizeof(decoded) - z.avail_out);
            unsigned offset = 0;
            if (palette_filled < sizeof(palette))
            {
                const unsigned copy = std::min<unsigned>(produced, sizeof(palette) - palette_filled);
                memcpy(palette + palette_filled, decoded, copy);
                palette_filled += copy;
                offset += copy;
            }
            while (offset < produced && valid)
            {
                if (pixel_position >= pixel_count)
                {
                    valid = false;
                    break;
                }
                const unsigned index = decoded[offset++];
                const unsigned char *color = palette + index * 3u;
                const unsigned char alpha = transparent && index == 0 ? 0 : 255;
                if (direct_format != EEasyRpgDirectImageNone)
                {
                    const unsigned y = pixel_position / w;
                    const unsigned x = pixel_position - y * w;
                    unsigned char *destination = static_cast<unsigned char *>(direct_pixels) +
                                                 y * direct_pitch + x * 2u;
                    StoreDirectPixel(destination, direct_format, color[0], color[1], color[2], alpha);
                }
                else
                {
                    unsigned char *destination = out + pixel_position * 4u;
                    destination[0] = color[0];
                    destination[1] = color[1];
                    destination[2] = color[2];
                    destination[3] = alpha;
                }
                ++pixel_position;
            }
            if (result == Z_STREAM_END)
            {
                finished = true;
                if (z.avail_in != 0)
                    valid = false;
                break;
            }
            if (produced == 0 && z.avail_in > 0)
            {
                valid = false;
                break;
            }
        }
    }
    inflateEnd(&z);

    if (!valid || !finished || palette_filled != sizeof(palette) || pixel_position != pixel_count)
    {
        free(out);
        return false;
    }
    width = static_cast<int>(w);
    height = static_cast<int>(h);
    pixels = out;
    return true;
}

bool ReadXYZ(FILE *stream, bool transparent, int &width, int &height, void *&pixels)
{
    return ReadXYZStreamTarget(stream, transparent, width, height, pixels, EEasyRpgDirectImageNone, 0, 0, 0, 0);
}

bool ReadXYZDirect(FILE *stream,
                   bool transparent,
                   int pixel_format,
                   int width,
                   int height,
                   void *pixels,
                   int pitch_bytes)
{
    int decoded_width = 0;
    int decoded_height = 0;
    void *ignored = 0;
    return ReadXYZStreamTarget(stream, transparent, decoded_width, decoded_height, ignored, pixel_format, pixels, pitch_bytes, width, height);
}
} // namespace ImageXYZ

namespace ImageBMP
{
bool ReadBMP(const uint8_t *data, unsigned len, bool transparent, int &width, int &height, void *&pixels)
{
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

    unsigned char *out = static_cast<unsigned char *>(malloc(pixel_count * 4u));
    if (!out)
        return false;

    const unsigned palette_offset = 14u + dib_size;
    unsigned palette_count = 0;
    if (bpp <= 8)
    {
        const unsigned max_palette = 1u << bpp;
        palette_count = Read32(data + 46);
        if (!palette_count)
            palette_count = max_palette;
        if (palette_count > max_palette || palette_offset > len || palette_count > (len - palette_offset) / 4u)
        {
            free(out);
            return false;
        }
    }

    for (unsigned y = 0; y < h; ++y)
    {
        const unsigned sy = bottom_up ? h - 1u - y : y;
        const unsigned char *row = data + pixel_offset + sy * row_bytes;
        for (int x = 0; x < w; ++x)
        {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a = 255;
            if (bpp <= 8)
            {
                unsigned index = 0;
                if (bpp == 8)
                    index = row[x];
                else if (bpp == 4)
                    index = (row[x >> 1] >> ((x & 1) ? 0 : 4)) & 0x0fu;
                else
                    index = (row[x >> 3] >> (7 - (x & 7))) & 1u;
                if (index >= palette_count)
                {
                    free(out);
                    return false;
                }
                const unsigned char *entry = data + palette_offset + index * 4u;
                b = entry[0];
                g = entry[1];
                r = entry[2];
                if (transparent && index == 0)
                    a = 0;
            }
            else if (bpp == 24)
            {
                const unsigned char *entry = row + x * 3;
                b = entry[0];
                g = entry[1];
                r = entry[2];
            }
            else
            {
                const unsigned char *entry = row + x * 4;
                b = entry[0];
                g = entry[1];
                r = entry[2];
                a = transparent ? entry[3] : 255;
            }
            unsigned char *dst = out + (y * static_cast<unsigned>(w) + static_cast<unsigned>(x)) * 4u;
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

bool ReadBMP(FILE *stream, bool transparent, int &width, int &height, void *&pixels)
{
    pixels = 0;
    width = 0;
    height = 0;
    if (!stream || fseek(stream, 0, SEEK_END) != 0)
        return false;
    const long file_size_long = ftell(stream);
    if (file_size_long < 54 || fseek(stream, 0, SEEK_SET) != 0)
        return false;
    const unsigned long file_size = static_cast<unsigned long>(file_size_long);

    unsigned char header[54];
    if (fread(header, 1, sizeof(header), stream) != sizeof(header) ||
        header[0] != 'B' || header[1] != 'M')
        return false;

    const unsigned pixel_offset = Read32(header + 10);
    const unsigned dib_size = Read32(header + 14);
    if (dib_size < 40 || 14ull + dib_size > file_size)
        return false;
    const int w = static_cast<int>(Read32(header + 18));
    const int signed_h = static_cast<int>(Read32(header + 22));
    const unsigned bpp = Read16(header + 28);
    const unsigned compression = Read32(header + 30);
    if (w <= 0 || signed_h == 0 || signed_h == static_cast<int>(0x80000000u) ||
        compression != 0 ||
        (bpp != 1 && bpp != 4 && bpp != 8 && bpp != 24 && bpp != 32))
        return false;

    const bool bottom_up = signed_h > 0;
    const unsigned h = static_cast<unsigned>(bottom_up ? signed_h : -signed_h);
    if (static_cast<unsigned>(w) > 0xffffffffu / h)
        return false;
    const unsigned pixel_count = static_cast<unsigned>(w) * h;
    if (pixel_count > 0xffffffffu / 4u ||
        static_cast<unsigned>(w) > (0xffffffffu - 31u) / bpp)
        return false;
    const unsigned row_bytes = ((static_cast<unsigned>(w) * bpp + 31u) / 32u) * 4u;
    if (!row_bytes || pixel_offset > file_size ||
        static_cast<unsigned long long>(row_bytes) * h > file_size - pixel_offset)
        return false;

    unsigned char palette[256 * 4];
    unsigned palette_count = 0;
    if (bpp <= 8)
    {
        const unsigned max_palette = 1u << bpp;
        palette_count = Read32(header + 46);
        if (!palette_count)
            palette_count = max_palette;
        const unsigned palette_offset = 14u + dib_size;
        if (palette_count > max_palette || palette_count > 256 ||
            palette_offset > file_size || palette_count * 4u > file_size - palette_offset ||
            fseek(stream, static_cast<long>(palette_offset), SEEK_SET) != 0 ||
            fread(palette, 4u, palette_count, stream) != palette_count)
            return false;
    }

    PrepareImageAllocation(pixel_count, row_bytes);
    unsigned char *out = static_cast<unsigned char *>(malloc(pixel_count * 4u));
    unsigned char *row = static_cast<unsigned char *>(malloc(row_bytes));
    if (!out || !row)
    {
        free(row);
        free(out);
        return false;
    }
    if (fseek(stream, static_cast<long>(pixel_offset), SEEK_SET) != 0)
    {
        free(row);
        free(out);
        return false;
    }

    bool valid = true;
    for (unsigned file_y = 0; file_y < h && valid; ++file_y)
    {
        if (fread(row, 1, row_bytes, stream) != row_bytes)
        {
            valid = false;
            break;
        }
        const unsigned y = bottom_up ? h - 1u - file_y : file_y;
        for (int x = 0; x < w; ++x)
        {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a = 255;
            if (bpp <= 8)
            {
                unsigned index = 0;
                if (bpp == 8)
                    index = row[x];
                else if (bpp == 4)
                    index = (row[x >> 1] >> ((x & 1) ? 0 : 4)) & 0x0fu;
                else
                    index = (row[x >> 3] >> (7 - (x & 7))) & 1u;
                if (index >= palette_count)
                {
                    valid = false;
                    break;
                }
                const unsigned char *entry = palette + index * 4u;
                b = entry[0];
                g = entry[1];
                r = entry[2];
                if (transparent && index == 0)
                    a = 0;
            }
            else if (bpp == 24)
            {
                const unsigned char *entry = row + x * 3;
                b = entry[0];
                g = entry[1];
                r = entry[2];
            }
            else
            {
                const unsigned char *entry = row + x * 4;
                b = entry[0];
                g = entry[1];
                r = entry[2];
                a = transparent ? entry[3] : 255;
            }
            unsigned char *dst = out + (y * static_cast<unsigned>(w) + static_cast<unsigned>(x)) * 4u;
            dst[0] = r;
            dst[1] = g;
            dst[2] = b;
            dst[3] = a;
        }
    }

    free(row);
    if (!valid)
    {
        free(out);
        return false;
    }
    width = w;
    height = static_cast<int>(h);
    pixels = out;
    return true;
}

bool ReadBMPDirect(FILE *stream,
                   bool transparent,
                   int pixel_format,
                   int width,
                   int height,
                   void *pixels,
                   int pitch_bytes)
{
    const unsigned destination_bpp = DirectPixelBytes(pixel_format);
    if (!stream || !pixels || width <= 0 || height <= 0 || !destination_bpp ||
        pitch_bytes < static_cast<int>(width * destination_bpp) || fseek(stream, 0, SEEK_END) != 0)
        return false;
    const long file_size_long = ftell(stream);
    if (file_size_long < 54 || fseek(stream, 0, SEEK_SET) != 0)
        return false;
    const unsigned long file_size = static_cast<unsigned long>(file_size_long);

    unsigned char header[54];
    if (fread(header, 1, sizeof(header), stream) != sizeof(header) ||
        header[0] != 'B' || header[1] != 'M')
        return false;
    const unsigned pixel_offset = Read32(header + 10);
    const unsigned dib_size = Read32(header + 14);
    const int w = static_cast<int>(Read32(header + 18));
    const int signed_h = static_cast<int>(Read32(header + 22));
    const unsigned bpp = Read16(header + 28);
    const unsigned compression = Read32(header + 30);
    if (dib_size < 40 || 14ull + dib_size > file_size || w != width || signed_h == 0 ||
        signed_h == static_cast<int>(0x80000000u) ||
        static_cast<unsigned>(signed_h > 0 ? signed_h : -signed_h) != static_cast<unsigned>(height) ||
        compression != 0 || (bpp != 1 && bpp != 4 && bpp != 8 && bpp != 24 && bpp != 32))
        return false;
    if (transparent && bpp == 32 && pixel_format != EEasyRpgDirectImageBgra8888)
        return false;

    const bool bottom_up = signed_h > 0;
    if (static_cast<unsigned>(width) > (0xffffffffu - 31u) / bpp)
        return false;
    const unsigned row_bytes = ((static_cast<unsigned>(width) * bpp + 31u) / 32u) * 4u;
    if (!row_bytes || pixel_offset > file_size ||
        static_cast<unsigned long long>(row_bytes) * static_cast<unsigned>(height) > file_size - pixel_offset)
        return false;

    unsigned char palette[256 * 4];
    unsigned palette_count = 0;
    if (bpp <= 8)
    {
        const unsigned max_palette = 1u << bpp;
        palette_count = Read32(header + 46);
        if (!palette_count)
            palette_count = max_palette;
        const unsigned palette_offset = 14u + dib_size;
        if (palette_count > max_palette || palette_count > 256 ||
            palette_offset > file_size || palette_count * 4u > file_size - palette_offset ||
            fseek(stream, static_cast<long>(palette_offset), SEEK_SET) != 0 ||
            fread(palette, 4u, palette_count, stream) != palette_count)
            return false;
    }

    PrepareImageAllocation(static_cast<unsigned>(width) * static_cast<unsigned>(height), row_bytes);
    unsigned char *source_row = static_cast<unsigned char *>(malloc(row_bytes));
    if (!source_row || fseek(stream, static_cast<long>(pixel_offset), SEEK_SET) != 0)
    {
        free(source_row);
        return false;
    }

    unsigned char *destination = static_cast<unsigned char *>(pixels);
    bool valid = true;
    for (int file_y = 0; file_y < height && valid; ++file_y)
    {
        if (fread(source_row, 1, row_bytes, stream) != row_bytes)
        {
            valid = false;
            break;
        }
        const int y = bottom_up ? height - 1 - file_y : file_y;
        unsigned char *destination_row = destination + y * pitch_bytes;
        for (int x = 0; x < width; ++x)
        {
            unsigned char r = 0;
            unsigned char g = 0;
            unsigned char b = 0;
            unsigned char a = 255;
            if (bpp <= 8)
            {
                unsigned index = 0;
                if (bpp == 8)
                    index = source_row[x];
                else if (bpp == 4)
                    index = (source_row[x >> 1] >> ((x & 1) ? 0 : 4)) & 0x0fu;
                else
                    index = (source_row[x >> 3] >> (7 - (x & 7))) & 1u;
                if (index >= palette_count)
                {
                    valid = false;
                    break;
                }
                const unsigned char *entry = palette + index * 4u;
                b = entry[0];
                g = entry[1];
                r = entry[2];
                if (transparent && index == 0)
                    a = 0;
            }
            else if (bpp == 24)
            {
                const unsigned char *entry = source_row + x * 3;
                b = entry[0];
                g = entry[1];
                r = entry[2];
            }
            else
            {
                const unsigned char *entry = source_row + x * 4;
                b = entry[0];
                g = entry[1];
                r = entry[2];
                a = transparent ? entry[3] : 255;
            }
            StoreDirectPixel(destination_row + x * destination_bpp, pixel_format, r, g, b, a);
        }
    }
    free(source_row);
    return valid;
}
} // namespace ImageBMP

namespace ImagePNG
{
unsigned int ReadBig32(const unsigned char *p)
{
    return (static_cast<unsigned int>(p[0]) << 24) |
           (static_cast<unsigned int>(p[1]) << 16) |
           (static_cast<unsigned int>(p[2]) << 8) |
           static_cast<unsigned int>(p[3]);
}

unsigned char Paeth(unsigned char a, unsigned char b, unsigned char c)
{
    const int p = static_cast<int>(a) + static_cast<int>(b) - static_cast<int>(c);
    const int pa = p > static_cast<int>(a) ? p - static_cast<int>(a) : static_cast<int>(a) - p;
    const int pb = p > static_cast<int>(b) ? p - static_cast<int>(b) : static_cast<int>(b) - p;
    const int pc = p > static_cast<int>(c) ? p - static_cast<int>(c) : static_cast<int>(c) - p;
    return pa <= pb && pa <= pc ? a : (pb <= pc ? b : c);
}

bool ValidBitDepth(unsigned color_type, unsigned bit_depth)
{
    if (color_type == 0)
        return bit_depth == 1 || bit_depth == 2 || bit_depth == 4 || bit_depth == 8 || bit_depth == 16;
    if (color_type == 2 || color_type == 4 || color_type == 6)
        return bit_depth == 8 || bit_depth == 16;
    if (color_type == 3)
        return bit_depth == 1 || bit_depth == 2 || bit_depth == 4 || bit_depth == 8;
    return false;
}

unsigned ChannelCount(unsigned color_type)
{
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

unsigned PassExtent(unsigned size, unsigned start, unsigned step)
{
    return size <= start ? 0 : 1u + (size - 1u - start) / step;
}

unsigned ReadSample(const unsigned char *row, unsigned sample, unsigned bit_depth)
{
    if (bit_depth == 8)
        return row[sample];
    if (bit_depth == 16)
    {
        const unsigned offset = sample * 2u;
        return (static_cast<unsigned>(row[offset]) << 8) | row[offset + 1u];
    }
    const unsigned bit_offset = sample * bit_depth;
    const unsigned shift = 8u - bit_depth - (bit_offset & 7u);
    return (row[bit_offset >> 3] >> shift) & ((1u << bit_depth) - 1u);
}

unsigned char ScaleSample(unsigned value, unsigned bit_depth)
{
    if (bit_depth == 16)
        return static_cast<unsigned char>(value >> 8);
    if (bit_depth == 8)
        return static_cast<unsigned char>(value);
    const unsigned maximum = (1u << bit_depth) - 1u;
    return static_cast<unsigned char>((value * 255u + maximum / 2u) / maximum);
}

bool UnfilterRow(unsigned char *destination,
                 const unsigned char *source,
                 const unsigned char *previous,
                 unsigned row_bytes,
                 unsigned bytes_per_pixel,
                 unsigned filter)
{
    if (filter > 4)
        return false;
    for (unsigned x = 0; x < row_bytes; ++x)
    {
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

bool DecodePixel(const unsigned char *row,
                 unsigned x,
                 unsigned color_type,
                 unsigned bit_depth,
                 const unsigned char *palette,
                 unsigned palette_bytes,
                 bool transparent,
                 unsigned char &out_r,
                 unsigned char &out_g,
                 unsigned char &out_b,
                 unsigned char &out_a)
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;
    const unsigned channels = ChannelCount(color_type);
    const unsigned sample = x * channels;

    if (color_type == 0)
    {
        const unsigned char gray = ScaleSample(ReadSample(row, sample, bit_depth), bit_depth);
        r = gray;
        g = gray;
        b = gray;
        if (transparent && gray == 0)
            a = 0;
    }
    else if (color_type == 2)
    {
        r = ScaleSample(ReadSample(row, sample, bit_depth), bit_depth);
        g = ScaleSample(ReadSample(row, sample + 1u, bit_depth), bit_depth);
        b = ScaleSample(ReadSample(row, sample + 2u, bit_depth), bit_depth);
    }
    else if (color_type == 3)
    {
        const unsigned index = ReadSample(row, x, bit_depth);
        const unsigned palette_count = palette_bytes / 3u;
        if (!palette || index >= palette_count)
            return false;
        r = palette[index * 3u];
        g = palette[index * 3u + 1u];
        b = palette[index * 3u + 2u];
        if (transparent && index == 0)
            a = 0;
    }
    else if (color_type == 4)
    {
        const unsigned char gray = ScaleSample(ReadSample(row, sample, bit_depth), bit_depth);
        r = gray;
        g = gray;
        b = gray;
        a = ScaleSample(ReadSample(row, sample + 1u, bit_depth), bit_depth);
    }
    else if (color_type == 6)
    {
        r = ScaleSample(ReadSample(row, sample, bit_depth), bit_depth);
        g = ScaleSample(ReadSample(row, sample + 1u, bit_depth), bit_depth);
        b = ScaleSample(ReadSample(row, sample + 2u, bit_depth), bit_depth);
        a = ScaleSample(ReadSample(row, sample + 3u, bit_depth), bit_depth);
    }
    else
    {
        return false;
    }

    out_r = r;
    out_g = g;
    out_b = b;
    out_a = a;
    return true;
}

struct PngStreamRows
{
    unsigned width;
    unsigned height;
    unsigned bit_depth;
    unsigned color_type;
    unsigned interlace;
    unsigned bits_per_pixel;
    unsigned pass;
    unsigned pass_width;
    unsigned pass_height;
    unsigned pass_row;
    unsigned row_bytes;
    unsigned bytes_per_pixel;
    unsigned row_fill;
    unsigned max_row_bytes;
    unsigned char *scanline;
    unsigned char *previous;
    unsigned char *current;
    bool rows_done;
};

bool SetupPngStreamPass(PngStreamRows &state)
{
    static const unsigned pass_x[7] = {0, 4, 0, 2, 0, 1, 0};
    static const unsigned pass_y[7] = {0, 0, 4, 0, 2, 0, 1};
    static const unsigned pass_dx[7] = {8, 8, 4, 4, 2, 2, 1};
    static const unsigned pass_dy[7] = {8, 8, 8, 4, 4, 2, 2};
    const unsigned pass_count = state.interlace ? 7u : 1u;
    while (state.pass < pass_count)
    {
        const unsigned start_x = state.interlace ? pass_x[state.pass] : 0;
        const unsigned start_y = state.interlace ? pass_y[state.pass] : 0;
        const unsigned step_x = state.interlace ? pass_dx[state.pass] : 1;
        const unsigned step_y = state.interlace ? pass_dy[state.pass] : 1;
        state.pass_width = PassExtent(state.width, start_x, step_x);
        state.pass_height = PassExtent(state.height, start_y, step_y);
        if (!state.pass_width || !state.pass_height)
        {
            ++state.pass;
            continue;
        }
        const unsigned long long row_bits =
            static_cast<unsigned long long>(state.pass_width) * state.bits_per_pixel;
        const unsigned long long row_bytes = (row_bits + 7u) >> 3;
        if (!row_bytes || row_bytes > state.max_row_bytes)
            return false;
        state.row_bytes = static_cast<unsigned>(row_bytes);
        state.bytes_per_pixel = (state.bits_per_pixel + 7u) >> 3;
        if (!state.bytes_per_pixel)
            state.bytes_per_pixel = 1;
        state.pass_row = 0;
        state.row_fill = 0;
        memset(state.previous, 0, state.row_bytes);
        return true;
    }
    state.rows_done = true;
    state.row_bytes = 0;
    state.row_fill = 0;
    return true;
}

bool ProcessPngStreamRow(PngStreamRows &state,
                         const unsigned char *palette,
                         unsigned palette_bytes,
                         bool transparent,
                         unsigned char *out,
                         int direct_format,
                         void *direct_pixels,
                         int direct_pitch)
{
    static const unsigned pass_x[7] = {0, 4, 0, 2, 0, 1, 0};
    static const unsigned pass_y[7] = {0, 0, 4, 0, 2, 0, 1};
    static const unsigned pass_dx[7] = {8, 8, 4, 4, 2, 2, 1};
    static const unsigned pass_dy[7] = {8, 8, 8, 4, 4, 2, 2};
    if (state.rows_done || state.row_fill != state.row_bytes + 1u)
        return false;
    const unsigned filter = state.scanline[0];
    if (!UnfilterRow(state.current, state.scanline + 1, state.pass_row == 0 ? 0 : state.previous, state.row_bytes, state.bytes_per_pixel, filter))
        return false;

    const unsigned start_x = state.interlace ? pass_x[state.pass] : 0;
    const unsigned start_y = state.interlace ? pass_y[state.pass] : 0;
    const unsigned step_x = state.interlace ? pass_dx[state.pass] : 1;
    const unsigned step_y = state.interlace ? pass_dy[state.pass] : 1;
    const unsigned destination_y = start_y + state.pass_row * step_y;
    for (unsigned x = 0; x < state.pass_width; ++x)
    {
        const unsigned destination_x = start_x + x * step_x;
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        unsigned char a = 255;
        if (!DecodePixel(state.current, x, state.color_type, state.bit_depth, palette, palette_bytes, transparent, r, g, b, a))
            return false;
        if (direct_format != EEasyRpgDirectImageNone)
        {
            const unsigned destination_bpp = DirectPixelBytes(direct_format);
            unsigned char *destination = static_cast<unsigned char *>(direct_pixels) +
                                         destination_y * direct_pitch + destination_x * destination_bpp;
            StoreDirectPixel(destination, direct_format, r, g, b, a);
        }
        else
        {
            unsigned char *destination = out +
                                         (destination_y * state.width + destination_x) * 4u;
            destination[0] = r;
            destination[1] = g;
            destination[2] = b;
            destination[3] = a;
        }
    }

    unsigned char *swap = state.previous;
    state.previous = state.current;
    state.current = swap;
    state.row_fill = 0;
    ++state.pass_row;
    if (state.pass_row >= state.pass_height)
    {
        ++state.pass;
        return SetupPngStreamPass(state);
    }
    return true;
}

bool SkipPngFileBytes(FILE *stream, unsigned bytes)
{
    unsigned char discard[1024];
    while (bytes > 0)
    {
        const unsigned chunk = std::min<unsigned>(bytes, sizeof(discard));
        if (fread(discard, 1, chunk, stream) != chunk)
            return false;
        bytes -= chunk;
    }
    return true;
}

bool ReadPNGStreamTarget(FILE *stream,
                         bool transparent,
                         int &width,
                         int &height,
                         void *&pixels,
                         int direct_format,
                         void *direct_pixels,
                         int direct_pitch,
                         int expected_width,
                         int expected_height)
{
    width = 0;
    height = 0;
    pixels = 0;
    static const unsigned char png_signature[8] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    if (!stream || fseek(stream, 0, SEEK_SET) != 0)
        return false;
    unsigned char signature[8];
    if (fread(signature, 1, sizeof(signature), stream) != sizeof(signature) ||
        memcmp(signature, png_signature, sizeof(signature)) != 0)
        return false;

    unsigned w = 0;
    unsigned h = 0;
    unsigned bit_depth = 0;
    unsigned color_type = 0;
    unsigned interlace = 0;
    unsigned char palette[768];
    unsigned palette_bytes = 0;
    bool saw_ihdr = false;
    bool saw_idat = false;
    bool saw_iend = false;
    bool stream_finished = false;
    bool decoder_ready = false;
    unsigned char *out = 0;
    unsigned char *row_storage = 0;
    z_stream z;
    memset(&z, 0, sizeof(z));
    PngStreamRows rows;
    memset(&rows, 0, sizeof(rows));

    bool valid = true;
    while (valid && !saw_iend)
    {
        unsigned char chunk_header[8];
        if (fread(chunk_header, 1, sizeof(chunk_header), stream) != sizeof(chunk_header))
        {
            valid = false;
            break;
        }
        const unsigned chunk_size = ReadBig32(chunk_header);
        const unsigned char *type = chunk_header + 4;
        if (memcmp(type, "IHDR", 4) == 0)
        {
            unsigned char ihdr[13];
            if (saw_ihdr || chunk_size != sizeof(ihdr) ||
                fread(ihdr, 1, sizeof(ihdr), stream) != sizeof(ihdr))
            {
                valid = false;
            }
            else
            {
                w = ReadBig32(ihdr);
                h = ReadBig32(ihdr + 4);
                bit_depth = ihdr[8];
                color_type = ihdr[9];
                interlace = ihdr[12];
                if (!w || !h || w > 4096 || h > 4096 || h > 0xffffffffu / w ||
                    ihdr[10] != 0 || ihdr[11] != 0 || interlace > 1 ||
                    !ValidBitDepth(color_type, bit_depth))
                    valid = false;
                else
                    saw_ihdr = true;
            }
        }
        else if (memcmp(type, "PLTE", 4) == 0)
        {
            if (!saw_ihdr || saw_idat || chunk_size == 0 || chunk_size > sizeof(palette) ||
                chunk_size % 3u != 0 || fread(palette, 1, chunk_size, stream) != chunk_size)
            {
                valid = false;
            }
            else
            {
                palette_bytes = chunk_size;
            }
        }
        else if (memcmp(type, "IDAT", 4) == 0)
        {
            if (!saw_ihdr || stream_finished)
            {
                valid = false;
            }
            else
            {
                if (!decoder_ready)
                {
                    if (color_type == 3 && palette_bytes == 0)
                    {
                        valid = false;
                    }
                    else
                    {
                        const unsigned channels = ChannelCount(color_type);
                        const unsigned bits_per_pixel = channels * bit_depth;
                        const unsigned long long max_row_bits = static_cast<unsigned long long>(w) * bits_per_pixel;
                        const unsigned long long max_row_bytes = (max_row_bits + 7u) >> 3;
                        const unsigned pixel_count = w * h;
                        if (!channels || !bits_per_pixel || !max_row_bytes || max_row_bytes > 0x7fffffffu ||
                            pixel_count > 0xffffffffu / 4u)
                        {
                            valid = false;
                        }
                        else if (direct_format != EEasyRpgDirectImageNone &&
                                 (!direct_pixels || DirectPixelBytes(direct_format) == 0u ||
                                  expected_width != static_cast<int>(w) || expected_height != static_cast<int>(h) ||
                                  direct_pitch < static_cast<int>(w * DirectPixelBytes(direct_format))))
                        {
                            valid = false;
                        }
                        else
                        {
                            const unsigned transient = static_cast<unsigned>(max_row_bytes) * 3u + 16u * 1024u;
                            PrepareImageAllocation(pixel_count, transient);
                            if (direct_format == EEasyRpgDirectImageNone)
                            {
                                out = static_cast<unsigned char *>(malloc(pixel_count * 4u));
                                if (out)
                                    memset(out, 0, pixel_count * 4u);
                            }
                            row_storage = static_cast<unsigned char *>(malloc(static_cast<size_t>(max_row_bytes) * 3u + 1u));
                            if ((direct_format == EEasyRpgDirectImageNone && !out) || !row_storage)
                            {
                                valid = false;
                            }
                            else
                            {
                                rows.width = w;
                                rows.height = h;
                                rows.bit_depth = bit_depth;
                                rows.color_type = color_type;
                                rows.interlace = interlace;
                                rows.bits_per_pixel = bits_per_pixel;
                                rows.max_row_bytes = static_cast<unsigned>(max_row_bytes);
                                rows.scanline = row_storage;
                                rows.previous = row_storage + max_row_bytes + 1u;
                                rows.current = rows.previous + max_row_bytes;
                                rows.pass = 0;
                                rows.rows_done = false;
                                if (!SetupPngStreamPass(rows) || inflateInit(&z) != Z_OK)
                                    valid = false;
                                else
                                    decoder_ready = true;
                            }
                        }
                    }
                }

                unsigned remaining = chunk_size;
                unsigned char input[8192];
                while (valid && remaining > 0)
                {
                    const unsigned input_size = std::min<unsigned>(remaining, sizeof(input));
                    if (fread(input, 1, input_size, stream) != input_size)
                    {
                        valid = false;
                        break;
                    }
                    remaining -= input_size;
                    z.next_in = input;
                    z.avail_in = input_size;
                    while (valid && z.avail_in > 0)
                    {
                        unsigned char dummy = 0;
                        const uInt output_capacity = rows.rows_done
                                                         ? 1u
                                                         : static_cast<uInt>(rows.row_bytes + 1u - rows.row_fill);
                        z.next_out = rows.rows_done ? &dummy : rows.scanline + rows.row_fill;
                        z.avail_out = output_capacity;
                        const int result = inflate(&z, Z_NO_FLUSH);
                        if (result != Z_OK && result != Z_STREAM_END)
                        {
                            valid = false;
                            break;
                        }
                        const unsigned produced = output_capacity - z.avail_out;
                        if (rows.rows_done)
                        {
                            if (produced != 0)
                                valid = false;
                        }
                        else
                        {
                            rows.row_fill += produced;
                            if (rows.row_fill == rows.row_bytes + 1u &&
                                !ProcessPngStreamRow(rows, palette, palette_bytes, transparent, out, direct_format, direct_pixels, direct_pitch))
                                valid = false;
                        }
                        if (result == Z_STREAM_END)
                        {
                            stream_finished = true;
                            if (!rows.rows_done || z.avail_in != 0 || remaining != 0)
                                valid = false;
                            break;
                        }
                        if (produced == 0 && z.avail_in == input_size)
                        {
                            valid = false;
                            break;
                        }
                    }
                }
                saw_idat = true;
            }
        }
        else if (memcmp(type, "IEND", 4) == 0)
        {
            if (chunk_size != 0 || !saw_idat || !stream_finished || !rows.rows_done)
                valid = false;
            saw_iend = true;
        }
        else
        {
            if (!SkipPngFileBytes(stream, chunk_size))
                valid = false;
        }

        unsigned char crc[4];
        if (valid && fread(crc, 1, sizeof(crc), stream) != sizeof(crc))
            valid = false;
    }

    if (decoder_ready)
        inflateEnd(&z);
    free(row_storage);
    if (!valid || !saw_iend)
    {
        free(out);
        return false;
    }
    width = static_cast<int>(w);
    height = static_cast<int>(h);
    pixels = out;
    return true;
}

bool ReadPNGBytesTarget(const uint8_t *data,
                        unsigned len,
                        bool transparent,
                        int &width,
                        int &height,
                        void *&pixels,
                        int direct_format,
                        void *direct_pixels,
                        int direct_pitch,
                        int expected_width,
                        int expected_height)
{
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
    const unsigned char *palette = 0;
    unsigned palette_bytes = 0;
    unsigned compressed_size = 0;
    unsigned idat_count = 0;
    const unsigned char *single_idat = 0;
    bool saw_ihdr = false;
    bool saw_iend = false;

    unsigned position = 8;
    while (position <= len - 12)
    {
        const unsigned chunk_size = ReadBig32(data + position);
        if (chunk_size > len - position - 12)
            return false;
        const unsigned char *type = data + position + 4;
        const unsigned char *chunk = data + position + 8;
        if (memcmp(type, "IHDR", 4) == 0)
        {
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
        }
        else if (memcmp(type, "PLTE", 4) == 0)
        {
            palette = chunk;
            palette_bytes = chunk_size;
        }
        else if (memcmp(type, "IDAT", 4) == 0)
        {
            if (chunk_size > 0xffffffffu - compressed_size)
                return false;
            compressed_size += chunk_size;
            ++idat_count;
            if (idat_count == 1)
                single_idat = chunk;
            else
                single_idat = 0;
        }
        else if (memcmp(type, "IEND", 4) == 0)
        {
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
    for (unsigned pass = 0; pass < pass_count; ++pass)
    {
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

    const unsigned pixel_count = w * h;
    if (pixel_count > 0xffffffffu / 4u)
        return false;
    PrepareImageAllocation(pixel_count,
                           decoded_size + (idat_count > 1 ? compressed_size : 0u));

    unsigned char *compressed = idat_count > 1
                                    ? static_cast<unsigned char *>(malloc(compressed_size))
                                    : 0;
    unsigned char *decoded = static_cast<unsigned char *>(malloc(decoded_size));
    if ((idat_count > 1 && !compressed) || !decoded)
    {
        free(compressed);
        free(decoded);
        return false;
    }

    if (idat_count > 1)
    {
        unsigned write_position = 0;
        position = 8;
        while (position <= len - 12)
        {
            const unsigned chunk_size = ReadBig32(data + position);
            const unsigned char *type = data + position + 4;
            if (memcmp(type, "IDAT", 4) == 0)
            {
                memcpy(compressed + write_position, data + position + 8, chunk_size);
                write_position += chunk_size;
            }
            if (memcmp(type, "IEND", 4) == 0)
                break;
            position += chunk_size + 12;
        }
    }

    unsigned long actual_size = decoded_size;
    const unsigned char *compressed_source = idat_count == 1 ? single_idat : compressed;
    if (!compressed_source ||
        uncompress(decoded, &actual_size, compressed_source, compressed_size) != 0 ||
        actual_size != decoded_size)
    {
        free(compressed);
        free(decoded);
        return false;
    }
    free(compressed);

    unsigned char *out = 0;
    if (direct_format != EEasyRpgDirectImageNone)
    {
        if (!direct_pixels || DirectPixelBytes(direct_format) != 2u ||
            expected_width != static_cast<int>(w) || expected_height != static_cast<int>(h) ||
            direct_pitch < static_cast<int>(w * 2u))
        {
            free(decoded);
            return false;
        }
        memset(direct_pixels, 0, static_cast<size_t>(direct_pitch) * h);
    }
    else
    {
        out = static_cast<unsigned char *>(malloc(pixel_count * 4u));
        if (!out)
        {
            free(decoded);
            return false;
        }
        memset(out, 0, pixel_count * 4u);
    }

    const unsigned long long max_row_bits64 = static_cast<unsigned long long>(w) * bits_per_pixel;
    const unsigned long long max_row_bytes64 = (max_row_bits64 + 7u) >> 3;
    if (!max_row_bytes64 || max_row_bytes64 > 0x7fffffffu)
    {
        free(decoded);
        free(out);
        return false;
    }
    const unsigned max_row_bytes = static_cast<unsigned>(max_row_bytes64);
    unsigned char *rows = static_cast<unsigned char *>(malloc(max_row_bytes * 2u));
    if (!rows)
    {
        free(decoded);
        free(out);
        return false;
    }

    unsigned decoded_position = 0;
    bool valid = true;
    for (unsigned pass = 0; pass < pass_count && valid; ++pass)
    {
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
        unsigned char *previous = rows;
        unsigned char *current = rows + max_row_bytes;
        memset(previous, 0, row_bytes);
        for (unsigned y = 0; y < pass_height; ++y)
        {
            if (decoded_position >= decoded_size || row_bytes > decoded_size - decoded_position - 1u)
            {
                valid = false;
                break;
            }
            const unsigned filter = decoded[decoded_position++];
            const unsigned char *source = decoded + decoded_position;
            decoded_position += row_bytes;
            if (!UnfilterRow(current, source, y == 0 ? 0 : previous, row_bytes, bytes_per_pixel, filter))
            {
                valid = false;
                break;
            }
            const unsigned destination_y = start_y + y * step_y;
            for (unsigned x = 0; x < pass_width; ++x)
            {
                const unsigned destination_x = start_x + x * step_x;
                unsigned char r = 0;
                unsigned char g = 0;
                unsigned char b = 0;
                unsigned char a = 255;
                if (!DecodePixel(current, x, color_type, bit_depth, palette, palette_bytes, transparent, r, g, b, a))
                {
                    valid = false;
                    break;
                }
                if (direct_format != EEasyRpgDirectImageNone)
                {
                    unsigned char *destination = static_cast<unsigned char *>(direct_pixels) +
                                                 destination_y * direct_pitch + destination_x * 2u;
                    StoreDirectPixel(destination, direct_format, r, g, b, a);
                }
                else
                {
                    unsigned char *destination = out + (destination_y * w + destination_x) * 4u;
                    destination[0] = r;
                    destination[1] = g;
                    destination[2] = b;
                    destination[3] = a;
                }
            }
            unsigned char *swap = previous;
            previous = current;
            current = swap;
        }
    }

    free(rows);
    free(decoded);
    if (!valid || decoded_position != decoded_size)
    {
        free(out);
        return false;
    }
    width = static_cast<int>(w);
    height = static_cast<int>(h);
    pixels = out;
    return true;
}

bool ReadPNGBytes(const uint8_t *data,
                  unsigned len,
                  bool transparent,
                  int &width,
                  int &height,
                  void *&pixels)
{
    return ReadPNGBytesTarget(data, len, transparent, width, height, pixels, EEasyRpgDirectImageNone, 0, 0, 0, 0);
}

bool ReadPNGDirect(FILE *stream,
                   bool transparent,
                   int pixel_format,
                   int width,
                   int height,
                   void *pixels,
                   int pitch_bytes)
{
    int decoded_width = 0;
    int decoded_height = 0;
    void *ignored = 0;
    return ReadPNGStreamTarget(stream, transparent, decoded_width, decoded_height, ignored, pixel_format, pixels, pitch_bytes, width, height);
}

bool ReadPNG(FILE *stream, const void *buffer, bool transparent, int &width, int &height, void *&pixels)
{
    if (stream)
        return ReadPNGStreamTarget(stream, transparent, width, height, pixels, EEasyRpgDirectImageNone, 0, 0, 0, 0);
    width = 0;
    height = 0;
    pixels = 0;
    (void)buffer;
    return false;
}

unsigned int Crc32(const unsigned char *data, unsigned length)
{
    unsigned int crc = 0xffffffffu;
    for (unsigned i = 0; i < length; ++i)
    {
        crc ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
    return crc ^ 0xffffffffu;
}

void StoreBig32(unsigned char *out, unsigned int value)
{
    out[0] = static_cast<unsigned char>(value >> 24);
    out[1] = static_cast<unsigned char>(value >> 16);
    out[2] = static_cast<unsigned char>(value >> 8);
    out[3] = static_cast<unsigned char>(value);
}

bool WriteChunk(std::ostream &stream, const char type[4], const unsigned char *data, unsigned length)
{
    unsigned char header[8];
    StoreBig32(header, length);
    memcpy(header + 4, type, 4);
    stream.write(reinterpret_cast<const char *>(header), sizeof(header));
    if (length)
        stream.write(reinterpret_cast<const char *>(data), length);

    unsigned char *crc_data = static_cast<unsigned char *>(malloc(length + 4u));
    if (!crc_data)
        return false;
    memcpy(crc_data, type, 4);
    if (length)
        memcpy(crc_data + 4, data, length);
    const unsigned int crc = Crc32(crc_data, length + 4u);
    free(crc_data);
    unsigned char crc_bytes[4];
    StoreBig32(crc_bytes, crc);
    stream.write(reinterpret_cast<const char *>(crc_bytes), sizeof(crc_bytes));
    return static_cast<bool>(stream);
}

bool WritePNG(std::ostream &stream, uint32_t width, uint32_t height, uint32_t *data)
{
    if (!data || width == 0 || height == 0 || width > 4096 || height > 4096)
        return false;
    if (width > (0xffffffffu - 1u) / 4u)
        return false;
    const unsigned row_bytes = width * 4u;
    if (height > 0xffffffffu / (row_bytes + 1u))
        return false;
    const unsigned raw_size = height * (row_bytes + 1u);
    unsigned char *raw = static_cast<unsigned char *>(malloc(raw_size));
    if (!raw)
        return false;

    for (unsigned y = 0; y < height; ++y)
    {
        unsigned char *row = raw + y * (row_bytes + 1u);
        row[0] = 0;
        for (unsigned x = 0; x < width; ++x)
        {
            const uint32_t pixel = data[y * width + x];
            const unsigned alpha = (pixel >> 24) & 0xffu;
            unsigned red = (pixel >> 16) & 0xffu;
            unsigned green = (pixel >> 8) & 0xffu;
            unsigned blue = pixel & 0xffu;
            if (alpha)
            {
                red = std::min(255u, (red * 255u + alpha / 2u) / alpha);
                green = std::min(255u, (green * 255u + alpha / 2u) / alpha);
                blue = std::min(255u, (blue * 255u + alpha / 2u) / alpha);
            }
            else
            {
                red = green = blue = 0;
            }
            unsigned char *out = row + 1u + x * 4u;
            out[0] = static_cast<unsigned char>(red);
            out[1] = static_cast<unsigned char>(green);
            out[2] = static_cast<unsigned char>(blue);
            out[3] = static_cast<unsigned char>(alpha);
        }
    }

    unsigned long compressed_capacity = compressBound(raw_size);
    unsigned char *compressed = static_cast<unsigned char *>(malloc(compressed_capacity));
    if (!compressed)
    {
        free(raw);
        return false;
    }
    unsigned long compressed_size = compressed_capacity;
    const int zlib_result = compress2(compressed, &compressed_size, raw, raw_size, 1);
    free(raw);
    if (zlib_result != 0 || compressed_size > 0xffffffffu)
    {
        free(compressed);
        return false;
    }

    static const unsigned char signature[8] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    stream.write(reinterpret_cast<const char *>(signature), sizeof(signature));
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
} // namespace ImagePNG

extern "C" int EasyRpgPlatformProbeDirectImage(void *stream_ptr,
                                               int transparent,
                                               unsigned int flags,
                                               int *width,
                                               int *height,
                                               int *pixel_format)
{
    if (!stream_ptr || !width || !height || !pixel_format || (flags & 2u) != 0)
        return 0;

    FILE *stream = static_cast<FILE *>(stream_ptr);
    if (fseek(stream, 0, SEEK_SET) != 0)
        return 0;
    unsigned char header[54];
    const size_t read = fread(header, 1, sizeof(header), stream);
    fseek(stream, 0, SEEK_SET);

    int w = 0;
    int h = 0;
    int format = EEasyRpgDirectImageNone;
    if (read >= 8 && memcmp(header, "XYZ1", 4) == 0)
    {
        w = static_cast<int>(Read16(header + 4));
        h = static_cast<int>(Read16(header + 6));
        format = transparent ? EEasyRpgDirectImageA1555 : EEasyRpgDirectImageRgb565;
    }
    else if (read >= 54 && header[0] == 'B' && header[1] == 'M')
    {
        const unsigned dib_size = Read32(header + 14);
        const int signed_h = static_cast<int>(Read32(header + 22));
        const unsigned bpp = Read16(header + 28);
        const unsigned compression = Read32(header + 30);
        w = static_cast<int>(Read32(header + 18));
        if (dib_size < 40 || w <= 0 || signed_h == 0 || signed_h == static_cast<int>(0x80000000u) ||
            compression != 0 || (bpp != 1 && bpp != 4 && bpp != 8 && bpp != 24 && bpp != 32))
            return 0;
        h = signed_h > 0 ? signed_h : -signed_h;
        format = transparent
                     ? (bpp == 32 ? EEasyRpgDirectImageBgra8888 : EEasyRpgDirectImageA1555)
                     : EEasyRpgDirectImageRgb565;
    }
    else if (read >= 33)
    {
        static const unsigned char png_signature[8] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
        if (memcmp(header, png_signature, sizeof(png_signature)) != 0 ||
            ImagePNG::ReadBig32(header + 8) != 13u || memcmp(header + 12, "IHDR", 4) != 0)
            return 0;
        w = static_cast<int>(ImagePNG::ReadBig32(header + 16));
        h = static_cast<int>(ImagePNG::ReadBig32(header + 20));
        const unsigned bit_depth = header[24];
        const unsigned color_type = header[25];
        if (w <= 0 || h <= 0 || w > 4096 || h > 4096 ||
            !ImagePNG::ValidBitDepth(color_type, bit_depth))
            return 0;
        format = transparent
                     ? ((color_type == 4 || color_type == 6)
                            ? EEasyRpgDirectImageBgra8888
                            : EEasyRpgDirectImageA1555)
                     : EEasyRpgDirectImageRgb565;
    }

    if (w <= 0 || h <= 0 || format == EEasyRpgDirectImageNone)
        return 0;
    *width = w;
    *height = h;
    *pixel_format = format;
    return 1;
}

extern "C" int EasyRpgPlatformDecodeDirectImage(void *stream_ptr,
                                                int transparent,
                                                int pixel_format,
                                                int width,
                                                int height,
                                                void *pixels,
                                                int pitch_bytes)
{
    const unsigned destination_bpp = DirectPixelBytes(pixel_format);
    if (!stream_ptr || !pixels || width <= 0 || height <= 0 || !destination_bpp ||
        pitch_bytes < static_cast<int>(width * destination_bpp))
        return 0;
    FILE *stream = static_cast<FILE *>(stream_ptr);
    if (fseek(stream, 0, SEEK_SET) != 0)
        return 0;
    unsigned char signature[8];
    const size_t read = fread(signature, 1, sizeof(signature), stream);
    if (fseek(stream, 0, SEEK_SET) != 0)
        return 0;
    if (read >= 4 && memcmp(signature, "XYZ1", 4) == 0)
        return ImageXYZ::ReadXYZDirect(stream, transparent != 0, pixel_format, width, height, pixels, pitch_bytes) ? 1 : 0;
    if (read >= 2 && signature[0] == 'B' && signature[1] == 'M')
        return ImageBMP::ReadBMPDirect(stream, transparent != 0, pixel_format, width, height, pixels, pitch_bytes) ? 1 : 0;
    static const unsigned char png_signature[8] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    if (read == sizeof(png_signature) && memcmp(signature, png_signature, sizeof(png_signature)) == 0)
        return ImagePNG::ReadPNGDirect(stream, transparent != 0, pixel_format, width, height, pixels, pitch_bytes) ? 1 : 0;
    return 0;
}
