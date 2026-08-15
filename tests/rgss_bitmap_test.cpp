/*
 * Host golden-pixel tests for the M-R2 renderer core bitmap primitive
 * (src/rgss/core/RgssBitmapCore.*). No Ruby/SDL/Symbian headers.
 *
 * Golden values are exact for the manual blend loops (per-pixel alpha
 * mask, additive/subtractive): channel = (src * a + dst * (255 - a) + 127) / 255
 * with a = (mask * opacity + 127) / 255, channels read back through the
 * bit-replicated 5/6/5 expansion. The pixman-composited paths (constant
 * opacity without a mask) are checked with a +/-1 tolerance because
 * pixman's rounding belongs to pixman, not to this core.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rgss/core/RgssBitmapCore.h"

using rgss::Rect;
using rgss::RgssBitmapCore;

namespace {

void CheckPixel(const RgssBitmapCore& aBitmap, int aX, int aY,
                int aRed, int aGreen, int aBlue, const char* aWhat)
{
    int r = -1, g = -1, b = -1;
    assert(aBitmap.GetPixel(aX, aY, &r, &g, &b));
    if (r != aRed || g != aGreen || b != aBlue)
        printf("FAIL %s at (%d,%d): got (%d,%d,%d) want (%d,%d,%d)\n",
               aWhat, aX, aY, r, g, b, aRed, aGreen, aBlue);
    assert(r == aRed && g == aGreen && b == aBlue);
}

/* Tolerance check for the pixman-composited paths: pixman's r5g6b5 fast
 * paths blend in reduced precision, so allow +/-2 per channel. */
void CheckPixelNear(const RgssBitmapCore& aBitmap, int aX, int aY,
                    int aRed, int aGreen, int aBlue, const char* aWhat)
{
    int r = -1, g = -1, b = -1;
    assert(aBitmap.GetPixel(aX, aY, &r, &g, &b));
    const int dr = r - aRed, dg = g - aGreen, db = b - aBlue;
    if (dr < -2 || dr > 2 || dg < -2 || dg > 2 || db < -2 || db > 2)
        printf("FAIL %s at (%d,%d): got (%d,%d,%d) want ~(%d,%d,%d)\n",
               aWhat, aX, aY, r, g, b, aRed, aGreen, aBlue);
    assert(dr >= -2 && dr <= 2 && dg >= -2 && dg <= 2 && db >= -2 && db <= 2);
}

/* Solid opaque bitmap filled with one color. */
void MakeSolid(RgssBitmapCore* aBitmap, int aWidth, int aHeight,
               int aRed, int aGreen, int aBlue)
{
    assert(aBitmap->Create(aWidth, aHeight));
    aBitmap->FillRect(Rect(0, 0, aWidth, aHeight), aRed, aGreen, aBlue, 255);
}

/*
 * A fully opaque RGBA8888 buffer must produce a mask-less bitmap: the
 * decoders (ReadXYZ/ReadBMP/ReadPNGBytes) emit exactly this shape when a
 * PNG has no transparency.
 */
void TestCreateFromRgba8OpaqueDropsMask()
{
    const unsigned char rgba[4 * 4] = {
        255, 0,   0,   255,
        0,   255, 0,   255,
        0,   0,   255, 255,
        255, 255, 255, 255
    };
    RgssBitmapCore bitmap;
    assert(bitmap.CreateFromRgba8(2, 2, rgba));
    assert(bitmap.IsOpaque());
    assert(bitmap.AlphaMask() == 0);
    CheckPixel(bitmap, 0, 0, 255, 0, 0, "rgba8 opaque");
    CheckPixel(bitmap, 1, 0, 0, 255, 0, "rgba8 opaque");
    CheckPixel(bitmap, 0, 1, 0, 0, 255, "rgba8 opaque");
    CheckPixel(bitmap, 1, 1, 255, 255, 255, "rgba8 opaque");
}

void TestCreateFromRgba8KeepsAlphaMask()
{
    const unsigned char rgba[2 * 4] = {
        255, 0, 0, 255,
        0, 255, 255, 128
    };
    RgssBitmapCore bitmap;
    assert(bitmap.CreateFromRgba8(2, 1, rgba));
    assert(!bitmap.IsOpaque());
    assert(bitmap.AlphaMask() != 0);
    assert(bitmap.AlphaAt(0, 0) == 255);
    assert(bitmap.AlphaAt(1, 0) == 128);
    CheckPixel(bitmap, 0, 0, 255, 0, 0, "rgba8 masked");
    CheckPixel(bitmap, 1, 0, 0, 255, 255, "rgba8 masked");
}

