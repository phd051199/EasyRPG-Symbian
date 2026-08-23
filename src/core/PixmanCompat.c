#include <stdlib.h>
#include <string.h>

#include "pixman-private.h"
#include "pixman-combine32.h"

static uint8_t SymbianReadAlpha(bits_image_t* image, int x, int y) {
    bits_image_t* alpha = image->common.alpha_map;
    if (!alpha)
        return 255;
    x -= image->common.alpha_origin_x;
    y -= image->common.alpha_origin_y;
    if (x < 0 || x >= alpha->width || y < 0 || y >= alpha->height)
        return 0;
    return (uint8_t)(alpha->fetch_pixel_32(alpha, x, y) >> 24);
}

static uint32_t SymbianPremultiplyRgb(uint32_t r, uint32_t g, uint32_t b, uint8_t a) {
    r = (r * a + 127u) / 255u;
    g = (g * a + 127u) / 255u;
    b = (b * a + 127u) / 255u;
    return ((uint32_t)a << 24) | (r << 16) | (g << 8) | b;
}

static uint32_t ReadPixel32(bits_image_t* image, int x, int y) {
    if (x < 0 || y < 0 || x >= image->width || y >= image->height)
        return 0;
    uint8_t* row = (uint8_t*)image->bits + image->rowstride * 4 * y;
    switch (image->format) {
    case PIXMAN_a8r8g8b8:
        return ((uint32_t*)row)[x];
    case PIXMAN_x8r8g8b8:
        return ((uint32_t*)row)[x] | 0xff000000u;
    case PIXMAN_a8b8g8r8: {
        uint32_t p = ((uint32_t*)row)[x];
        return (p & 0xff000000u) | ((p & 0x000000ffu) << 16) | (p & 0x0000ff00u) | ((p & 0x00ff0000u) >> 16);
    }
    case PIXMAN_x8b8g8r8: {
        uint32_t p = ((uint32_t*)row)[x];
        return 0xff000000u | ((p & 0x000000ffu) << 16) | (p & 0x0000ff00u) | ((p & 0x00ff0000u) >> 16);
    }
    case PIXMAN_b8g8r8a8: {
        uint32_t p = ((uint32_t*)row)[x];
        return ((p & 0x000000ffu) << 24) | ((p & 0x0000ff00u) << 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0xff000000u) >> 24);
    }
    case PIXMAN_b8g8r8x8: {
        uint32_t p = ((uint32_t*)row)[x];
        return 0xff000000u | ((p & 0x0000ff00u) << 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0xff000000u) >> 24);
    }
    case PIXMAN_r8g8b8a8: {
        uint32_t p = ((uint32_t*)row)[x];
        return ((p & 0x000000ffu) << 24) | ((p & 0xff000000u) >> 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0x0000ff00u) >> 8);
    }
    case PIXMAN_r8g8b8x8: {
        uint32_t p = ((uint32_t*)row)[x];
        return 0xff000000u | ((p & 0xff000000u) >> 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0x0000ff00u) >> 8);
    }
    case PIXMAN_a8:
        return ((uint32_t)row[x]) << 24;
    case PIXMAN_a1:
        return ((((uint32_t*)row)[x >> 5] >> (x & 31)) & 1u) ? 0xff000000u : 0;
    case PIXMAN_r5g6b5: {
        uint16_t p = ((uint16_t*)row)[x];
        uint32_t r = (p >> 11) & 31;
        uint32_t g = (p >> 5) & 63;
        uint32_t b = p & 31;
        r = (r << 3) | (r >> 2);
        g = (g << 2) | (g >> 4);
        b = (b << 3) | (b >> 2);
        return SymbianPremultiplyRgb(r, g, b, SymbianReadAlpha(image, x, y));
    }
    case PIXMAN_b5g6r5: {
        uint16_t p = ((uint16_t*)row)[x];
        uint32_t b = (p >> 11) & 31;
        uint32_t g = (p >> 5) & 63;
        uint32_t r = p & 31;
        r = (r << 3) | (r >> 2);
        g = (g << 2) | (g >> 4);
        b = (b << 3) | (b >> 2);
        return SymbianPremultiplyRgb(r, g, b, SymbianReadAlpha(image, x, y));
    }
    case PIXMAN_a1r5g5b5:
    case PIXMAN_x1r5g5b5: {
        uint16_t p = ((uint16_t*)row)[x];
        uint8_t a = image->format == PIXMAN_a1r5g5b5 && !(p & 0x8000u) ? 0 : 255;
        uint32_t r = (p >> 10) & 31u;
        uint32_t g = (p >> 5) & 31u;
        uint32_t b = p & 31u;
        r = (r << 3) | (r >> 2);
        g = (g << 3) | (g >> 2);
        b = (b << 3) | (b >> 2);
        return SymbianPremultiplyRgb(r, g, b, a);
    }
    case PIXMAN_a1b5g5r5:
    case PIXMAN_x1b5g5r5: {
        uint16_t p = ((uint16_t*)row)[x];
        uint8_t a = image->format == PIXMAN_a1b5g5r5 && !(p & 0x8000u) ? 0 : 255;
        uint32_t b = (p >> 10) & 31u;
        uint32_t g = (p >> 5) & 31u;
        uint32_t r = p & 31u;
        r = (r << 3) | (r >> 2);
        g = (g << 3) | (g >> 2);
        b = (b << 3) | (b >> 2);
        return SymbianPremultiplyRgb(r, g, b, a);
    }
    default:
        return 0;
    }
}

