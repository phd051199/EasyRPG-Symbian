/* Host unit test for the RGSS renderer core (M-R2): scene composition,
 * sprite ordering, viewport clipping, opacity, transitions and downscale.
 * Plain asserts, pattern of tests/rgss_types_test.cpp. Pixel values are
 * RGB565; helpers expand channels to 0-255 so no pixman knowledge is
 * needed to read the expectations. */

#include "rgss/core/RgssBitmapCore.h"
#include "rgss/core/RgssDownscale.h"
#include "rgss/core/RgssGraphicsCore.h"
#include "rgss/core/RgssScene.h"
#include "rgss/core/RgssSpriteCore.h"

#include <cassert>
#include <cstdio>
#include <cstring>

using namespace rgss;

namespace {

const int KScreenW = 32;
const int KScreenH = 24;

unsigned short Pack(int aRed, int aGreen, int aBlue)
{
    return RgssBitmapCore::RgbTo565(aRed, aGreen, aBlue);
}

void Expand(unsigned short aPixel, int* aRed, int* aGreen, int* aBlue)
{
    RgssBitmapCore::From565(aPixel, aRed, aGreen, aBlue);
}

bool ChannelNear(int aValue, int aExpected, int aTolerance)
{
    int diff = aValue - aExpected;
    if (diff < 0) diff = -diff;
    return diff <= aTolerance;
}

/* Fills a bitmap with one color. */
void Fill(RgssBitmapCore& aBitmap, int aRed, int aGreen, int aBlue)
{
    Rect full(0, 0, aBitmap.Width(), aBitmap.Height());
    aBitmap.FillRect(full, aRed, aGreen, aBlue, 255);
}

unsigned short FramePixel(const RgssGraphicsCore& aGfx, int aX, int aY)
{
    return aGfx.GetFrame()[aY * aGfx.Width() + aX];
}

/* Solid 8x8 bitmap of one color. */
void MakeSolidBitmap(RgssBitmapCore& aBitmap, unsigned short aColor)
{
    aBitmap.Create(8, 8);
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            aBitmap.Pixels()[y * 8 + x] = aColor;
}

void Attach(RgssSpriteCore& aSprite, RgssScene& aScene, RgssBitmapCore& aBitmap)
{
    aSprite.bitmap = &aBitmap;
    aSprite.srcRect.Set(0, 0, aBitmap.Width(), aBitmap.Height());
    aScene.AddElement(&aSprite);
}

} // namespace

static void TestSpriteZOrder()
{
    RgssScene scene;
    RgssGraphicsCore gfx(scene);
    gfx.SetSize(KScreenW, KScreenH);

    RgssBitmapCore red, blue;
    MakeSolidBitmap(red, Pack(255, 0, 0));
    MakeSolidBitmap(blue, Pack(0, 0, 255));

    RgssSpriteCore low, high;
    Attach(low, scene, red);
    Attach(high, scene, blue);
    low.x = low.y = 4;
    high.x = high.y = 4;
    low.z = 0;
    high.z = 1;

    gfx.Update();
    assert(FramePixel(gfx, 8, 8) == Pack(0, 0, 255)); /* higher z on top */
    assert(FramePixel(gfx, 4, 10) == Pack(0, 0, 255));

    low.z = 5; /* swap */
    gfx.Update();
    assert(FramePixel(gfx, 8, 8) == Pack(255, 0, 0));
    printf("sprite z-order ok\n");
}

static void TestSpriteYTieBreak()
{
    RgssScene scene;
    RgssGraphicsCore gfx(scene);
    gfx.SetSize(KScreenW, KScreenH);

    RgssBitmapCore red, blue;
    MakeSolidBitmap(red, Pack(255, 0, 0));
    MakeSolidBitmap(blue, Pack(0, 0, 255));

    RgssSpriteCore upper, lower; /* same z, tie broken by y */
    Attach(upper, scene, red);
    Attach(lower, scene, blue);
    upper.x = lower.x = 2;
    upper.y = 4;  /* bitmap rows 4..11 */
    lower.y = 8;  /* bitmap rows 8..15; overlap rows 8..11 */
    gfx.Update();
    /* Lower y draws first, so the y=8 sprite wins the overlap. */
    assert(FramePixel(gfx, 5, 10) == Pack(0, 0, 255));

    upper.y = 12; /* now upper has the larger y and wins rows 12..15 */
    gfx.Update();
    assert(FramePixel(gfx, 5, 14) == Pack(255, 0, 0));
    printf("sprite y tie-break ok\n");
}

