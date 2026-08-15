#include "rgss/core/RgssBitmapCore.h"
#include "rgss/core/RgssPlaneCore.h"
#include "rgss/core/RgssSpriteCore.h"
#include "rgss/core/RgssTilemapCore.h"
#include "rgss/core/RgssTypes.h"
#include "rgss/core/RgssWindowCore.h"
#include "rgss/core/RgssVxAutotileTables.h"

#include <assert.h>
#include <stdio.h>

using namespace rgss;

namespace {

void Pixel(const RgssBitmapCore& aBitmap, int aX, int aY,
           int aRed, int aGreen, int aBlue)
{
    int red = -1, green = -1, blue = -1;
    assert(aBitmap.GetPixel(aX, aY, &red, &green, &blue));
    if (red != aRed || green != aGreen || blue != aBlue)
        printf("pixel (%d,%d) got %d,%d,%d expected %d,%d,%d\n",
               aX, aY, red, green, blue, aRed, aGreen, aBlue);
    assert(red == aRed && green == aGreen && blue == aBlue);
}

void Fill(RgssBitmapCore& aBitmap, int aWidth, int aHeight,
          int aRed, int aGreen, int aBlue)
{
    assert(aBitmap.Create(aWidth, aHeight));
    aBitmap.FillRect(Rect(0, 0, aWidth, aHeight),
                     aRed, aGreen, aBlue, 255);
}

void TestPlaneRepeatAndScroll()
{
    RgssBitmapCore pattern;
    assert(pattern.Create(2, 1));
    pattern.SetPixel(0, 0, 255, 0, 0);
    pattern.SetPixel(1, 0, 0, 255, 0);

    RgssBitmapCore dest;
    Fill(dest, 6, 2, 0, 0, 0);
    RgssPlaneCore plane;
    plane.bitmap = &pattern;
    plane.ox = 1;
    plane.Draw(dest, 0, 0, Rect(0, 0, 6, 2));

    Pixel(dest, 0, 0, 0, 255, 0);
    Pixel(dest, 1, 0, 255, 0, 0);
    Pixel(dest, 2, 0, 0, 255, 0);
    Pixel(dest, 5, 1, 255, 0, 0);
    printf("plane repeat/scroll ok\n");
}

void TestWindowContentsAndClip()
{
    RgssBitmapCore contents;
    Fill(contents, 8, 8, 255, 0, 0);
    RgssBitmapCore dest;
    Fill(dest, 64, 48, 0, 0, 0);

    RgssWindowCore window;
    window.x = 4;
    window.y = 3;
    window.width = 40;
    window.height = 40;
    window.backOpacity = 0;
    window.contents = &contents;
    window.Draw(dest, 0, 0, Rect(0, 0, 30, 48));

    Pixel(dest, 19, 19, 0, 0, 0);
    Pixel(dest, 20, 19, 255, 0, 0);
    Pixel(dest, 27, 26, 255, 0, 0);
    Pixel(dest, 30, 20, 0, 0, 0);
    printf("window contents/clip ok\n");
}

void TestWindowToneDoesNotTintContents()
{
    RgssBitmapCore skin;
    Fill(skin, 192, 128, 80, 80, 80);
    RgssBitmapCore contents;
    Fill(contents, 8, 8, 255, 0, 0);
    RgssBitmapCore dest;
    Fill(dest, 64, 64, 0, 0, 0);

    RgssWindowCore window;
    window.width = 64;
    window.height = 64;
    window.windowskin = &skin;
    window.contents = &contents;
    window.tone.blue = 120;
    window.Draw(dest, 0, 0, Rect(0, 0, 64, 64));

    int red = 0, green = 0, blue = 0;
    assert(dest.GetPixel(4, 4, &red, &green, &blue));
    assert(blue > red);
    Pixel(dest, 17, 17, 255, 0, 0);
    printf("window tone scope ok\n");
}

void TestVxaWindowPaddingAndOpenness()
{
    RgssBitmapCore contents;
    Fill(contents, 8, 8, 255, 0, 0);
    RgssBitmapCore dest;
    Fill(dest, 32, 32, 0, 0, 0);

    RgssWindowCore window;
    window.generation = 3;
    window.width = 32;
    window.height = 32;
    window.padding = 12;
    window.paddingBottom = 12;
    window.backOpacity = 0;
    window.contents = &contents;
    window.openness = 128;
    window.Draw(dest, 0, 0, Rect(0, 0, 32, 32));

    Pixel(dest, 11, 12, 0, 0, 0);
    Pixel(dest, 12, 12, 255, 0, 0);
    Pixel(dest, 19, 19, 255, 0, 0);
    Pixel(dest, 12, 4, 0, 0, 0);
    printf("window vxa padding/openness ok\n");
}

void TestRegularTiles()
{
    RgssBitmapCore tileset;
    Fill(tileset, 256, 32, 0, 0, 0);
    tileset.FillRect(Rect(0, 0, 32, 32), 255, 0, 0, 255);
    tileset.FillRect(Rect(32, 0, 32, 32), 0, 255, 0, 255);

    Table map(2, 1, 1);
    map.Set(0, 0, 0, 384);
    map.Set(1, 0, 0, 385);

    RgssTilemapCore tilemap;
    tilemap.tileset = &tileset;
    tilemap.mapData = &map;

    RgssBitmapCore dest;
    Fill(dest, 64, 32, 0, 0, 255);
    tilemap.Draw(dest, 0, 0, Rect(0, 0, 64, 32));
    Pixel(dest, 8, 8, 255, 0, 0);
    Pixel(dest, 40, 8, 0, 255, 0);

    Fill(dest, 32, 32, 0, 0, 255);
    tilemap.ox = 32;
    tilemap.Draw(dest, 0, 0, Rect(0, 0, 32, 32));
    Pixel(dest, 8, 8, 0, 255, 0);
    printf("tilemap regular/scroll ok\n");
}

void PaintVxFloorPattern(RgssBitmapCore& aSheet, int aBaseQuarterX,
                         int aBaseQuarterY, int aShape,
                         const int aColors[4][3])
{
    for (int quadrant = 0; quadrant < 4; ++quadrant)
    {
        const int qx = aBaseQuarterX + KVxFloorAutotileTable[aShape][quadrant][0];
        const int qy = aBaseQuarterY + KVxFloorAutotileTable[aShape][quadrant][1];
        aSheet.FillRect(Rect(qx * 16, qy * 16, 16, 16),
                        aColors[quadrant][0], aColors[quadrant][1],
                        aColors[quadrant][2], 255);
    }
}

void TestVxaA2AutotileReferenceTable()
{
    RgssBitmapCore a2;
    assert(a2.CreateTransparent(256, 96));
    const int colors[4][3] = {
        {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 255}
    };
    PaintVxFloorPattern(a2, 0, 0, 47, colors);

    Table map(1, 1, 1);
    map.Set(0, 0, 0, 2816 + 47);
    RgssTilemapCore tilemap;
    tilemap.generation = 3;
    tilemap.bitmaps[1] = &a2;
    tilemap.mapData = &map;

    RgssBitmapCore dest;
    Fill(dest, 32, 32, 0, 0, 0);
    tilemap.Draw(dest, 0, 0, Rect(0, 0, 32, 32));
    Pixel(dest, 8, 8, 255, 0, 0);
    Pixel(dest, 24, 8, 0, 255, 0);
    Pixel(dest, 8, 24, 0, 0, 255);
    Pixel(dest, 24, 24, 255, 255, 255);
    printf("tilemap vxa A2 reference autotile ok\n");
}

void TestVxaA1WaterAnimation()
{
    RgssBitmapCore a1;
    assert(a1.CreateTransparent(192, 64));
    const int blue[4][3] = {
        {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}
    };
    const int red[4][3] = {
        {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}
    };
    PaintVxFloorPattern(a1, 0, 0, 47, blue);
    PaintVxFloorPattern(a1, 4, 0, 47, red);

    Table map(1, 1, 1);
    map.Set(0, 0, 0, 2048 + 47);
    RgssTilemapCore tilemap;
    tilemap.generation = 3;
    tilemap.bitmaps[0] = &a1;
    tilemap.mapData = &map;

    RgssBitmapCore dest;
    Fill(dest, 32, 32, 0, 0, 0);
    tilemap.Draw(dest, 0, 0, Rect(0, 0, 32, 32));
    Pixel(dest, 8, 8, 0, 0, 255);

    for (int i = 0; i < 30; ++i)
        tilemap.UpdateAnimation();
    Fill(dest, 32, 32, 0, 0, 0);
    tilemap.Draw(dest, 0, 0, Rect(0, 0, 32, 32));
    Pixel(dest, 8, 8, 255, 0, 0);
    printf("tilemap vxa A1 water animation ok\n");
}

void TestVxaRegularTileAndStarPriority()
{
    RgssBitmapCore sheet;
    Fill(sheet, 64, 32, 0, 0, 0);
    sheet.FillRect(Rect(32, 0, 32, 32), 0, 0, 255, 255);

    Table map(1, 1, 1);
    map.Set(0, 0, 0, 1);
    Table flags(2, 1, 1);

    RgssTilemapCore tilemap;
    tilemap.generation = 3;
    tilemap.bitmaps[5] = &sheet;
    tilemap.mapData = &map;
    tilemap.flags = &flags;

    RgssBitmapCore dest;
    Fill(dest, 32, 32, 0, 0, 0);
    tilemap.Draw(dest, 0, 0, Rect(0, 0, 32, 32));
    Pixel(dest, 16, 16, 0, 0, 255);

    RgssBitmapCore spriteBitmap;
    Fill(spriteBitmap, 32, 32, 255, 0, 0);
    RgssSpriteCore sprite;
    sprite.bitmap = &spriteBitmap;
    sprite.srcRect.Set(0, 0, 32, 32);
    sprite.z = 199;
    flags.Set(1, 0, 0, 0x10);

    RgssScene scene;
    scene.SetScreenSize(32, 32);
    scene.AddElement(&tilemap);
    scene.AddElement(&sprite);
    Fill(dest, 32, 32, 0, 0, 0);
    scene.Compose(dest);
    Pixel(dest, 16, 16, 0, 0, 255);

    sprite.z = 201;
    Fill(dest, 32, 32, 0, 0, 0);
    scene.Compose(dest);
    Pixel(dest, 16, 16, 255, 0, 0);
    printf("tilemap vxa regular/star priority ok\n");
}

void TestCompactAutotileAnimation()
{
    RgssBitmapCore autotile;
    assert(autotile.Create(64, 32));
    autotile.FillRect(Rect(0, 0, 32, 32), 0, 0, 255, 255);
    autotile.FillRect(Rect(32, 0, 32, 32), 255, 0, 0, 255);

    Table map(1, 1, 1);
    map.Set(0, 0, 0, 48);

    RgssTilemapCore tilemap;
    tilemap.autotiles[0] = &autotile;
    tilemap.mapData = &map;

    RgssBitmapCore dest;
    Fill(dest, 32, 32, 0, 0, 0);
    tilemap.Draw(dest, 0, 0, Rect(0, 0, 32, 32));
    Pixel(dest, 16, 16, 0, 0, 255);

    for (int i = 0; i < 16; ++i)
        tilemap.UpdateAnimation();
    Fill(dest, 32, 32, 0, 0, 0);
    tilemap.Draw(dest, 0, 0, Rect(0, 0, 32, 32));
    Pixel(dest, 16, 16, 255, 0, 0);
    printf("tilemap compact autotile animation ok\n");
}

void TestSpriteBushOpacityAndWave()
{
    RgssBitmapCore spriteBitmap;
    Fill(spriteBitmap, 8, 8, 255, 0, 0);
    RgssSpriteCore sprite;
    sprite.bitmap = &spriteBitmap;
    sprite.srcRect.Set(0, 0, 8, 8);

    RgssBitmapCore dest;
    Fill(dest, 16, 8, 0, 0, 0);
    sprite.bushTop = 4;
    sprite.bushOpacity = 0;
    sprite.Draw(dest, 0, 0, Rect(0, 0, 16, 8));
    Pixel(dest, 2, 1, 255, 0, 0);
    Pixel(dest, 2, 6, 0, 0, 0);

    sprite.bushTop = 0;
    sprite.waveAmp = 3;
    sprite.waveLength = 8;
    sprite.wavePhase = 90.0;
    sprite.x = 4;
    Fill(dest, 16, 8, 0, 0, 0);
    sprite.Draw(dest, 0, 0, Rect(0, 0, 16, 8));
    Pixel(dest, 4, 1, 0, 0, 0);
    Pixel(dest, 7, 1, 255, 0, 0);
    printf("sprite bush opacity/wave ok\n");
}

void TestPrioritySlicesInterleaveWithSprite()
{
    RgssBitmapCore tileset;
    Fill(tileset, 256, 32, 0, 0, 0);
    tileset.FillRect(Rect(0, 0, 32, 32), 0, 0, 255, 255);

    Table map(1, 1, 1);
    map.Set(0, 0, 0, 384);
    Table priorities(385, 1, 1);
    priorities.Set(384, 0, 0, 1);

    RgssTilemapCore tilemap;
    tilemap.tileset = &tileset;
    tilemap.mapData = &map;
    tilemap.priorities = &priorities;

    RgssBitmapCore spriteBitmap;
    Fill(spriteBitmap, 32, 32, 255, 0, 0);
    RgssSpriteCore sprite;
    sprite.bitmap = &spriteBitmap;
    sprite.srcRect.Set(0, 0, 32, 32);
    sprite.z = 63;

    RgssScene scene;
    scene.SetScreenSize(32, 32);
    scene.AddElement(&tilemap);
    scene.AddElement(&sprite);

    RgssBitmapCore screen;
    Fill(screen, 32, 32, 0, 0, 0);
    scene.Compose(screen);
    /* Row 0 priority 1 has effective z 64, so it covers z=63 sprite. */
    Pixel(screen, 16, 16, 0, 0, 255);

    Fill(screen, 32, 32, 0, 0, 0);
    sprite.z = 65;
    scene.Compose(screen);
    Pixel(screen, 16, 16, 255, 0, 0);
    printf("tilemap priority/sprite interleave ok\n");
}

void TestCanonicalAutotilePattern()
{
    RgssBitmapCore autotile;
    assert(autotile.CreateTransparent(96, 128));

    /* Pattern 47 is {1,2,7,8}: paint those source quadrants four colors. */
    autotile.FillRect(Rect(0, 0, 16, 16), 255, 0, 0, 255);      // 1
    autotile.FillRect(Rect(16, 0, 16, 16), 0, 255, 0, 255);     // 2
    autotile.FillRect(Rect(0, 16, 16, 16), 0, 0, 255, 255);     // 7
    autotile.FillRect(Rect(16, 16, 16, 16), 255, 255, 255, 255);// 8

    Table map(1, 1, 1);
    map.Set(0, 0, 0, 95); // autotile slot 0, pattern 47

    RgssTilemapCore tilemap;
    tilemap.autotiles[0] = &autotile;
    tilemap.mapData = &map;

    RgssBitmapCore dest;
    Fill(dest, 32, 32, 0, 0, 0);
    tilemap.Draw(dest, 0, 0, Rect(0, 0, 32, 32));
    Pixel(dest, 8, 8, 255, 0, 0);
    Pixel(dest, 24, 8, 0, 255, 0);
    Pixel(dest, 8, 24, 0, 0, 255);
    Pixel(dest, 24, 24, 255, 255, 255);
    printf("tilemap canonical autotile pattern ok\n");
}

} // namespace

int main()
{
    TestPlaneRepeatAndScroll();
    TestWindowContentsAndClip();
    TestWindowToneDoesNotTintContents();
    TestVxaWindowPaddingAndOpenness();
    TestRegularTiles();
    TestVxaRegularTileAndStarPriority();
    TestVxaA2AutotileReferenceTable();
    TestVxaA1WaterAnimation();
    TestCompactAutotileAnimation();
    TestSpriteBushOpacityAndWave();
    TestPrioritySlicesInterleaveWithSprite();
    TestCanonicalAutotilePattern();
    printf("rgss_m4_drawables_test: all ok\n");
    return 0;
}