static void WritePixel32(bits_image_t* image, int x, int y, uint32_t p) {
    if (x < 0 || y < 0 || x >= image->width || y >= image->height)
        return;
    uint8_t* row = (uint8_t*)image->bits + image->rowstride * 4 * y;
    switch (image->format) {
    case PIXMAN_a8r8g8b8:
        ((uint32_t*)row)[x] = p;
        break;
    case PIXMAN_x8r8g8b8:
        ((uint32_t*)row)[x] = p & 0x00ffffffu;
        break;
    case PIXMAN_a8b8g8r8:
        ((uint32_t*)row)[x] = (p & 0xff000000u) | ((p & 0x000000ffu) << 16) | (p & 0x0000ff00u) | ((p & 0x00ff0000u) >> 16);
        break;
    case PIXMAN_x8b8g8r8:
        ((uint32_t*)row)[x] = ((p & 0x000000ffu) << 16) | (p & 0x0000ff00u) | ((p & 0x00ff0000u) >> 16);
        break;
    case PIXMAN_b8g8r8a8:
        ((uint32_t*)row)[x] = ((p & 0x000000ffu) << 24) | ((p & 0x0000ff00u) << 8) | ((p & 0x00ff0000u) >> 8) | ((p & 0xff000000u) >> 24);
        break;
    case PIXMAN_b8g8r8x8:
        ((uint32_t*)row)[x] = ((p & 0x000000ffu) << 24) | ((p & 0x0000ff00u) << 8) | ((p & 0x00ff0000u) >> 8);
        break;
    case PIXMAN_r8g8b8a8:
        ((uint32_t*)row)[x] = ((p & 0x00ff0000u) << 8) | ((p & 0x0000ff00u) << 8) | ((p & 0x000000ffu) << 8) | ((p & 0xff000000u) >> 24);
        break;
    case PIXMAN_r8g8b8x8:
        ((uint32_t*)row)[x] = ((p & 0x00ff0000u) << 8) | ((p & 0x0000ff00u) << 8) | ((p & 0x000000ffu) << 8);
        break;
    case PIXMAN_a8:
        row[x] = (uint8_t)(p >> 24);
        break;
    case PIXMAN_a1: {
        uint32_t* word = &((uint32_t*)row)[x >> 5];
        const uint32_t bit = 1u << (x & 31);
        if ((p >> 24) >= 128u)
            *word |= bit;
        else
            *word &= ~bit;
        break;
    }
    case PIXMAN_r5g6b5: {
        uint16_t r = (uint16_t)((p >> 19) & 31);
        uint16_t g = (uint16_t)((p >> 10) & 63);
        uint16_t b = (uint16_t)((p >> 3) & 31);
        ((uint16_t*)row)[x] = (uint16_t)((r << 11) | (g << 5) | b);
        break;
    }
    case PIXMAN_b5g6r5: {
        uint16_t r = (uint16_t)((p >> 19) & 31);
        uint16_t g = (uint16_t)((p >> 10) & 63);
        uint16_t b = (uint16_t)((p >> 3) & 31);
        ((uint16_t*)row)[x] = (uint16_t)((b << 11) | (g << 5) | r);
        break;
    }
    case PIXMAN_a1r5g5b5:
    case PIXMAN_x1r5g5b5: {
        uint16_t r = (uint16_t)((p >> 19) & 31u);
        uint16_t g = (uint16_t)((p >> 11) & 31u);
        uint16_t b = (uint16_t)((p >> 3) & 31u);
        uint16_t a = image->format == PIXMAN_x1r5g5b5 || (p >> 24) >= 128u ? 0x8000u : 0;
        ((uint16_t*)row)[x] = (uint16_t)(a | (r << 10) | (g << 5) | b);
        break;
    }
    case PIXMAN_a1b5g5r5:
    case PIXMAN_x1b5g5r5: {
        uint16_t r = (uint16_t)((p >> 19) & 31u);
        uint16_t g = (uint16_t)((p >> 11) & 31u);
        uint16_t b = (uint16_t)((p >> 3) & 31u);
        uint16_t a = image->format == PIXMAN_x1b5g5r5 || (p >> 24) >= 128u ? 0x8000u : 0;
        ((uint16_t*)row)[x] = (uint16_t)(a | (b << 10) | (g << 5) | r);
        break;
    }
    default:
        break;
    }
}

static void FetchScanline32(bits_image_t* image, int x, int y, int width, uint32_t* buffer, const uint32_t* mask) {
    int i;
    for (i = 0; i < width; ++i) {
        if (!mask || mask[i]) buffer[i] = ReadPixel32(image, x + i, y);
    }
}

static void StoreScanline32(bits_image_t* image, int x, int y, int width, const uint32_t* values) {
    int sourceOffset = 0;
    int i;
    if (y < 0 || y >= image->height || width <= 0)
        return;
    if (x < 0) {
        sourceOffset = -x;
        width -= sourceOffset;
        x = 0;
    }
    if (x + width > image->width)
        width = image->width - x;
    for (i = 0; i < width; ++i)
        WritePixel32(image, x + i, y, values[sourceOffset + i]);
}

static argb_t FetchPixelFloat(bits_image_t* image, int x, int y) {
    uint32_t p = ReadPixel32(image, x, y);
    argb_t v;
    v.a = ((p >> 24) & 255) / 255.0f;
    v.r = ((p >> 16) & 255) / 255.0f;
    v.g = ((p >> 8) & 255) / 255.0f;
    v.b = (p & 255) / 255.0f;
    return v;
}

static void FetchScanlineFloat(bits_image_t* image, int x, int y, int width, uint32_t* buffer, const uint32_t* mask) {
    int i;
    argb_t* out = (argb_t*)buffer;
    for (i = 0; i < width; ++i) {
        if (!mask || mask[i]) out[i] = FetchPixelFloat(image, x + i, y);
    }
}