static void TestViewportClipping()
{
    RgssScene scene;
    RgssGraphicsCore gfx(scene);
    gfx.SetSize(KScreenW, KScreenH);

    ViewportElement viewport(Rect(4, 4, 10, 10));
    scene.AddViewport(&viewport);

    RgssBitmapCore green;
    MakeSolidBitmap(green, Pack(0, 255, 0));
    RgssSpriteCore sprite;
    sprite.viewport = &viewport;
    Attach(sprite, scene, green);
    sprite.x = sprite.y = 8; /* screen 12..19, clipped at viewport edge 14 */

    gfx.Update();
    assert(FramePixel(gfx, 12, 12) == Pack(0, 255, 0));
    assert(FramePixel(gfx, 13, 13) == Pack(0, 255, 0));
    assert(FramePixel(gfx, 14, 12) == 0); /* outside viewport rect */
    assert(FramePixel(gfx, 12, 14) == 0);
    assert(FramePixel(gfx, 3, 3) == 0); /* outside the viewport entirely */
    printf("viewport clipping ok\n");
}

static void TestViewportOffset()
{
    RgssScene scene;
    RgssGraphicsCore gfx(scene);
    gfx.SetSize(KScreenW, KScreenH);

    ViewportElement viewport(Rect(2, 2, 16, 16));
    viewport.ox = 4;
    viewport.oy = 2;
    scene.AddViewport(&viewport);

    RgssBitmapCore cyan;
    MakeSolidBitmap(cyan, Pack(0, 255, 255));
    RgssSpriteCore sprite;
    sprite.viewport = &viewport;
    Attach(sprite, scene, cyan);
    sprite.x = sprite.y = 8; /* screen (2-4+8, 2-2+8) = (6, 8) */

    gfx.Update();
    assert(FramePixel(gfx, 6, 8) == Pack(0, 255, 255));
    assert(FramePixel(gfx, 5, 8) == 0);
    assert(FramePixel(gfx, 6, 7) == 0);
    printf("viewport ox/oy ok\n");
}

static void TestOpacity()
{
    RgssScene scene;
    RgssGraphicsCore gfx(scene);
    gfx.SetSize(KScreenW, KScreenH);

    RgssBitmapCore white;
    MakeSolidBitmap(white, Pack(255, 255, 255));
    RgssSpriteCore sprite;
    Attach(sprite, scene, white);
    sprite.x = sprite.y = 0;

    sprite.opacity = 0; /* invisible */
    gfx.Update();
    assert(FramePixel(gfx, 3, 3) == 0);

    sprite.opacity = 255; /* opaque */
    gfx.Update();
    assert(FramePixel(gfx, 3, 3) == Pack(255, 255, 255));

    sprite.opacity = 128; /* halfway over the black background */
    gfx.Update();
    int r, g, b;
    Expand(FramePixel(gfx, 3, 3), &r, &g, &b);
    /* ~128 per channel; small tolerance for RGB565 quantization. */
    assert(ChannelNear(r, 128, 6));
    assert(ChannelNear(g, 128, 6));
    assert(ChannelNear(b, 128, 6));
    printf("opacity ok\n");
}

static void TestFillRectClear()
{
    RgssBitmapCore bmp;
    bmp.Create(8, 8);
    assert(bmp.IsValid() && bmp.Width() == 8 && bmp.Height() == 8);

    Rect rect(2, 2, 4, 4);
    bmp.FillRect(rect, 255, 0, 0, 255);
    assert(bmp.Pixels()[2 * 8 + 2] == Pack(255, 0, 0));
    assert(bmp.Pixels()[5 * 8 + 5] == Pack(255, 0, 0));
    assert(bmp.Pixels()[6 * 8 + 6] == 0); /* outside the rect */

    int r, g, b;
    assert(bmp.GetPixel(3, 3, &r, &g, &b) && r == 255 && g == 0 && b == 0);
    assert(!bmp.GetPixel(8, 8, &r, &g, &b)); /* out of range */

    bmp.SetPixel(0, 0, 0, 255, 0);
    assert(bmp.Pixels()[0] == Pack(0, 255, 0));

    /* Alpha fill over red: half-strength green over red -> yellow-ish. */
    bmp.FillRect(Rect(2, 2, 4, 4), 0, 0, 255, 128);
    assert(bmp.GetPixel(3, 3, &r, &g, &b));
    assert(ChannelNear(r, 128, 6));
    assert(ChannelNear(b, 128, 6));

    bmp.ClearRect(Rect(2, 2, 4, 4));
    assert(bmp.Pixels()[3 * 8 + 3] == 0);
    assert(bmp.Pixels()[0] == Pack(0, 255, 0)); /* untouched */

    bmp.Clear();
    assert(bmp.Pixels()[0] == 0);
    printf("fill_rect/clear ok\n");
}

