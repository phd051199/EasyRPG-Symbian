#include "rgss/core/RgssBitmapCore.h"

#include <stdlib.h>
#include <string.h>

#include "pixman.h"

namespace rgss {

namespace {

inline void Expand565(unsigned short aPixel, int* aRed, int* aGreen, int* aBlue)
{
    int r = (aPixel >> 11) & 31;
    int g = (aPixel >> 5) & 63;
    int b = aPixel & 31;
    *aRed = (r << 3) | (r >> 2);
    *aGreen = (g << 2) | (g >> 4);
    *aBlue = (b << 3) | (b >> 2);
}

inline unsigned short Pack565(int aRed, int aGreen, int aBlue)
{
    if (aRed < 0) aRed = 0;
    if (aRed > 255) aRed = 255;
    if (aGreen < 0) aGreen = 0;
    if (aGreen > 255) aGreen = 255;
    if (aBlue < 0) aBlue = 0;
    if (aBlue > 255) aBlue = 255;
    return static_cast<unsigned short>(((aRed >> 3) << 11) |
                                       ((aGreen >> 2) << 5) |
                                       (aBlue >> 3));
}

inline int ClampByte(int aValue)
{
    if (aValue < 0) return 0;
    if (aValue > 255) return 255;
    return aValue;
}

/* Manual composite for alpha/mirror/add/sub paths. RGB is stored straight
 * (not premultiplied), so transparent destination bitmaps need explicit
 * source-over alpha math instead of a plain channel lerp. */
void CompositeLoop(RgssBitmapCore* aDst, RgssBitmapCore* aSrc,
                   const Rect& aSrcRect, int aDstX, int aDstY,
                   int aDstW, int aDstH, int aOpacity, bool aMirror,
                   int aBlendType, const unsigned char* aSrcMask)
{
    unsigned short* srcPixels = aSrc->Pixels();
    const int dstW = aDst->Width();
    const int dstH = aDst->Height();
    const int srcW = aSrc->Width();
    const int srcH = aSrc->Height();
    const int srcStride = aSrc->StrideShorts();
    const int srcMaskStride = srcW;

    for (int j = 0; j < aDstH; ++j) {
        const int sourceY = aSrcRect.y + j * aSrcRect.height / aDstH;
        const int dy = aDstY + j;
        if (dy < 0 || dy >= dstH || sourceY < 0 || sourceY >= srcH)
            continue;
        for (int i = 0; i < aDstW; ++i) {
            const int dx = aDstX + i;
            if (dx < 0 || dx >= dstW)
                continue;
            int sx = i * aSrcRect.width / aDstW;
            if (aMirror)
                sx = aSrcRect.width - 1 - sx;
            const int sourceX = aSrcRect.x + sx;
            if (sourceX < 0 || sourceX >= srcW)
                continue;

            int alpha = aOpacity;
            if (aSrcMask)
                alpha = (aSrcMask[sourceY * srcMaskStride + sourceX] *
                         aOpacity + 127) / 255;
            if (alpha <= 0)
                continue;

            int sr, sg, sb, dr, dg, db;
            Expand565(srcPixels[sourceY * srcStride + sourceX], &sr, &sg, &sb);
            if (!aDst->GetPixel(dx, dy, &dr, &dg, &db))
                continue;
            const int destAlpha = aDst->AlphaAt(dx, dy);

            if (aBlendType == KBlendAdditive && destAlpha == 255) {
                aDst->SetPixelRgba(dx, dy,
                    dr + sr * alpha / 255,
                    dg + sg * alpha / 255,
                    db + sb * alpha / 255, 255);
                continue;
            }
            if (aBlendType == KBlendSubtractive && destAlpha == 255) {
                aDst->SetPixelRgba(dx, dy,
                    dr - sr * alpha / 255,
                    dg - sg * alpha / 255,
                    db - sb * alpha / 255, 255);
                continue;
            }

            if (aBlendType == KBlendAdditive) {
                sr = ClampByte(dr + sr);
                sg = ClampByte(dg + sg);
                sb = ClampByte(db + sb);
            } else if (aBlendType == KBlendSubtractive) {
                sr = ClampByte(dr - sr);
                sg = ClampByte(dg - sg);
                sb = ClampByte(db - sb);
            }

            const int destContribution =
                (destAlpha * (255 - alpha) + 127) / 255;
            const int outAlpha = alpha + destContribution;
            if (outAlpha <= 0) {
                aDst->SetPixelRgba(dx, dy, 0, 0, 0, 0);
                continue;
            }
            aDst->SetPixelRgba(dx, dy,
                (sr * alpha + dr * destContribution + outAlpha / 2) / outAlpha,
                (sg * alpha + dg * destContribution + outAlpha / 2) / outAlpha,
                (sb * alpha + db * destContribution + outAlpha / 2) / outAlpha,
                outAlpha);
        }
    }
}

/* RGB(0-255) <-> HSL(h 0-360, s/l 0-1), plain textbook conversion. */
void RgbToHsl(int aRed, int aGreen, int aBlue, double* aH, double* aS, double* aL)
{
    double r = aRed / 255.0, g = aGreen / 255.0, b = aBlue / 255.0;
    double max = r > g ? (r > b ? r : b) : (g > b ? g : b);
    double min = r < g ? (r < b ? r : b) : (g < b ? g : b);
    *aL = (max + min) / 2.0;
    if (max == min) {
        *aH = 0.0;
        *aS = 0.0;
        return;
    }
    double d = max - min;
    *aS = *aL > 0.5 ? d / (2.0 - max - min) : d / (max + min);
    if (max == r)
        *aH = (g - b) / d + (g < b ? 6.0 : 0.0);
    else if (max == g)
        *aH = (b - r) / d + 2.0;
    else
        *aH = (r - g) / d + 4.0;
    *aH *= 60.0;
}

double HueToRgb(double aV1, double aV2, double aVh)
{
    if (aVh < 0.0) aVh += 1.0;
    if (aVh > 1.0) aVh -= 1.0;
    if (6.0 * aVh < 1.0) return aV1 + (aV2 - aV1) * 6.0 * aVh;
    if (2.0 * aVh < 1.0) return aV2;
    if (3.0 * aVh < 2.0) return aV1 + (aV2 - aV1) * (2.0 / 3.0 - aVh) * 6.0;
    return aV1;
}

void HslToRgb(double aH, double aS, double aL, int* aRed, int* aGreen, int* aBlue)
{
    if (aS == 0.0) {
        *aRed = *aGreen = *aBlue = static_cast<int>(aL * 255.0 + 0.5);
        return;
    }
    double v2 = aL < 0.5 ? aL * (1.0 + aS) : (aL + aS - aL * aS);
    double v1 = 2.0 * aL - v2;
    double h = aH / 360.0;
    *aRed = ClampByte(static_cast<int>(HueToRgb(v1, v2, h + 1.0 / 3.0) * 255.0 + 0.5));
    *aGreen = ClampByte(static_cast<int>(HueToRgb(v1, v2, h) * 255.0 + 0.5));
    *aBlue = ClampByte(static_cast<int>(HueToRgb(v1, v2, h - 1.0 / 3.0) * 255.0 + 0.5));
}

} // namespace

RgssBitmapCore::RgssBitmapCore()
    : data(0), ownsData(false), width(0), height(0), strideShorts(0), image(0),
      alphaMask(0)
{
}

RgssBitmapCore::~RgssBitmapCore()
{
    Destroy();
}

void RgssBitmapCore::Destroy()
{
    if (image) {
        pixman_image_unref(static_cast<pixman_image_t*>(image));
        image = 0;
    }
    if (data && ownsData) {
        free(data);
    }
    data = 0;
    ownsData = false;
    if (alphaMask) {
        free(alphaMask);
        alphaMask = 0;
    }
    width = height = strideShorts = 0;
}

bool RgssBitmapCore::MakeImage()
{
    if (image)
        pixman_image_unref(static_cast<pixman_image_t*>(image));
    image = 0;
    if (!data || width <= 0 || height <= 0)
        return false;
    int strideBytes = (width * 2 + 3) & ~3; /* pixman wants a 4-byte multiple */
    strideShorts = strideBytes / 2;
    image = pixman_image_create_bits_no_clear(PIXMAN_r5g6b5, width, height,
                                              reinterpret_cast<uint32_t*>(data),
                                              strideBytes);
    return image != 0;
}

bool RgssBitmapCore::EnsureAlphaMask(unsigned char aInitialAlpha)
{
    if (alphaMask)
        return true;
    if (width <= 0 || height <= 0)
        return false;
    alphaMask = static_cast<unsigned char*>(
        malloc(static_cast<size_t>(width) * height));
    if (!alphaMask)
        return false;
    memset(alphaMask, aInitialAlpha, static_cast<size_t>(width) * height);
    return true;
}

bool RgssBitmapCore::Create(int aWidth, int aHeight)
{
    Destroy();
    if (aWidth <= 0 || aHeight <= 0)
        return false;
    int strideBytes = (aWidth * 2 + 3) & ~3;
    unsigned short* buffer =
        static_cast<unsigned short*>(malloc(static_cast<size_t>(strideBytes) * aHeight));
    if (!buffer)
        return false;
    memset(buffer, 0, static_cast<size_t>(strideBytes) * aHeight);
    data = buffer;
    ownsData = true;
    width = aWidth;
    height = aHeight;
    if (!MakeImage()) {
        Destroy();
        return false;
    }
    return true;
}

bool RgssBitmapCore::CreateTransparent(int aWidth, int aHeight)
{
    if (!Create(aWidth, aHeight))
        return false;
    if (!EnsureAlphaMask(0)) {
        Destroy();
        return false;
    }
    return true;
}

bool RgssBitmapCore::SetPixels(int aWidth, int aHeight, unsigned short* aBuffer,
                               bool aTakeOwnership)
{
    Destroy();
    if (aWidth <= 0 || aHeight <= 0 || !aBuffer)
        return false;
    data = aBuffer;
    ownsData = aTakeOwnership;
    width = aWidth;
    height = aHeight;
    if (!MakeImage()) {
        Destroy();
        return false;
    }
    return true;
}

unsigned short RgssBitmapCore::RgbTo565(int aRed, int aGreen, int aBlue)
{
    return Pack565(aRed, aGreen, aBlue);
}

void RgssBitmapCore::From565(unsigned short aPixel, int* aRed, int* aGreen, int* aBlue)
{
    Expand565(aPixel, aRed, aGreen, aBlue);
}

bool RgssBitmapCore::CreateFromRgba8(int aWidth, int aHeight,
                                     const unsigned char* aRgba)
{
    Destroy();
    if (aWidth <= 0 || aHeight <= 0 || !aRgba)
        return false;
    if (!Create(aWidth, aHeight))
        return false;
    unsigned char* mask =
        static_cast<unsigned char*>(malloc(static_cast<size_t>(width) * height));
    if (!mask) {
        Destroy();
        return false;
    }
    bool anyAlpha = false;
    for (int y = 0; y < height; ++y) {
        const unsigned char* src = aRgba + static_cast<size_t>(y) * width * 4;
        unsigned short* row = data + y * strideShorts;
        unsigned char* maskRow = mask + static_cast<size_t>(y) * width;
        for (int x = 0; x < width; ++x) {
            row[x] = Pack565(src[x * 4], src[x * 4 + 1], src[x * 4 + 2]);
            maskRow[x] = src[x * 4 + 3];
            if (src[x * 4 + 3] != 255)
                anyAlpha = true;
        }
    }
    if (!anyAlpha) {
        /* Fully opaque: drop the mask so blits keep the fast paths. */
        free(mask);
        return true;
    }
    alphaMask = mask;
    return true;
}

unsigned char RgssBitmapCore::AlphaAt(int aX, int aY) const
{
    if (!IsValid() || aX < 0 || aY < 0 || aX >= width || aY >= height || !alphaMask)
        return 255;
    return alphaMask[aY * width + aX];
}

void RgssBitmapCore::Clear()
{
    if (data)
        memset(data, 0, static_cast<size_t>(strideShorts) * height * 2);
    if (alphaMask)
        memset(alphaMask, 0, static_cast<size_t>(width) * height);
}

void RgssBitmapCore::ClearRect(const Rect& aRect)
{
    if (!IsValid() || aRect.IsEmpty())
        return;
    int x1 = aRect.x < 0 ? 0 : aRect.x;
    int y1 = aRect.y < 0 ? 0 : aRect.y;
    int x2 = aRect.x + aRect.width > width ? width : aRect.x + aRect.width;
    int y2 = aRect.y + aRect.height > height ? height : aRect.y + aRect.height;
    if (x1 >= x2 || y1 >= y2)
        return;
    for (int y = y1; y < y2; ++y) {
        memset(data + y * strideShorts + x1, 0,
               static_cast<size_t>(x2 - x1) * 2);
        if (alphaMask)
            memset(alphaMask + static_cast<size_t>(y) * width + x1, 0,
                   static_cast<size_t>(x2 - x1));
    }
}

void RgssBitmapCore::ClearTransparent()
{
    if (!IsValid())
        return;
    if (!EnsureAlphaMask(255))
        return;
    memset(data, 0, static_cast<size_t>(strideShorts) * height * 2);
    memset(alphaMask, 0, static_cast<size_t>(width) * height);
}

void RgssBitmapCore::ClearRectTransparent(const Rect& aRect)
{
    if (!IsValid() || aRect.IsEmpty())
        return;
    if (!EnsureAlphaMask(255))
        return;
    ClearRect(aRect);
}

/* Source-over composite of a constant alpha into the A8 mask; the mask is
 * allocated (fully opaque) on first blended write. */
void RgssBitmapCore::BlendMaskRect(const Rect& aRect, int aAlpha)
{
    /* Clip first: pixman clips the color fill itself, the mask must match. */
    Rect r(aRect);
    if (r.x < 0) { r.width += r.x; r.x = 0; }
    if (r.y < 0) { r.height += r.y; r.y = 0; }
    if (r.x + r.width > width) r.width = width - r.x;
    if (r.y + r.height > height) r.height = height - r.y;
    if (r.IsEmpty())
        return;
    if (aAlpha >= 255) {
        if (alphaMask)
            for (int y = r.y; y < r.y + r.height; ++y)
                memset(alphaMask + static_cast<size_t>(y) * width + r.x,
                       255, static_cast<size_t>(r.width));
        return;
    }
    if (!alphaMask) {
        alphaMask =
            static_cast<unsigned char*>(malloc(static_cast<size_t>(width) * height));
        if (!alphaMask)
            return;
        memset(alphaMask, 255, static_cast<size_t>(width) * height);
    }
    for (int y = r.y; y < r.y + r.height; ++y) {
        unsigned char* row = alphaMask + static_cast<size_t>(y) * width + r.x;
        for (int x = 0; x < r.width; ++x) {
            const int m = row[x];
            row[x] = static_cast<unsigned char>((aAlpha * 255 + m * (255 - aAlpha) + 127) / 255);
        }
    }
}

void RgssBitmapCore::FillRect(const Rect& aRect, int aRed, int aGreen, int aBlue,
                              int aAlpha)
{
    if (!IsValid() || aRect.IsEmpty() || aAlpha <= 0)
        return;
    if (aAlpha > 255)
        aAlpha = 255;

    const int sr = ClampByte(aRed);
    const int sg = ClampByte(aGreen);
    const int sb = ClampByte(aBlue);

    if (aAlpha == 255) {
        BlendMaskRect(aRect, 255);
        pixman_color_t color;
        color.red = static_cast<uint16_t>(sr * 257);
        color.green = static_cast<uint16_t>(sg * 257);
        color.blue = static_cast<uint16_t>(sb * 257);
        color.alpha = 0xffff;
        pixman_rectangle16_t box;
        box.x = static_cast<int16_t>(aRect.x);
        box.y = static_cast<int16_t>(aRect.y);
        box.width = static_cast<uint16_t>(aRect.width);
        box.height = static_cast<uint16_t>(aRect.height);
        pixman_image_fill_rectangles(PIXMAN_OP_SRC,
                                     static_cast<pixman_image_t*>(image),
                                     &color, 1, &box);
        return;
    }

    Rect r(aRect);
    if (r.x < 0) { r.width += r.x; r.x = 0; }
    if (r.y < 0) { r.height += r.y; r.y = 0; }
    if (r.x + r.width > width) r.width = width - r.x;
    if (r.y + r.height > height) r.height = height - r.y;
    if (r.IsEmpty())
        return;

    for (int y = r.y; y < r.y + r.height; ++y) {
        for (int x = r.x; x < r.x + r.width; ++x) {
            int dr, dg, db;
            GetPixel(x, y, &dr, &dg, &db);
            const int destAlpha = AlphaAt(x, y);
            const int destContribution =
                (destAlpha * (255 - aAlpha) + 127) / 255;
            const int outAlpha = aAlpha + destContribution;
            if (outAlpha <= 0) {
                SetPixelRgba(x, y, 0, 0, 0, 0);
                continue;
            }
            SetPixelRgba(x, y,
                (sr * aAlpha + dr * destContribution + outAlpha / 2) / outAlpha,
                (sg * aAlpha + dg * destContribution + outAlpha / 2) / outAlpha,
                (sb * aAlpha + db * destContribution + outAlpha / 2) / outAlpha,
                outAlpha);
        }
    }
}

bool RgssBitmapCore::GetPixel(int aX, int aY, int* aRed, int* aGreen, int* aBlue) const
{
    if (!IsValid() || aX < 0 || aY < 0 || aX >= width || aY >= height)
        return false;
    Expand565(data[aY * strideShorts + aX], aRed, aGreen, aBlue);
    return true;
}

void RgssBitmapCore::SetPixel(int aX, int aY, int aRed, int aGreen, int aBlue)
{
    SetPixelRgba(aX, aY, aRed, aGreen, aBlue, 255);
}

void RgssBitmapCore::SetPixelRgba(int aX, int aY, int aRed, int aGreen,
                                  int aBlue, int aAlpha)
{
    if (!IsValid() || aX < 0 || aY < 0 || aX >= width || aY >= height)
        return;
    const int alpha = ClampByte(aAlpha);
    data[aY * strideShorts + aX] = Pack565(aRed, aGreen, aBlue);
    if (alpha < 255) {
        if (!EnsureAlphaMask(255))
            return;
        alphaMask[aY * width + aX] = static_cast<unsigned char>(alpha);
    } else if (alphaMask) {
        alphaMask[aY * width + aX] = 255;
    }
}

void RgssBitmapCore::Blit(RgssBitmapCore* aSrc, const Rect& aSrcRect, int aDstX,
                          int aDstY, int aOpacity)
{
    if (!IsValid() || !aSrc || !aSrc->IsValid() || aSrcRect.IsEmpty())
        return;
    if (aOpacity <= 0)
        return;
    if (aOpacity > 255) aOpacity = 255;
    /* Any alpha-bearing source or destination needs the straight-alpha
     * loop so the destination mask stays in sync with RGB. */
    if (aSrc->alphaMask || alphaMask) {
        CompositeLoop(this, aSrc, aSrcRect, aDstX, aDstY,
                      aSrcRect.width, aSrcRect.height, aOpacity, false,
                      KBlendNormal, aSrc->alphaMask);
        return;
    }
    pixman_image_t* src = static_cast<pixman_image_t*>(aSrc->image);
    pixman_image_t* dst = static_cast<pixman_image_t*>(image);
    if (aOpacity == 255) {
        pixman_image_composite(PIXMAN_OP_SRC, src, 0, dst,
                               aSrcRect.x, aSrcRect.y, 0, 0,
                               aDstX, aDstY, aSrcRect.width, aSrcRect.height);
        return;
    }
    pixman_color_t maskColor;
    maskColor.red = maskColor.green = maskColor.blue = 0;
    maskColor.alpha = static_cast<uint16_t>(aOpacity * 257);
    pixman_image_t* mask = pixman_image_create_solid_fill(&maskColor);
    if (!mask)
        return;
    pixman_image_composite(PIXMAN_OP_OVER, src, mask, dst,
                           aSrcRect.x, aSrcRect.y, 0, 0,
                           aDstX, aDstY, aSrcRect.width, aSrcRect.height);
    pixman_image_unref(mask);
}

void RgssBitmapCore::BltAdditive(RgssBitmapCore* aSrc, const Rect& aSrcRect,
                                 int aDstX, int aDstY, int aOpacity)
{
    if (!IsValid() || !aSrc || !aSrc->IsValid() || aSrcRect.IsEmpty())
        return;
    if (aOpacity <= 0)
        return;
    if (aOpacity > 255) aOpacity = 255;
    CompositeLoop(this, aSrc, aSrcRect, aDstX, aDstY,
                  aSrcRect.width, aSrcRect.height, aOpacity, false,
                  KBlendAdditive, aSrc->alphaMask);
}

void RgssBitmapCore::BltSubtractive(RgssBitmapCore* aSrc, const Rect& aSrcRect,
                                    int aDstX, int aDstY, int aOpacity)
{
    if (!IsValid() || !aSrc || !aSrc->IsValid() || aSrcRect.IsEmpty())
        return;
    if (aOpacity <= 0)
        return;
    if (aOpacity > 255) aOpacity = 255;
    CompositeLoop(this, aSrc, aSrcRect, aDstX, aDstY,
                  aSrcRect.width, aSrcRect.height, aOpacity, false,
                  KBlendSubtractive, aSrc->alphaMask);
}

void RgssBitmapCore::StretchBlt(RgssBitmapCore* aSrc, const Rect& aSrcRect,
                                int aDstX, int aDstY, double aScaleX,
                                double aScaleY, int aOpacity, bool aMirror,
                                int aBlendType)
{
    if (!IsValid() || !aSrc || !aSrc->IsValid() || aSrcRect.IsEmpty())
        return;
    if (aOpacity <= 0)
        return;
    if (aOpacity > 255) aOpacity = 255;
    if (aScaleX <= 0.0 || aScaleY <= 0.0)
        return;
    int dstW = static_cast<int>(aSrcRect.width * aScaleX + 0.5);
    int dstH = static_cast<int>(aSrcRect.height * aScaleY + 0.5);
    if (dstW <= 0 || dstH <= 0)
        return;

    /* Alpha-bearing source or destination forces the manual loop so the
     * destination A8 mask is composited together with RGB. */
    if (aSrc->alphaMask || alphaMask) {
        CompositeLoop(this, aSrc, aSrcRect, aDstX, aDstY, dstW, dstH,
                      aOpacity, aMirror, aBlendType, aSrc->alphaMask);
        return;
    }

    if (!aMirror && aBlendType == KBlendNormal) {
        /* SLOW PATH: pixman transform (nearest-neighbor) composited OVER. */
        pixman_image_t* src = static_cast<pixman_image_t*>(aSrc->image);
        pixman_transform_t transform;
        double kx = static_cast<double>(aSrcRect.width) / dstW;
        double ky = static_cast<double>(aSrcRect.height) / dstH;
        pixman_transform_init_scale(&transform,
                                    pixman_double_to_fixed(kx),
                                    pixman_double_to_fixed(ky));
        pixman_transform_translate(&transform, 0,
                                   pixman_int_to_fixed(aSrcRect.x),
                                   pixman_int_to_fixed(aSrcRect.y));
        pixman_image_set_filter(src, PIXMAN_FILTER_NEAREST, 0, 0);
        pixman_image_set_transform(src, &transform);
        pixman_image_t* mask = 0;
        pixman_color_t maskColor;
        maskColor.red = maskColor.green = maskColor.blue = 0;
        maskColor.alpha = static_cast<uint16_t>(aOpacity * 257);
        if (aOpacity < 255) {
            mask = pixman_image_create_solid_fill(&maskColor);
            if (!mask) {
                pixman_image_set_transform(src, 0);
                return;
            }
        }
        pixman_image_composite(PIXMAN_OP_OVER, src, mask,
                               static_cast<pixman_image_t*>(image),
                               0, 0, 0, 0, aDstX, aDstY, dstW, dstH);
        pixman_image_set_transform(src, 0);
        if (mask)
            pixman_image_unref(mask);
        return;
    }

    CompositeLoop(this, aSrc, aSrcRect, aDstX, aDstY, dstW, dstH,
                  aOpacity, aMirror, aBlendType, aSrc->alphaMask);
}

void RgssBitmapCore::HueChange(int aDegrees)
{
    if (!IsValid())
        return;
    for (int y = 0; y < height; ++y) {
        unsigned short* row = data + y * strideShorts;
        for (int x = 0; x < width; ++x) {
            int r, g, b;
            Expand565(row[x], &r, &g, &b);
            double h, s, l;
            RgbToHsl(r, g, b, &h, &s, &l);
            h += aDegrees;
            while (h < 0.0) h += 360.0;
            while (h >= 360.0) h -= 360.0;
            HslToRgb(h, s, l, &r, &g, &b);
            row[x] = Pack565(r, g, b);
        }
    }
}

void RgssBitmapCore::MultiplyBrightness(int aLevel)
{
    if (!IsValid())
        return;
    if (aLevel < 0) aLevel = 0;
    if (aLevel > 255) aLevel = 255;
    for (int i = 0; i < strideShorts * height; ++i) {
        int r, g, b;
        Expand565(data[i], &r, &g, &b);
        data[i] = Pack565((r * aLevel + 127) / 255,
                          (g * aLevel + 127) / 255,
                          (b * aLevel + 127) / 255);
    }
}

} // namespace rgss