static void StoreScanlineFloat(bits_image_t* image, int x, int y, int width, const uint32_t* values) {
    int i;
    const argb_t* in = (const argb_t*)values;
    for (i = 0; i < width; ++i) {
        uint32_t a = (uint32_t)CLIP((int)(in[i].a * 255.0f + 0.5f), 0, 255);
        uint32_t r = (uint32_t)CLIP((int)(in[i].r * 255.0f + 0.5f), 0, 255);
        uint32_t g = (uint32_t)CLIP((int)(in[i].g * 255.0f + 0.5f), 0, 255);
        uint32_t b = (uint32_t)CLIP((int)(in[i].b * 255.0f + 0.5f), 0, 255);
        WritePixel32(image, x + i, y, (a << 24) | (r << 16) | (g << 8) | b);
    }
}

void _pixman_bits_image_setup_accessors(bits_image_t* image) {
    image->fetch_scanline_32 = FetchScanline32;
    image->fetch_pixel_32 = ReadPixel32;
    image->store_scanline_32 = StoreScanline32;
    image->fetch_scanline_float = FetchScanlineFloat;
    image->fetch_pixel_float = FetchPixelFloat;
    image->store_scanline_float = StoreScanlineFloat;
}

static force_inline uint32_t SymbianAddBytesSaturate(uint32_t lhs, uint32_t rhs) {
#if defined(__arm__) && defined(__ARM_ARCH) && (__ARM_ARCH >= 6)
    uint32_t result;
    __asm__ volatile ("uqadd8 %0, %1, %2" : "=r"(result) : "r"(lhs), "r"(rhs));
    return result;
#else
    UN8x4_ADD_UN8x4(lhs, rhs);
    return lhs;
#endif
}

static force_inline uint32_t SymbianOver(uint32_t src, uint32_t dest) {
    uint32_t a = ~src >> 24;
    UN8x4_MUL_UN8(dest, a);
    return SymbianAddBytesSaturate(dest, src);
}

static force_inline uint32_t SymbianIn(uint32_t value, uint8_t alpha) {
    uint16_t a = alpha;
    UN8x4_MUL_UN8(value, a);
    return value;
}

static force_inline uint32_t SymbianExpand565(uint16_t value) {
    uint32_t r = (value >> 11) & 31u;
    uint32_t g = (value >> 5) & 63u;
    uint32_t b = value & 31u;
    r = (r << 3) | (r >> 2);
    g = (g << 2) | (g >> 4);
    b = (b << 3) | (b >> 2);
    return 0xff000000u | (r << 16) | (g << 8) | b;
}

static force_inline uint16_t SymbianPack565(uint32_t value) {
    return (uint16_t)((((value >> 19) & 31u) << 11) |
                      (((value >> 10) & 63u) << 5) |
                      ((value >> 3) & 31u));
}

static force_inline uint16_t SymbianA1555To565(uint16_t value) {
    uint16_t r = (uint16_t)((value >> 10) & 31u);
    uint16_t g = (uint16_t)((value >> 5) & 31u);
    uint16_t b = (uint16_t)(value & 31u);
    return (uint16_t)((r << 11) | (((g << 1) | (g >> 4)) << 5) | b);
}

static force_inline uint32_t SymbianExpandA1555(uint16_t value) {
    uint32_t r;
    uint32_t g;
    uint32_t b;
    if ((value & 0x8000u) == 0)
        return 0;
    r = (value >> 10) & 31u;
    g = (value >> 5) & 31u;
    b = value & 31u;
    r = (r << 3) | (r >> 2);
    g = (g << 3) | (g >> 2);
    b = (b << 3) | (b >> 2);
    return 0xff000000u | (r << 16) | (g << 8) | b;
}

static force_inline uint16_t SymbianOver565(uint32_t src, uint16_t dest) {
    uint32_t alpha = src >> 24;
    if (alpha == 255)
        return SymbianPack565(src);
    if (src == 0)
        return dest;
    return SymbianPack565(SymbianOver(src, SymbianExpand565(dest)));
}

static force_inline uint16_t* SymbianLine16(bits_image_t* image, int x, int y) {
    return (uint16_t*)((uint8_t*)image->bits + image->rowstride * 4 * y) + x;
}

static force_inline uint8_t* SymbianLine8(bits_image_t* image, int x, int y) {
    return (uint8_t*)image->bits + image->rowstride * 4 * y + x;
}

static void SymbianFastOver8888(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t* dstLine;
    uint32_t* srcLine;
    int dstStride;
    int srcStride;
    PIXMAN_IMAGE_GET_LINE(dest_image, dest_x, dest_y, uint32_t, dstStride, dstLine, 1);
    PIXMAN_IMAGE_GET_LINE(src_image, src_x, src_y, uint32_t, srcStride, srcLine, 1);
    while (height-- > 0) {
        uint32_t* dst = dstLine;
        uint32_t* src = srcLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        while (w-- > 0) {
            uint32_t s = *src++;
            uint32_t a = s >> 24;
            if (a == 255)
                *dst = s;
            else if (s != 0)
                *dst = SymbianOver(s, *dst);
            ++dst;
        }
    }
}

static void SymbianFastOver8888A8(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t* dstLine;
    uint32_t* srcLine;
    uint8_t* maskLine;
    int dstStride;
    int srcStride;
    int maskStride;
    (void)imp;
    PIXMAN_IMAGE_GET_LINE(dest_image, dest_x, dest_y, uint32_t, dstStride, dstLine, 1);
    PIXMAN_IMAGE_GET_LINE(src_image, src_x, src_y, uint32_t, srcStride, srcLine, 1);
    maskStride = mask_image->bits.rowstride * 4;
    maskLine = SymbianLine8(&mask_image->bits, mask_x, mask_y);
    while (height-- > 0) {
        uint32_t* dst = dstLine;
        uint32_t* src = srcLine;
        uint8_t* mask = maskLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        maskLine += maskStride;
        while (w-- > 0) {
            uint32_t s = *src++;
            uint8_t m = *mask++;
            if (m != 255)
                UN8x4_MUL_UN8(s, m);
            if ((s >> 24) == 255)
                *dst = s;
            else if (s != 0)
                *dst = SymbianOver(s, *dst);
            ++dst;
        }
    }
}