static void TestDownscale2To1()
{
    /* 4x4 -> 2x2. Top-left block averages 1 red pixel over 4 (r ~ 64);
     * the other blocks are uniform green/white/blue. */
    unsigned short src[16];
    memset(src, 0, sizeof(src));
    src[0 * 4 + 0] = Pack(255, 0, 0);
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y) {
            if (x >= 2 && y < 2) src[y * 4 + x] = Pack(0, 255, 0);
            if (y >= 2 && x < 2) src[y * 4 + x] = Pack(255, 255, 255);
            if (x >= 2 && y >= 2) src[y * 4 + x] = Pack(0, 0, 255);
        }

    unsigned short dst[4];
    RgssDownscaleRgb565(src, 4, 4, dst, 2, 2);

    /* Expected channel math mirrors the definition: expand to 8-bit,
     * average (sum + 2) / 4, repack to 5/6/5 bits. */
    int r, g, b;
    Expand(dst[0], &r, &g, &b); /* (255 + 0 + 0 + 0 + 2) / 4 = 64 */
    assert(r >= 60 && r <= 68);
    assert(g == 0 && b == 0);
    assert(dst[1] == Pack(0, 255, 0));
    assert(dst[2] == Pack(255, 255, 255));
    assert(dst[3] == Pack(0, 0, 255));
    printf("downscale 2:1 average ok\n");
}

static void TestDownscale3To2()
{
    /* 3x3 -> 2x2 nearest: columns/rows 0,1 map to 0,1 (floor(i * 3 / 2)). */
    unsigned short src[9];
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            src[y * 3 + x] = static_cast<unsigned short>(0x10 * (y * 3 + x + 1));

    unsigned short dst[4];
    RgssDownscaleRgb565(src, 3, 3, dst, 2, 2);

    assert(dst[0] == src[0]);
    assert(dst[1] == src[1]);
    assert(dst[2] == src[3]);
    assert(dst[3] == src[4]);
    printf("downscale 3:2 nearest ok\n");
}

static void TestSpriteZoom()
{
    RgssScene scene;
    RgssGraphicsCore gfx(scene);
    gfx.SetSize(KScreenW, KScreenH);

    RgssBitmapCore red;
    red.Create(2, 2);
    Fill(red, 255, 0, 0);

    RgssSpriteCore sprite;
    Attach(sprite, scene, red);
    sprite.x = sprite.y = 1;
    sprite.zoomX = 2.0;
    sprite.zoomY = 2.0; /* 2x2 red becomes a 4x4 block at (1,1) */

    gfx.Update();
    assert(FramePixel(gfx, 1, 1) == Pack(255, 0, 0));
    assert(FramePixel(gfx, 4, 4) == Pack(255, 0, 0));
    assert(FramePixel(gfx, 0, 1) == 0);
    assert(FramePixel(gfx, 5, 1) == 0);
    assert(FramePixel(gfx, 2, 5) == 0);
    printf("sprite zoom ok\n");
}

static void TestMirrorAndBlend()
{
    /* 2x2 bitmap: red left column, blue right column; mirrored blit
     * swaps the columns (manual slow path). */
    RgssBitmapCore src;
    src.Create(2, 2);
    src.Pixels()[0] = Pack(255, 0, 0);
    src.Pixels()[1] = Pack(0, 0, 255);
    src.Pixels()[2] = Pack(255, 0, 0);
    src.Pixels()[3] = Pack(0, 0, 255);

    RgssBitmapCore dst;
    dst.Create(4, 4);
    Fill(dst, 0, 0, 0);
    dst.StretchBlt(&src, Rect(0, 0, 2, 2), 1, 1, 1.0, 1.0, 255, true,
                   KBlendNormal);
    assert(dst.Pixels()[1 * 4 + 1] == Pack(0, 0, 255)); /* mirrored */
    assert(dst.Pixels()[1 * 4 + 2] == Pack(255, 0, 0));

    /* Additive over black stays the source; subtractive on white clips. */
    RgssBitmapCore black, white;
    black.Create(2, 1);
    Fill(black, 0, 0, 0);
    black.BltAdditive(&src, Rect(0, 0, 2, 1), 0, 0, 255);
    assert(black.Pixels()[0] == Pack(255, 0, 0));
    assert(black.Pixels()[1] == Pack(0, 0, 255));

    white.Create(2, 1);
    Fill(white, 255, 255, 255);
    white.BltSubtractive(&src, Rect(0, 0, 2, 1), 0, 0, 255);
    assert(white.Pixels()[0] == Pack(0, 255, 255)); /* white - red */
    assert(white.Pixels()[1] == Pack(255, 255, 0)); /* white - blue */
    printf("mirror/add/sub blend ok\n");
}