void TestOpaqueBlitGolden()
{
    RgssBitmapCore dst;
    MakeSolid(&dst, 4, 4, 0, 0, 255);
    RgssBitmapCore src;
    MakeSolid(&src, 2, 2, 255, 0, 0);
    dst.Blit(&src, Rect(0, 0, 2, 2), 1, 1, 255);
    CheckPixel(dst, 0, 0, 0, 0, 255, "opaque blit untouched");
    CheckPixel(dst, 1, 1, 255, 0, 0, "opaque blit interior");
    CheckPixel(dst, 2, 2, 255, 0, 0, "opaque blit interior");
    CheckPixel(dst, 3, 3, 0, 0, 255, "opaque blit untouched");

    /* Clipping on all four edges. */
    MakeSolid(&dst, 4, 4, 0, 0, 255);
    dst.Blit(&src, Rect(0, 0, 2, 2), -1, -1, 255);
    CheckPixel(dst, 0, 0, 255, 0, 0, "opaque blit clip top-left");
    CheckPixel(dst, 3, 3, 0, 0, 255, "opaque blit clip bottom-right");
    CheckPixel(dst, 0, 0, 255, 0, 0, "opaque blit clip maps src(1,1)");

    MakeSolid(&dst, 4, 4, 0, 0, 255);
    dst.Blit(&src, Rect(0, 0, 2, 2), 3, 3, 255);
    CheckPixel(dst, 3, 3, 255, 0, 0, "opaque blit clip bottom-right single");
    CheckPixel(dst, 2, 3, 0, 0, 255, "opaque blit clip neighbour");
}

/* Pixman path (no mask, constant opacity): tolerance +/-1 per channel. */
void TestConstantOpacityBlit()
{
    RgssBitmapCore dst;
    MakeSolid(&dst, 2, 1, 255, 255, 255);
    RgssBitmapCore src;
    MakeSolid(&src, 2, 1, 255, 0, 0);
    dst.Blit(&src, Rect(0, 0, 2, 1), 0, 0, 128);
    /* Expected = manual blend formula per channel width: r 31 -> 255;
     * g (0*128 + 63*127 + 127)/255 = 31 -> 125;
     * b (0*128 + 31*127 + 127)/255 = 15 -> 123. */
    CheckPixelNear(dst, 0, 0, 255, 125, 123, "constant opacity blend");
    CheckPixelNear(dst, 1, 0, 255, 125, 123, "constant opacity blend");
}

/*
 * The per-pixel alpha golden test (required): a masked source blitted
 * over white with full and reduced opacity, plus a src-rect offset and a
 * fully transparent source pixel.
 */
void TestPerPixelAlphaBlendGolden()
{
    /* 2x1: opaque red, half-transparent red. */
    const unsigned char rgba[2 * 4] = {
        255, 0, 0, 255,
        255, 0, 0, 128
    };
    RgssBitmapCore src;
    assert(src.CreateFromRgba8(2, 1, rgba));
    assert(src.AlphaMask() != 0);

    /* Opacity 255: px0 is pure red; px1 blends with a = (128*255+127)/255 = 128:
     * g/b = (0*128 + 255*127 + 127)/255 = 127, which reads back through the
     * 6/5-bit quantization as 125/123. */
    RgssBitmapCore dst;
    MakeSolid(&dst, 2, 1, 255, 255, 255);
    dst.Blit(&src, Rect(0, 0, 2, 1), 0, 0, 255);
    CheckPixel(dst, 0, 0, 255, 0, 0, "per-pixel alpha fully opaque px");
    CheckPixel(dst, 1, 0, 255, 125, 123, "per-pixel alpha a=128 over white");

    /* Opacity combines with the mask: px0 a = (255*128+127)/255 = 128,
     * px1 a = (128*128+127)/255 = 64. */
    MakeSolid(&dst, 2, 1, 255, 255, 255);
    dst.Blit(&src, Rect(0, 0, 2, 1), 0, 0, 128);
    CheckPixel(dst, 0, 0, 255, 125, 123, "per-pixel alpha combine a=128 px0");
    CheckPixel(dst, 1, 0, 255, 190, 189, "per-pixel alpha combine a=64 px1");

    /* Source rectangle selects only the masked pixel. */
    MakeSolid(&dst, 2, 1, 255, 255, 255);
    dst.Blit(&src, Rect(1, 0, 1, 1), 0, 0, 255);
    CheckPixel(dst, 0, 0, 255, 125, 123, "per-pixel alpha src rect offset");
    CheckPixel(dst, 1, 0, 255, 255, 255, "per-pixel alpha src rect rest");

    /* A fully transparent source pixel leaves the destination alone. */
    const unsigned char transparent[1 * 4] = {255, 0, 0, 0};
    RgssBitmapCore hole;
    assert(hole.CreateFromRgba8(1, 1, transparent));
    assert(hole.AlphaMask() != 0);
    MakeSolid(&dst, 2, 1, 255, 255, 255);
    dst.Blit(&hole, Rect(0, 0, 1, 1), 1, 0, 255);
    CheckPixel(dst, 1, 0, 255, 255, 255, "per-pixel alpha zero leaves dst");
}