static void SymbianFastOver8888Solid(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t mask = _pixman_image_get_solid(imp, mask_image, dest_image->bits.format);
    uint8_t maskAlpha = (uint8_t)(mask >> 24);
    uint32_t* dstLine;
    uint32_t* srcLine;
    int dstStride;
    int srcStride;
    if (maskAlpha == 0)
        return;
    PIXMAN_IMAGE_GET_LINE(dest_image, dest_x, dest_y, uint32_t, dstStride, dstLine, 1);
    PIXMAN_IMAGE_GET_LINE(src_image, src_x, src_y, uint32_t, srcStride, srcLine, 1);
    while (height-- > 0) {
        uint32_t* dst = dstLine;
        uint32_t* src = srcLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        while (w-- > 0) {
            uint32_t s = *src++;
            if (maskAlpha != 255)
                UN8x4_MUL_UN8(s, maskAlpha);
            if ((s >> 24) == 255)
                *dst = s;
            else if (s != 0)
                *dst = SymbianOver(s, *dst);
            ++dst;
        }
    }
}

static void SymbianFastOverSolid(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t src = _pixman_image_get_solid(imp, src_image, dest_image->bits.format);
    uint32_t srcAlpha = src >> 24;
    uint32_t* dstLine;
    int dstStride;
    if (src == 0)
        return;
    PIXMAN_IMAGE_GET_LINE(dest_image, dest_x, dest_y, uint32_t, dstStride, dstLine, 1);
    while (height-- > 0) {
        uint32_t* dst = dstLine;
        int32_t w = width;
        dstLine += dstStride;
        if (srcAlpha == 255) {
            while (w-- > 0)
                *dst++ = src;
        } else {
            while (w-- > 0) {
                *dst = SymbianOver(src, *dst);
                ++dst;
            }
        }
    }
}

static void SymbianFastOverSolidA8(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t src = _pixman_image_get_solid(imp, src_image, dest_image->bits.format);
    uint32_t srcAlpha = src >> 24;
    uint32_t* dstLine;
    uint8_t* maskLine;
    int dstStride;
    int maskStride;
    if (src == 0)
        return;
    PIXMAN_IMAGE_GET_LINE(dest_image, dest_x, dest_y, uint32_t, dstStride, dstLine, 1);
    maskStride = mask_image->bits.rowstride * 4;
    maskLine = SymbianLine8(&mask_image->bits, mask_x, mask_y);
    while (height-- > 0) {
        uint32_t* dst = dstLine;
        uint8_t* mask = maskLine;
        int32_t w = width;
        dstLine += dstStride;
        maskLine += maskStride;
        while (w-- > 0) {
            uint8_t m = *mask++;
            if (m == 255) {
                if (srcAlpha == 255)
                    *dst = src;
                else
                    *dst = SymbianOver(src, *dst);
            } else if (m != 0) {
                uint32_t s = SymbianIn(src, m);
                *dst = SymbianOver(s, *dst);
            }
            ++dst;
        }
    }
}

static void SymbianFastOver8888To565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint16_t* dstLine;
    uint32_t* srcLine;
    int dstStride;
    int srcStride;
    (void)imp;
    dstStride = dest_image->bits.rowstride * 2;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    PIXMAN_IMAGE_GET_LINE(src_image, src_x, src_y, uint32_t, srcStride, srcLine, 1);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        uint32_t* src = srcLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        while (w-- > 0) {
            uint32_t s = *src++;
            if (s != 0)
                *dst = SymbianOver565(s, *dst);
            ++dst;
        }
    }
}

static void SymbianFastOver8888A8To565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint16_t* dstLine;
    uint32_t* srcLine;
    uint8_t* maskLine;
    int dstStride;
    int srcStride;
    int maskStride;
    (void)imp;
    dstStride = dest_image->bits.rowstride * 2;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    PIXMAN_IMAGE_GET_LINE(src_image, src_x, src_y, uint32_t, srcStride, srcLine, 1);
    maskStride = mask_image->bits.rowstride * 4;
    maskLine = SymbianLine8(&mask_image->bits, mask_x, mask_y);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        uint32_t* src = srcLine;
        uint8_t* mask = maskLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        maskLine += maskStride;
        while (w-- > 0) {
            uint32_t s = *src++;
            uint8_t m = *mask++;
            if (m != 255)
                UN8x4_MUL_UN8(s, m);
            if (s != 0)
                *dst = SymbianOver565(s, *dst);
            ++dst;
        }
    }
}

static void SymbianFastOver8888SolidTo565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t mask = _pixman_image_get_solid(imp, mask_image, PIXMAN_a8r8g8b8);
    uint8_t maskAlpha = (uint8_t)(mask >> 24);
    uint16_t* dstLine;
    uint32_t* srcLine;
    int dstStride;
    int srcStride;
    if (maskAlpha == 0)
        return;
    dstStride = dest_image->bits.rowstride * 2;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    PIXMAN_IMAGE_GET_LINE(src_image, src_x, src_y, uint32_t, srcStride, srcLine, 1);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        uint32_t* src = srcLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        while (w-- > 0) {
            uint32_t s = *src++;
            if (maskAlpha != 255)
                UN8x4_MUL_UN8(s, maskAlpha);
            if (s != 0)
                *dst = SymbianOver565(s, *dst);
            ++dst;
        }
    }
}

static void SymbianFastOverA1555To565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint16_t* dstLine;
    uint16_t* srcLine;
    int dstStride;
    int srcStride;
    (void)imp;
    dstStride = dest_image->bits.rowstride * 2;
    srcStride = src_image->bits.rowstride * 2;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    srcLine = SymbianLine16(&src_image->bits, src_x, src_y);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        uint16_t* src = srcLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        while (w-- > 0) {
            uint16_t s = *src++;
            if (s & 0x8000u)
                *dst = SymbianA1555To565(s);
            ++dst;
        }
    }
}