static void TestAlphaMaskBlit()
{
    /* RGBA source: left pixel opaque red, right pixel half-transparent
     * blue. CreateFromRgba8 drops the mask only when fully opaque. */
    unsigned char rgba[2 * 4];
    rgba[0] = 255; rgba[1] = 0; rgba[2] = 0; rgba[3] = 255;
    rgba[4] = 0; rgba[5] = 0; rgba[6] = 255; rgba[7] = 128;

    RgssBitmapCore src;
    src.CreateFromRgba8(2, 1, rgba);
    assert(!src.IsOpaque()); /* the 128-alpha pixel keeps the mask */
    assert(src.AlphaAt(0, 0) == 255 && src.AlphaAt(1, 0) == 128);

    RgssBitmapCore dst;
    dst.Create(2, 1);
    Fill(dst, 255, 255, 255);
    dst.Blit(&src, Rect(0, 0, 2, 1), 0, 0, 255);

    assert(dst.Pixels()[0] == Pack(255, 0, 0)); /* opaque red replaces */
    int r, g, b;
    Expand(dst.Pixels()[1], &r, &g, &b); /* blue at 128 over white */
    assert(r > 110 && r < 150); /* white shows through 128-alpha blue */
    assert(g > 110 && g < 150);
    assert(b > 240); /* blue channel already maxed in both layers */
    printf("alpha mask blit ok\n");
}

static void TestHueChange()
{
    RgssBitmapCore bmp;
    bmp.Create(4, 4);
    Fill(bmp, 255, 0, 0);
    bmp.HueChange(120); /* red (hue 0) rotates to green (hue 120) */
    int r, g, b;
    Expand(bmp.Pixels()[0], &r, &g, &b);
    assert(g > 200 && r < 50 && b < 50);
    printf("hue change ok\n");
}

static void TestTransitionCrossfade()
{
    RgssScene scene;
    RgssGraphicsCore gfx(scene);
    gfx.SetSize(KScreenW, KScreenH);

    RgssBitmapCore red, blue;
    red.Create(KScreenW, KScreenH);
    Fill(red, 255, 0, 0);
    blue.Create(KScreenW, KScreenH);
    Fill(blue, 0, 0, 255);

    RgssSpriteCore before, after;
    Attach(before, scene, red);
    gfx.Update();
    assert(FramePixel(gfx, 10, 10) == Pack(255, 0, 0));

    gfx.Freeze();
    before.visible = false;
    Attach(after, scene, blue);
    gfx.Transition(4, 0);
    gfx.Update(); /* step 0: fully frozen red */
    assert(FramePixel(gfx, 10, 10) == Pack(255, 0, 0));

    gfx.Update(); /* step 1: frozen at 255 * 3/4 over live blue */
    int r, g, b;
    Expand(FramePixel(gfx, 10, 10), &r, &g, &b);
    assert(r > 150 && b > 20 && b < 110);

    for (int i = 0; i < 4; ++i)
        gfx.Update();
    assert(FramePixel(gfx, 10, 10) == Pack(0, 0, 255)); /* fully live */
    printf("transition crossfade ok\n");
}

static void TestFadeOutFadeIn()
{
    RgssScene scene;
    RgssGraphicsCore gfx(scene);
    gfx.SetSize(KScreenW, KScreenH);

    RgssBitmapCore white;
    white.Create(KScreenW, KScreenH);
    Fill(white, 255, 255, 255);
    RgssSpriteCore sprite;
    Attach(sprite, scene, white);
    gfx.Update();
    assert(FramePixel(gfx, 10, 10) == Pack(255, 255, 255));

    gfx.FadeOut(2);
    gfx.Update(); /* halfway: brightness 255 * 1/2 */
    int r, g, b;
    Expand(FramePixel(gfx, 10, 10), &r, &g, &b);
    assert(ChannelNear(r, 128, 8));
    gfx.Update(); /* black */
    assert(FramePixel(gfx, 10, 10) == 0);
    assert(gfx.Brightness() == 0);

    gfx.FadeIn(2);
    gfx.Update();
    gfx.Update();
    assert(FramePixel(gfx, 10, 10) == Pack(255, 255, 255));
    printf("fade out/in ok\n");
}

static void TestFrameCounter()
{
    RgssScene scene;
    RgssGraphicsCore gfx(scene);
    gfx.SetSize(KScreenW, KScreenH);
    assert(gfx.frameCount == 0 && gfx.frameRate == 40);
    gfx.Update();
    gfx.Update();
    assert(gfx.frameCount == 2);
    assert(gfx.Width() == KScreenW && gfx.Height() == KScreenH);
    printf("frame counter ok\n");
}

int main()
{
    TestSpriteZOrder();
    TestSpriteYTieBreak();
    TestViewportClipping();
    TestViewportOffset();
    TestOpacity();
    TestFillRectClear();
    TestDownscale2To1();
    TestDownscale3To2();
    TestSpriteZoom();
    TestMirrorAndBlend();
    TestAlphaMaskBlit();
    TestHueChange();
    TestTransitionCrossfade();
    TestFadeOutFadeIn();
    TestFrameCounter();
    printf("rgss_compositor_test: all ok\n");
    return 0;
}
