#include "rgss/core/RgssBitmapCore.h"
#include "rgss/core/RgssFontCore.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

using namespace rgss;

namespace {

int CountVisible(const RgssBitmapCore& aBitmap)
{
    int count = 0;
    for (int y = 0; y < aBitmap.Height(); ++y)
        for (int x = 0; x < aBitmap.Width(); ++x)
            if (aBitmap.AlphaAt(x, y) != 0)
                ++count;
    return count;
}

void TestMeasure()
{
    RgssFontStyle style;
    style.size = 22;
    Rect ascii = RgssMeasureTextUtf8("ABC", 3, style);
    assert(ascii.width == 33);
    assert(ascii.height == 22);

    const char japanese[] = "A\xe3\x81\x82"; // A + U+3042 HIRAGANA LETTER A
    Rect mixed = RgssMeasureTextUtf8(japanese, sizeof(japanese) - 1, style);
    assert(mixed.width == 33); // half-width Latin 11 + full-width JP 22
    assert(mixed.height == 22);
    printf("font measure latin/japanese ok\n");
}

void TestDrawAndAlignment()
{
    RgssFontStyle style;
    style.size = 22;
    style.shadow = false;
    style.color = Color(255, 0, 0, 255);

    RgssBitmapCore bitmap;
    assert(bitmap.CreateTransparent(96, 32));
    RgssDrawTextUtf8(bitmap, Rect(0, 0, 96, 32), "ABC", 3, style, 0);
    assert(CountVisible(bitmap) > 30);
    int red = 0, green = 0, blue = 0;
    bool foundRed = false;
    for (int y = 0; y < bitmap.Height() && !foundRed; ++y)
    {
        for (int x = 0; x < bitmap.Width(); ++x)
        {
            if (bitmap.AlphaAt(x, y) && bitmap.GetPixel(x, y, &red, &green, &blue))
            {
                if (red > 200 && green < 20 && blue < 20)
                {
                    foundRed = true;
                    break;
                }
            }
        }
    }
    assert(foundRed);

    bitmap.ClearTransparent();
    const char japanese[] = "\xe3\x81\x82";
    RgssDrawTextUtf8(bitmap, Rect(0, 0, 96, 32), japanese,
                     sizeof(japanese) - 1, style, 1);
    assert(CountVisible(bitmap) > 20);
    /* Centered 22px glyph should not touch the outer 20px margins. */
    for (int y = 0; y < bitmap.Height(); ++y)
    {
        assert(bitmap.AlphaAt(0, y) == 0);
        assert(bitmap.AlphaAt(95, y) == 0);
    }
    printf("font draw latin/japanese/alignment ok\n");
}

void TestUtf8FallbackIsBounded()
{
    RgssFontStyle style;
    const char malformed[] = "A\xffZ";
    Rect measured = RgssMeasureTextUtf8(malformed, sizeof(malformed) - 1, style);
    assert(measured.width > 0);
    assert(measured.width < 100);
    printf("font malformed utf8 fallback ok\n");
}

} // namespace

int main()
{
    TestMeasure();
    TestDrawAndAlignment();
    TestUtf8FallbackIsBounded();
    printf("rgss_font_test: all ok\n");
    return 0;
}