static void SymbianFastOverA1555A8To565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint16_t* dstLine;
    uint16_t* srcLine;
    uint8_t* maskLine;
    int dstStride;
    int srcStride;
    int maskStride;
    (void)imp;
    dstStride = dest_image->bits.rowstride * 2;
    srcStride = src_image->bits.rowstride * 2;
    maskStride = mask_image->bits.rowstride * 4;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    srcLine = SymbianLine16(&src_image->bits, src_x, src_y);
    maskLine = SymbianLine8(&mask_image->bits, mask_x, mask_y);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        uint16_t* src = srcLine;
        uint8_t* mask = maskLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        maskLine += maskStride;
        while (w-- > 0) {
            uint16_t s = *src++;
            uint8_t m = *mask++;
            if ((s & 0x8000u) && m != 0) {
                if (m == 255)
                    *dst = SymbianA1555To565(s);
                else
                    *dst = SymbianOver565(SymbianIn(SymbianExpandA1555(s), m), *dst);
            }
            ++dst;
        }
    }
}

static void SymbianFastOverA1555SolidTo565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t mask = _pixman_image_get_solid(imp, mask_image, PIXMAN_a8r8g8b8);
    uint8_t maskAlpha = (uint8_t)(mask >> 24);
    uint16_t* dstLine;
    uint16_t* srcLine;
    int dstStride;
    int srcStride;
    if (maskAlpha == 0)
        return;
    dstStride = dest_image->bits.rowstride * 2;
    srcStride = src_image->bits.rowstride * 2;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    srcLine = SymbianLine16(&src_image->bits, src_x, src_y);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        uint16_t* src = srcLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        while (w-- > 0) {
            uint16_t s = *src++;
            if (s & 0x8000u) {
                if (maskAlpha == 255)
                    *dst = SymbianA1555To565(s);
                else
                    *dst = SymbianOver565(SymbianIn(SymbianExpandA1555(s), maskAlpha), *dst);
            }
            ++dst;
        }
    }
}

static void SymbianFastOver565To565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint16_t* dstLine;
    uint16_t* srcLine;
    int dstStride;
    int srcStride;
    (void)imp;
    dstStride = dest_image->bits.rowstride * 2;
    srcStride = src_image->bits.rowstride * 2;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    srcLine = SymbianLine16(&src_image->bits, src_x, src_y);
    while (height-- > 0) {
        memcpy(dstLine, srcLine, (size_t)width * sizeof(uint16_t));
        dstLine += dstStride;
        srcLine += srcStride;
    }
}

static void SymbianFastOver565A8To565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint16_t* dstLine;
    uint16_t* srcLine;
    uint8_t* maskLine;
    int dstStride;
    int srcStride;
    int maskStride;
    (void)imp;
    dstStride = dest_image->bits.rowstride * 2;
    srcStride = src_image->bits.rowstride * 2;
    maskStride = mask_image->bits.rowstride * 4;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    srcLine = SymbianLine16(&src_image->bits, src_x, src_y);
    maskLine = SymbianLine8(&mask_image->bits, mask_x, mask_y);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        uint16_t* src = srcLine;
        uint8_t* mask = maskLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        maskLine += maskStride;
        while (w-- > 0) {
            uint8_t m = *mask++;
            if (m == 255)
                *dst = *src;
            else if (m != 0)
                *dst = SymbianOver565(SymbianIn(SymbianExpand565(*src), m), *dst);
            ++src;
            ++dst;
        }
    }
}

static void SymbianFastOver565SolidTo565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t mask = _pixman_image_get_solid(imp, mask_image, PIXMAN_a8r8g8b8);
    uint8_t maskAlpha = (uint8_t)(mask >> 24);
    uint16_t* dstLine;
    uint16_t* srcLine;
    int dstStride;
    int srcStride;
    if (maskAlpha == 0)
        return;
    dstStride = dest_image->bits.rowstride * 2;
    srcStride = src_image->bits.rowstride * 2;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    srcLine = SymbianLine16(&src_image->bits, src_x, src_y);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        uint16_t* src = srcLine;
        int32_t w = width;
        dstLine += dstStride;
        srcLine += srcStride;
        if (maskAlpha == 255) {
            memcpy(dst, src, (size_t)width * sizeof(uint16_t));
        } else {
            while (w-- > 0) {
                *dst = SymbianOver565(SymbianIn(SymbianExpand565(*src), maskAlpha), *dst);
                ++src;
                ++dst;
            }
        }
    }
}

static void SymbianFastOverSolidTo565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t src = _pixman_image_get_solid(imp, src_image, PIXMAN_a8r8g8b8);
    uint16_t* dstLine;
    int dstStride;
    if (src == 0)
        return;
    dstStride = dest_image->bits.rowstride * 2;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        int32_t w = width;
        dstLine += dstStride;
        while (w-- > 0) {
            *dst = SymbianOver565(src, *dst);
            ++dst;
        }
    }
}

static void SymbianFastOverSolidA8To565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t src = _pixman_image_get_solid(imp, src_image, PIXMAN_a8r8g8b8);
    uint16_t* dstLine;
    uint8_t* maskLine;
    int dstStride;
    int maskStride;
    if (src == 0)
        return;
    dstStride = dest_image->bits.rowstride * 2;
    maskStride = mask_image->bits.rowstride * 4;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    maskLine = SymbianLine8(&mask_image->bits, mask_x, mask_y);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        uint8_t* mask = maskLine;
        int32_t w = width;
        dstLine += dstStride;
        maskLine += maskStride;
        while (w-- > 0) {
            uint8_t m = *mask++;
            if (m == 255)
                *dst = SymbianOver565(src, *dst);
            else if (m != 0)
                *dst = SymbianOver565(SymbianIn(src, m), *dst);
            ++dst;
        }
    }
}