/* Odd widths make the pixman row stride (4-byte aligned) exceed Width();
 * the manual masked path must use the stride, not the width. */
void TestOddWidthMaskedBlitGolden()
{
    /* 3x2 source: column alphas 255/128/0 over two rows. */
    unsigned char rgba[3 * 2 * 4];
    for (int i = 0; i < 3 * 2; ++i) {
        rgba[i * 4] = 255;
        rgba[i * 4 + 1] = 0;
        rgba[i * 4 + 2] = 0;
        rgba[i * 4 + 3] = static_cast<unsigned char>(i % 3 == 0 ? 255 : (i % 3 == 1 ? 128 : 0));
    }
    RgssBitmapCore src;
    assert(src.CreateFromRgba8(3, 2, rgba));
    assert(src.AlphaMask() != 0);
    assert(src.StrideShorts() == 4); /* 3 shorts padded to 4 */

    RgssBitmapCore dst;
    MakeSolid(&dst, 3, 2, 255, 255, 255);
    dst.Blit(&src, Rect(0, 0, 3, 2), 0, 0, 255);
    for (int y = 0; y < 2; ++y) {
        CheckPixel(dst, 0, y, 255, 0, 0, "odd-width masked blit a=255");
        CheckPixel(dst, 1, y, 255, 125, 123, "odd-width masked blit a=128");
        CheckPixel(dst, 2, y, 255, 255, 255, "odd-width masked blit a=0");
    }
}

void TestMaskedStretchBlitGolden()
{
    /* 2x2 uniform red with alpha 128, stretched x2 over white: every
     * destination pixel blends with a = (128*255+127)/255 = 128. */
    unsigned char rgba[2 * 2 * 4];
    for (int i = 0; i < 2 * 2; ++i) {
        rgba[i * 4] = 255;
        rgba[i * 4 + 1] = 0;
        rgba[i * 4 + 2] = 0;
        rgba[i * 4 + 3] = 128;
    }
    RgssBitmapCore src;
    assert(src.CreateFromRgba8(2, 2, rgba));
    RgssBitmapCore dst;
    MakeSolid(&dst, 4, 4, 255, 255, 255);
    dst.StretchBlt(&src, Rect(0, 0, 2, 2), 0, 0, 2.0, 2.0, 255, false,
                   rgss::KBlendNormal);
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x)
            CheckPixel(dst, x, y, 255, 125, 123, "masked stretch a=128");
}

void TestFillRectAlphaCompositesMask()
{
    /* Start fully transparent (alpha-0 RGBA, like a decoded PNG that is
     * empty), then watch FillRect composite straight RGB + alpha. */
    const unsigned char rgba[2 * 2 * 4] = {0};
    RgssBitmapCore bitmap;
    assert(bitmap.CreateFromRgba8(2, 2, rgba));
    assert(bitmap.AlphaMask() != 0);
    assert(bitmap.AlphaAt(0, 0) == 0);

    /* a=128 over alpha 0 keeps straight source RGB and stores alpha 128. */
    bitmap.FillRect(Rect(0, 0, 2, 2), 255, 0, 0, 128);
    assert(bitmap.AlphaAt(0, 0) == 128);
    assert(bitmap.AlphaAt(1, 1) == 128);
    CheckPixel(bitmap, 0, 0, 255, 0, 0, "fill a=128 over transparent");

    /* Opaque fill covers transparency again in its rectangle. */
    bitmap.FillRect(Rect(0, 0, 1, 1), 0, 255, 0, 255);
    assert(bitmap.AlphaAt(0, 0) == 255);
    assert(bitmap.AlphaAt(1, 0) == 128);

    /* A second half-transparent fill composites: (128*255 + 128*127 + 127)/255 = 192. */
    bitmap.FillRect(Rect(1, 0, 1, 1), 0, 0, 255, 128);
    assert(bitmap.AlphaAt(1, 0) == 192);

    /* An opaque-start bitmap stays opaque through a blended fill
     * (source-over 128 over 255 is 255). */
    RgssBitmapCore opaque;
    MakeSolid(&opaque, 1, 1, 0, 0, 0);
    opaque.FillRect(Rect(0, 0, 1, 1), 255, 0, 0, 128);
    assert(opaque.AlphaAt(0, 0) == 255);

    /* Existing-mask clear makes the whole bitmap transparent. */
    bitmap.Clear();
    assert(bitmap.AlphaAt(0, 0) == 0);
    assert(bitmap.AlphaAt(1, 1) == 0);
}

