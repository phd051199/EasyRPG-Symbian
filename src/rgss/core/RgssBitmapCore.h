#ifndef RGSS_BITMAP_CORE_H
#define RGSS_BITMAP_CORE_H

/*
 * RGB565 bitmap primitive for the RGSS renderer core. Wraps a malloc'd
 * (or external) pixel buffer plus a pixman_image_t. Pixman is only touched
 * from the .cpp so neither this header nor the binding needs pixman.h.
 * No exceptions, no RTTI; allocation failures leave the bitmap invalid.
 *
 * Alpha: RGB565 has no alpha channel, but RGSS sprites need per-pixel
 * transparency from PNGs. Every bitmap therefore carries an optional A8
 * alpha mask - a separate malloc'd buffer with one byte per pixel, stored
 * alongside the RGB565 pixels. A null mask means fully opaque everywhere.
 * Image decoders (ReadXYZ/ReadBMP/ReadPNGBytes in src/core/Image.cpp)
 * output RGBA8888 buffers; CreateFromRgba8 converts those into the RGB565
 * pixels plus the A8 mask in one pass and drops the mask again when every
 * pixel is opaque, so fully opaque bitmaps keep the fast blit paths.
 */

#include <stddef.h>

#include "RgssTypes.h"

namespace rgss {

/* Sprite#blend_type values (RGSS1). */
const int KBlendNormal = 0;
const int KBlendAdditive = 1;
const int KBlendSubtractive = 2;

class RgssBitmapCore
{
public:
    RgssBitmapCore();
    ~RgssBitmapCore();

    /* Zero-filled buffer owned by the bitmap (opaque black, no mask). */
    bool Create(int aWidth, int aHeight);
    /* RGSS Bitmap.new semantics: zero RGB with a fully transparent A8 mask. */
    bool CreateTransparent(int aWidth, int aHeight);
    /* Wraps an external RGB565 buffer; ownership only affects deletion.
     * The wrapped bitmap is treated as fully opaque (no alpha mask). */
    bool SetPixels(int aWidth, int aHeight, unsigned short* aBuffer,
                   bool aTakeOwnership);
    /*
     * Fills the RGB565 pixels and the A8 alpha mask from an RGBA8888
     * buffer (4 bytes per pixel, row-major; the input stays owned by the
     * caller - the decoders free their output with free()). When every
     * alpha byte is 255 the mask is dropped and the bitmap is opaque.
     */
    bool CreateFromRgba8(int aWidth, int aHeight, const unsigned char* aRgba);

    bool IsValid() const { return image != 0 && width > 0 && height > 0; }
    int Width() const { return width; }
    int Height() const { return height; }
    /* Row stride in unsigned short units (4-byte aligned for pixman, so it
     * can exceed Width() on odd widths). */
    int StrideShorts() const { return strideShorts; }
    unsigned short* Pixels() { return data; }
    const unsigned short* Pixels() const { return data; }
    /* Null when the bitmap is fully opaque. */
    const unsigned char* AlphaMask() const { return alphaMask; }
    bool IsOpaque() const { return alphaMask == 0; }
    /* 255 when there is no alpha mask. */
    unsigned char AlphaAt(int aX, int aY) const;

    /* Channel helpers (0-255 in and out; 5/6/5 bits are bit-replicated). */
    static unsigned short RgbTo565(int aRed, int aGreen, int aBlue);
    static void From565(unsigned short aPixel, int* aRed, int* aGreen, int* aBlue);

    /* Back-buffer clear: zero RGB and clear an existing alpha mask. */
    void Clear();
    void ClearRect(const Rect& aRect);
    /* RGSS Bitmap#clear variants: ensure transparency even for an opaque
     * source image that did not previously need an alpha mask. */
    void ClearTransparent();
    void ClearRectTransparent(const Rect& aRect);
    /* aAlpha < 255 blends over the destination pixels and composites the
     * alpha into the mask source-over style (mask allocated on demand). */
    void FillRect(const Rect& aRect, int aRed, int aGreen, int aBlue, int aAlpha);
    bool GetPixel(int aX, int aY, int* aRed, int* aGreen, int* aBlue) const;
    void SetPixel(int aX, int aY, int aRed, int aGreen, int aBlue);
    /* Exact RGBA replacement used by Bitmap#set_pixel. */
    void SetPixelRgba(int aX, int aY, int aRed, int aGreen, int aBlue,
                      int aAlpha);

    /* 1:1 blit; aOpacity 0-255 scales the source over the destination.
     * Per-pixel source alpha (A8 mask) is combined with aOpacity:
     * effective = (mask * opacity + 127) / 255. Fully opaque source and
     * aOpacity == 255 take the pixman OP_SRC fast path. */
    void Blit(RgssBitmapCore* aSrc, const Rect& aSrcRect, int aDstX, int aDstY,
              int aOpacity);
    /* Scaled blit (aScaleX/aScaleY == 1.0 takes the pixman 1:1 fast path,
     * otherwise a pixman transform; SLOW PATH). aMirror flips horizontally,
     * aBlendType selects normal/additive/subtractive (add/sub are manual
     * loops - SLOW PATH). A source alpha mask forces the manual loop so the
     * per-pixel alpha combination stays exact. */
    void StretchBlt(RgssBitmapCore* aSrc, const Rect& aSrcRect, int aDstX,
                    int aDstY, double aScaleX, double aScaleY, int aOpacity,
                    bool aMirror, int aBlendType);
    /* In-place hue rotation by aDegrees; correctness over speed. */
    void HueChange(int aDegrees);
    /* SLOW PATH manual loops for blend types 1/2 (1:1 blit). */
    void BltAdditive(RgssBitmapCore* aSrc, const Rect& aSrcRect, int aDstX,
                     int aDstY, int aOpacity);
    void BltSubtractive(RgssBitmapCore* aSrc, const Rect& aSrcRect, int aDstX,
                        int aDstY, int aOpacity);
    /* Per-channel multiply, 0-255 (frame fade to black/white). */
    void MultiplyBrightness(int aLevel);

private:
    RgssBitmapCore(const RgssBitmapCore&);
    RgssBitmapCore& operator=(const RgssBitmapCore&);

    void Destroy();
    bool MakeImage();
    bool EnsureAlphaMask(unsigned char aInitialAlpha);
    void BlendMaskRect(const Rect& aRect, int aAlpha);

    unsigned short* data;
    bool ownsData;
    int width;
    int height;
    int strideShorts;    /* in unsigned short units */
    void* image;         /* pixman_image_t*, opaque here */
    unsigned char* alphaMask; /* A8, one byte per pixel, owned; null = opaque */
};

/*
 * Decoding (PNG/BMP/...) lives in the binding layer (VFS + host decode);
 * the core stays pixel-only. Implemented by the binding, not in core.
 * Returns a heap RgssBitmapCore (caller deletes) or null.
 */
RgssBitmapCore* RgssDecodeImageToRgb565(const unsigned char* aData, long aSize);

} // namespace rgss

#endif