static void SymbianFastSrcSolidTo565(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t src = _pixman_image_get_solid(imp, src_image, PIXMAN_a8r8g8b8);
    uint16_t packed = SymbianPack565(src);
    uint16_t* dstLine;
    int dstStride;
    dstStride = dest_image->bits.rowstride * 2;
    dstLine = SymbianLine16(&dest_image->bits, dest_x, dest_y);
    while (height-- > 0) {
        uint16_t* dst = dstLine;
        int32_t w = width;
        dstLine += dstStride;
        while (w-- > 0)
            *dst++ = packed;
    }
}

static void SymbianFastSrcMemcpy(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    int bpp = PIXMAN_FORMAT_BPP(dest_image->bits.format) / 8;
    int srcStride = src_image->bits.rowstride * 4;
    int dstStride = dest_image->bits.rowstride * 4;
    uint32_t bytes = width * bpp;
    uint8_t* src = (uint8_t*)src_image->bits.bits + src_y * srcStride + src_x * bpp;
    uint8_t* dst = (uint8_t*)dest_image->bits.bits + dest_y * dstStride + dest_x * bpp;
    (void)imp;
    while (height-- > 0) {
        memcpy(dst, src, bytes);
        src += srcStride;
        dst += dstStride;
    }
}

static void SymbianFastSolidFill(pixman_implementation_t* imp, pixman_composite_info_t* info) {
    PIXMAN_COMPOSITE_ARGS(info);
    uint32_t src = _pixman_image_get_solid(imp, src_image, dest_image->bits.format);
    pixman_fill(dest_image->bits.bits,
                dest_image->bits.rowstride,
                PIXMAN_FORMAT_BPP(dest_image->bits.format),
                dest_x,
                dest_y,
                width,
                height,
                src);
}

static const pixman_fast_path_t SymbianFastPaths[] = {
    PIXMAN_STD_FAST_PATH(OVER, a8r8g8b8, null, a8r8g8b8, SymbianFastOver8888),
    PIXMAN_STD_FAST_PATH(OVER, a8r8g8b8, a8, a8r8g8b8, SymbianFastOver8888A8),
    PIXMAN_STD_FAST_PATH(OVER, a8r8g8b8, solid, a8r8g8b8, SymbianFastOver8888Solid),
    PIXMAN_STD_FAST_PATH(OVER, solid, null, a8r8g8b8, SymbianFastOverSolid),
    PIXMAN_STD_FAST_PATH(OVER, solid, a8, a8r8g8b8, SymbianFastOverSolidA8),
    PIXMAN_STD_FAST_PATH(OVER, a8r8g8b8, null, r5g6b5, SymbianFastOver8888To565),
    PIXMAN_STD_FAST_PATH(OVER, a8r8g8b8, a8, r5g6b5, SymbianFastOver8888A8To565),
    PIXMAN_STD_FAST_PATH(OVER, a8r8g8b8, solid, r5g6b5, SymbianFastOver8888SolidTo565),
    PIXMAN_STD_FAST_PATH(OVER, a1r5g5b5, null, r5g6b5, SymbianFastOverA1555To565),
    PIXMAN_STD_FAST_PATH(OVER, a1r5g5b5, a8, r5g6b5, SymbianFastOverA1555A8To565),
    PIXMAN_STD_FAST_PATH(OVER, a1r5g5b5, solid, r5g6b5, SymbianFastOverA1555SolidTo565),
    PIXMAN_STD_FAST_PATH(OVER, r5g6b5, null, r5g6b5, SymbianFastOver565To565),
    PIXMAN_STD_FAST_PATH(OVER, r5g6b5, a8, r5g6b5, SymbianFastOver565A8To565),
    PIXMAN_STD_FAST_PATH(OVER, r5g6b5, solid, r5g6b5, SymbianFastOver565SolidTo565),
    PIXMAN_STD_FAST_PATH(OVER, solid, null, r5g6b5, SymbianFastOverSolidTo565),
    PIXMAN_STD_FAST_PATH(OVER, solid, a8, r5g6b5, SymbianFastOverSolidA8To565),
    PIXMAN_STD_FAST_PATH(SRC, solid, null, r5g6b5, SymbianFastSrcSolidTo565),
    PIXMAN_STD_FAST_PATH(SRC, r5g6b5, null, r5g6b5, SymbianFastSrcMemcpy),
    PIXMAN_STD_FAST_PATH(SRC, a8r8g8b8, null, a8r8g8b8, SymbianFastSrcMemcpy),
    PIXMAN_STD_FAST_PATH(SRC, solid, null, a8r8g8b8, SymbianFastSolidFill),
    { PIXMAN_OP_NONE }
};

pixman_implementation_t* _pixman_implementation_create_symbian_fast_path(pixman_implementation_t* fallback) {
    return _pixman_implementation_create(fallback, SymbianFastPaths);
}

void _pixman_setup_combiner_functions_float(pixman_implementation_t* imp) {
    (void)imp;
}

static void InitUnusedGradient(pixman_image_t* image, pixman_iter_t* iter) {
    (void)image;
    iter->get_scanline = _pixman_iter_get_scanline_noop;
    iter->write_back = 0;
    iter->fini = 0;
}

void _pixman_linear_gradient_iter_init(pixman_image_t* image, pixman_iter_t* iter) { InitUnusedGradient(image, iter); }
void _pixman_radial_gradient_iter_init(pixman_image_t* image, pixman_iter_t* iter) { InitUnusedGradient(image, iter); }
void _pixman_conical_gradient_iter_init(pixman_image_t* image, pixman_iter_t* iter) { InitUnusedGradient(image, iter); }

static int BoxEmpty32(const pixman_box32_t* box) {
    return box->x1 >= box->x2 || box->y1 >= box->y2;
}

static int BoxEmpty16(const pixman_box16_t* box) {
    return box->x1 >= box->x2 || box->y1 >= box->y2;
}