void TestRgssTransparentBitmapSemantics()
{
    RgssBitmapCore bitmap;
    assert(bitmap.CreateTransparent(3, 2));
    assert(!bitmap.IsOpaque());
    assert(bitmap.AlphaAt(0, 0) == 0);
    assert(bitmap.AlphaAt(2, 1) == 0);

    bitmap.SetPixelRgba(1, 0, 255, 0, 0, 128);
    assert(bitmap.AlphaAt(1, 0) == 128);
    CheckPixel(bitmap, 1, 0, 255, 0, 0, "set_pixel rgba straight color");

    bitmap.SetPixelRgba(2, 0, 0, 255, 0, 255);
    assert(bitmap.AlphaAt(2, 0) == 255);
    CheckPixel(bitmap, 2, 0, 0, 255, 0, "set_pixel opaque");

    RgssBitmapCore image;
    MakeSolid(&image, 2, 2, 0, 0, 255);
    assert(image.IsOpaque());
    image.ClearRectTransparent(Rect(1, 0, 1, 2));
    assert(!image.IsOpaque());
    assert(image.AlphaAt(0, 0) == 255);
    assert(image.AlphaAt(1, 0) == 0);
    assert(image.AlphaAt(1, 1) == 0);
    image.ClearTransparent();
    assert(image.AlphaAt(0, 0) == 0);
    assert(image.AlphaAt(1, 1) == 0);
}

void TestAddSubBlitGolden()
{
    RgssBitmapCore src;
    MakeSolid(&src, 1, 1, 255, 0, 0);
    RgssBitmapCore dst;
    MakeSolid(&dst, 1, 1, 0, 0, 0);
    dst.BltAdditive(&src, Rect(0, 0, 1, 1), 0, 0, 255);
    CheckPixel(dst, 0, 0, 255, 0, 0, "additive over black");

    MakeSolid(&dst, 1, 1, 255, 255, 255);
    dst.BltSubtractive(&src, Rect(0, 0, 1, 1), 0, 0, 255);
    CheckPixel(dst, 0, 0, 0, 255, 255, "subtractive red from white");
}

void TestHueChangeGolden()
{
    RgssBitmapCore bitmap;
    MakeSolid(&bitmap, 1, 1, 255, 0, 0);
    bitmap.HueChange(180);
    CheckPixel(bitmap, 0, 0, 0, 255, 255, "hue +180 red to cyan");
}

void TestMultiplyBrightnessGolden()
{
    RgssBitmapCore bitmap;
    MakeSolid(&bitmap, 1, 1, 255, 0, 0);
    bitmap.MultiplyBrightness(255);
    CheckPixel(bitmap, 0, 0, 255, 0, 0, "brightness 255 identity");
    bitmap.MultiplyBrightness(0);
    CheckPixel(bitmap, 0, 0, 0, 0, 0, "brightness 0 black");
}

void TestSetPixelsWrapsExternalBuffer()
{
    unsigned short buffer[2 * 2];
    for (int i = 0; i < 4; ++i)
        buffer[i] = rgss::RgssBitmapCore::RgbTo565(0, 0, 255);
    RgssBitmapCore bitmap;
    assert(bitmap.SetPixels(2, 2, buffer, false));
    assert(bitmap.Pixels() == buffer);
    assert(bitmap.IsOpaque());
    CheckPixel(bitmap, 1, 1, 0, 0, 255, "external buffer wrap");
    /* The wrap must not free the caller's buffer (valgrind-visible in CI). */
}

} // namespace

int main()
{
    TestCreateFromRgba8OpaqueDropsMask();
    TestCreateFromRgba8KeepsAlphaMask();
    TestOpaqueBlitGolden();
    TestConstantOpacityBlit();
    TestPerPixelAlphaBlendGolden();
    TestOddWidthMaskedBlitGolden();
    TestMaskedStretchBlitGolden();
    TestFillRectAlphaCompositesMask();
    TestRgssTransparentBitmapSemantics();
    TestAddSubBlitGolden();
    TestHueChangeGolden();
    TestMultiplyBrightnessGolden();
    TestSetPixelsWrapsExternalBuffer();
    printf("PASS rgss_bitmap_test\n");
    return 0;
}