void pixman_region32_init(pixman_region32_t* region) {
    region->extents.x1 = region->extents.y1 = region->extents.x2 = region->extents.y2 = 0;
    region->data = 0;
}

void pixman_region32_init_rect(pixman_region32_t* region, int x, int y, unsigned int width, unsigned int height) {
    region->extents.x1 = x;
    region->extents.y1 = y;
    region->extents.x2 = x + (int)width;
    region->extents.y2 = y + (int)height;
    region->data = 0;
}

pixman_bool_t pixman_region32_init_rects(pixman_region32_t* region, const pixman_box32_t* boxes, int count) {
    int i;
    pixman_region32_init(region);
    if (count <= 0) return TRUE;
    region->extents = boxes[0];
    for (i = 1; i < count; ++i) {
        if (boxes[i].x1 < region->extents.x1) region->extents.x1 = boxes[i].x1;
        if (boxes[i].y1 < region->extents.y1) region->extents.y1 = boxes[i].y1;
        if (boxes[i].x2 > region->extents.x2) region->extents.x2 = boxes[i].x2;
        if (boxes[i].y2 > region->extents.y2) region->extents.y2 = boxes[i].y2;
    }
    return TRUE;
}

void pixman_region32_init_with_extents(pixman_region32_t* region, pixman_box32_t* extents) { region->extents = *extents; region->data = 0; }
void pixman_region32_fini(pixman_region32_t* region) { region->data = 0; }
void pixman_region32_clear(pixman_region32_t* region) { pixman_region32_init(region); }

pixman_bool_t pixman_region32_copy(pixman_region32_t* dest, pixman_region32_t* source) {
    dest->extents = source->extents;
    dest->data = 0;
    return TRUE;
}

pixman_bool_t pixman_region32_intersect(pixman_region32_t* dest, pixman_region32_t* a, pixman_region32_t* b) {
    dest->extents.x1 = MAX(a->extents.x1, b->extents.x1);
    dest->extents.y1 = MAX(a->extents.y1, b->extents.y1);
    dest->extents.x2 = MIN(a->extents.x2, b->extents.x2);
    dest->extents.y2 = MIN(a->extents.y2, b->extents.y2);
    dest->data = 0;
    if (BoxEmpty32(&dest->extents)) pixman_region32_clear(dest);
    return TRUE;
}

pixman_bool_t pixman_region32_intersect_rect(pixman_region32_t* dest, pixman_region32_t* source, int x, int y, unsigned int width, unsigned int height) {
    pixman_region32_t rect;
    pixman_region32_init_rect(&rect, x, y, width, height);
    return pixman_region32_intersect(dest, source, &rect);
}

pixman_bool_t pixman_region32_union(pixman_region32_t* dest, pixman_region32_t* a, pixman_region32_t* b) {
    if (BoxEmpty32(&a->extents)) return pixman_region32_copy(dest, b);
    if (BoxEmpty32(&b->extents)) return pixman_region32_copy(dest, a);
    dest->extents.x1 = MIN(a->extents.x1, b->extents.x1);
    dest->extents.y1 = MIN(a->extents.y1, b->extents.y1);
    dest->extents.x2 = MAX(a->extents.x2, b->extents.x2);
    dest->extents.y2 = MAX(a->extents.y2, b->extents.y2);
    dest->data = 0;
    return TRUE;
}

pixman_bool_t pixman_region32_union_rect(pixman_region32_t* dest, pixman_region32_t* source, int x, int y, unsigned int width, unsigned int height) {
    pixman_region32_t rect;
    pixman_region32_init_rect(&rect, x, y, width, height);
    return pixman_region32_union(dest, source, &rect);
}

void pixman_region32_translate(pixman_region32_t* region, int x, int y) {
    if (x == 0 && y == 0)
        return;
    region->extents.x1 += x; region->extents.x2 += x;
    region->extents.y1 += y; region->extents.y2 += y;
}

pixman_bool_t pixman_region32_not_empty(pixman_region32_t* region) { return !BoxEmpty32(&region->extents); }
pixman_box32_t* pixman_region32_extents(pixman_region32_t* region) { return &region->extents; }
int pixman_region32_n_rects(pixman_region32_t* region) { return BoxEmpty32(&region->extents) ? 0 : 1; }
pixman_box32_t* pixman_region32_rectangles(pixman_region32_t* region, int* count) { if (count) *count = pixman_region32_n_rects(region); return &region->extents; }
pixman_bool_t pixman_region32_equal(pixman_region32_t* a, pixman_region32_t* b) { return memcmp(&a->extents, &b->extents, sizeof(a->extents)) == 0; }
void pixman_region32_reset(pixman_region32_t* region, pixman_box32_t* box) { region->extents = *box; region->data = 0; }
pixman_bool_t pixman_region32_selfcheck(pixman_region32_t* region) { (void)region; return TRUE; }

pixman_bool_t pixman_region32_contains_point(pixman_region32_t* region, int x, int y, pixman_box32_t* box) {
    pixman_bool_t inside = x >= region->extents.x1 && x < region->extents.x2 && y >= region->extents.y1 && y < region->extents.y2;
    if (inside && box) *box = region->extents;
    return inside;
}

pixman_region_overlap_t pixman_region32_contains_rectangle(pixman_region32_t* region, pixman_box32_t* rect) {
    if (rect->x2 <= region->extents.x1 || rect->x1 >= region->extents.x2 || rect->y2 <= region->extents.y1 || rect->y1 >= region->extents.y2) return PIXMAN_REGION_OUT;
    if (rect->x1 >= region->extents.x1 && rect->x2 <= region->extents.x2 && rect->y1 >= region->extents.y1 && rect->y2 <= region->extents.y2) return PIXMAN_REGION_IN;
    return PIXMAN_REGION_PART;
}

void pixman_region_init(pixman_region16_t* region) {
    region->extents.x1 = region->extents.y1 = region->extents.x2 = region->extents.y2 = 0;
    region->data = 0;
}

void pixman_region_init_rect(pixman_region16_t* region, int x, int y, unsigned int width, unsigned int height) {
    region->extents.x1 = (int16_t)x; region->extents.y1 = (int16_t)y;
    region->extents.x2 = (int16_t)(x + width); region->extents.y2 = (int16_t)(y + height);
    region->data = 0;
}

pixman_bool_t pixman_region_init_rects(pixman_region16_t* region, const pixman_box16_t* boxes, int count) {
    int i;
    pixman_region_init(region);
    if (count <= 0) return TRUE;
    region->extents = boxes[0];
    for (i = 1; i < count; ++i) {
        if (boxes[i].x1 < region->extents.x1) region->extents.x1 = boxes[i].x1;
        if (boxes[i].y1 < region->extents.y1) region->extents.y1 = boxes[i].y1;
        if (boxes[i].x2 > region->extents.x2) region->extents.x2 = boxes[i].x2;
        if (boxes[i].y2 > region->extents.y2) region->extents.y2 = boxes[i].y2;
    }
    return TRUE;
}

void pixman_region_init_with_extents(pixman_region16_t* region, pixman_box16_t* extents) { region->extents = *extents; region->data = 0; }
void pixman_region_fini(pixman_region16_t* region) { region->data = 0; }
void pixman_region_clear(pixman_region16_t* region) { pixman_region_init(region); }

pixman_bool_t pixman_region_copy(pixman_region16_t* dest, pixman_region16_t* source) { dest->extents = source->extents; dest->data = 0; return TRUE; }

pixman_bool_t pixman_region_intersect(pixman_region16_t* dest, pixman_region16_t* a, pixman_region16_t* b) {
    dest->extents.x1 = MAX(a->extents.x1, b->extents.x1); dest->extents.y1 = MAX(a->extents.y1, b->extents.y1);
    dest->extents.x2 = MIN(a->extents.x2, b->extents.x2); dest->extents.y2 = MIN(a->extents.y2, b->extents.y2);
    dest->data = 0;
    if (BoxEmpty16(&dest->extents)) pixman_region_clear(dest);
    return TRUE;
}

pixman_bool_t pixman_region_intersect_rect(pixman_region16_t* dest, pixman_region16_t* source, int x, int y, unsigned int width, unsigned int height) {
    pixman_region16_t rect;
    pixman_region_init_rect(&rect, x, y, width, height);
    return pixman_region_intersect(dest, source, &rect);
}

pixman_bool_t pixman_region_union(pixman_region16_t* dest, pixman_region16_t* a, pixman_region16_t* b) {
    if (BoxEmpty16(&a->extents)) return pixman_region_copy(dest, b);
    if (BoxEmpty16(&b->extents)) return pixman_region_copy(dest, a);
    dest->extents.x1 = MIN(a->extents.x1, b->extents.x1); dest->extents.y1 = MIN(a->extents.y1, b->extents.y1);
    dest->extents.x2 = MAX(a->extents.x2, b->extents.x2); dest->extents.y2 = MAX(a->extents.y2, b->extents.y2);
    dest->data = 0;
    return TRUE;
}

pixman_bool_t pixman_region_union_rect(pixman_region16_t* dest, pixman_region16_t* source, int x, int y, unsigned int width, unsigned int height) {
    pixman_region16_t rect;
    pixman_region_init_rect(&rect, x, y, width, height);
    return pixman_region_union(dest, source, &rect);
}

void pixman_region_translate(pixman_region16_t* region, int x, int y) {
    if (x == 0 && y == 0)
        return;
    region->extents.x1 += x; region->extents.x2 += x;
    region->extents.y1 += y; region->extents.y2 += y;
}
pixman_bool_t pixman_region_not_empty(pixman_region16_t* region) { return !BoxEmpty16(&region->extents); }
pixman_box16_t* pixman_region_extents(pixman_region16_t* region) { return &region->extents; }
int pixman_region_n_rects(pixman_region16_t* region) { return BoxEmpty16(&region->extents) ? 0 : 1; }
pixman_box16_t* pixman_region_rectangles(pixman_region16_t* region, int* count) { if (count) *count = pixman_region_n_rects(region); return &region->extents; }
pixman_bool_t pixman_region_equal(pixman_region16_t* a, pixman_region16_t* b) { return memcmp(&a->extents, &b->extents, sizeof(a->extents)) == 0; }
void pixman_region_reset(pixman_region16_t* region, pixman_box16_t* box) { region->extents = *box; region->data = 0; }
pixman_bool_t pixman_region_selfcheck(pixman_region16_t* region) { (void)region; return TRUE; }

pixman_bool_t pixman_region_contains_point(pixman_region16_t* region, int x, int y, pixman_box16_t* box) {
    pixman_bool_t inside = x >= region->extents.x1 && x < region->extents.x2 && y >= region->extents.y1 && y < region->extents.y2;
    if (inside && box) *box = region->extents;
    return inside;
}

pixman_region_overlap_t pixman_region_contains_rectangle(pixman_region16_t* region, pixman_box16_t* rect) {
    if (rect->x2 <= region->extents.x1 || rect->x1 >= region->extents.x2 || rect->y2 <= region->extents.y1 || rect->y1 >= region->extents.y2) return PIXMAN_REGION_OUT;
    if (rect->x1 >= region->extents.x1 && rect->x2 <= region->extents.x2 && rect->y1 >= region->extents.y1 && rect->y2 <= region->extents.y2) return PIXMAN_REGION_IN;
    return PIXMAN_REGION_PART;
}

void pixman_region_set_static_pointers(pixman_box16_t* a, pixman_region16_data_t* b, pixman_region16_data_t* c) { (void)a; (void)b; (void)c; }
