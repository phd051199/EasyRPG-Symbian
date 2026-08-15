#include "rgss/core/RgssFontCore.h"

#include "GohuFont.h"
#include "RgssShinonomeFont.h"
#include "rgss/core/RgssBitmapCore.h"

namespace rgss {

namespace {

struct Glyph
{
    Glyph() : gohu(0), shinonome(0), width(6) {}
    const unsigned char* gohu;
    const ShinonomeGlyph* shinonome;
    int width;
};

unsigned int DecodeUtf8(const char* aText, size_t aLength, size_t* aOffset)
{
    if (!aText || !aOffset || *aOffset >= aLength)
        return 0;
    const unsigned char first = static_cast<unsigned char>(aText[(*aOffset)++]);
    if (first < 0x80)
        return first;

    int continuation = 0;
    unsigned int code = 0;
    if ((first & 0xe0) == 0xc0) { continuation = 1; code = first & 0x1f; }
    else if ((first & 0xf0) == 0xe0) { continuation = 2; code = first & 0x0f; }
    else if ((first & 0xf8) == 0xf0) { continuation = 3; code = first & 0x07; }
    else return '?';

    if (*aOffset + static_cast<size_t>(continuation) > aLength)
    {
        *aOffset = aLength;
        return '?';
    }
    for (int i = 0; i < continuation; ++i)
    {
        const unsigned char next = static_cast<unsigned char>(aText[(*aOffset)++]);
        if ((next & 0xc0) != 0x80)
            return '?';
        code = (code << 6) | (next & 0x3f);
    }
    if ((continuation == 1 && code < 0x80) ||
        (continuation == 2 && code < 0x800) ||
        (continuation == 3 && code < 0x10000) ||
        code > 0x10ffff || (code >= 0xd800 && code <= 0xdfff))
        return '?';
    return code;
}

const ShinonomeGlyph* FindShinonome(unsigned int aCode)
{
    int low = 0;
    int high = 11217;
    while (low < high)
    {
        const int middle = low + (high - low) / 2;
        if (SHINONOME_GOTHIC[middle].code < aCode)
            low = middle + 1;
        else
            high = middle;
    }
    if (low < 11217 && SHINONOME_GOTHIC[low].code == aCode)
        return &SHINONOME_GOTHIC[low];
    return 0;
}

Glyph LookupGlyph(unsigned int aCode)
{
    Glyph glyph;
    glyph.gohu = EasyRpgGohuGlyphRows(aCode);
    if (glyph.gohu)
    {
        glyph.width = 6;
        return glyph;
    }
    glyph.shinonome = FindShinonome(aCode);
    if (!glyph.shinonome)
        glyph.shinonome = FindShinonome('?');
    glyph.width = glyph.shinonome && glyph.shinonome->is_full ? 12 : 6;
    return glyph;
}

bool GlyphPixel(const Glyph& aGlyph, int aX, int aY)
{
    if (aX < 0 || aX >= aGlyph.width || aY < 0 || aY >= 12)
        return false;
    if (aGlyph.gohu)
        return aY < 11 && (aGlyph.gohu[aY] & (1u << aX)) != 0;
    if (aGlyph.shinonome)
        return (aGlyph.shinonome->data[aY] & (1u << aX)) != 0;
    return false;
}

int FontSize(const RgssFontStyle& aStyle)
{
    if (aStyle.size < 6) return 6;
    if (aStyle.size > 96) return 96;
    return aStyle.size;
}

int ScaleWidth(int aBaseWidth, int aFontSize)
{
    const int width = (aBaseWidth * aFontSize + 6) / 12;
    return width > 0 ? width : 1;
}

Rect Intersect(const Rect& aLeft, const Rect& aRight)
{
    const int x1 = aLeft.x > aRight.x ? aLeft.x : aRight.x;
    const int y1 = aLeft.y > aRight.y ? aLeft.y : aRight.y;
    const int x2a = aLeft.x + aLeft.width;
    const int x2b = aRight.x + aRight.width;
    const int y2a = aLeft.y + aLeft.height;
    const int y2b = aRight.y + aRight.height;
    const int x2 = x2a < x2b ? x2a : x2b;
    const int y2 = y2a < y2b ? y2a : y2b;
    return x2 > x1 && y2 > y1 ? Rect(x1, y1, x2 - x1, y2 - y1)
                                : Rect();
}

void DrawGlyph(RgssBitmapCore& aBitmap, const Rect& aClip, const Glyph& aGlyph,
               int aX, int aY, int aFontSize, const Color& aColor,
               bool aBold, bool aItalic)
{
    const int red = static_cast<int>(aColor.red + 0.5);
    const int green = static_cast<int>(aColor.green + 0.5);
    const int blue = static_cast<int>(aColor.blue + 0.5);
    const int alpha = static_cast<int>(aColor.alpha + 0.5);
    if (alpha <= 0)
        return;

    for (int sourceY = 0; sourceY < 12; ++sourceY)
    {
        const int top = aY + sourceY * aFontSize / 12;
        const int bottom = aY + (sourceY + 1) * aFontSize / 12;
        const int italicShift = aItalic ? ((11 - sourceY) * aFontSize / 12) / 4 : 0;
        for (int sourceX = 0; sourceX < aGlyph.width; ++sourceX)
        {
            if (!GlyphPixel(aGlyph, sourceX, sourceY))
                continue;
            const int left = aX + sourceX * aFontSize / 12 + italicShift;
            const int right = aX + (sourceX + 1) * aFontSize / 12 + italicShift;
            Rect block(left, top, right > left ? right - left : 1,
                       bottom > top ? bottom - top : 1);
            block = Intersect(block, aClip);
            if (!block.IsEmpty())
                aBitmap.FillRect(block, red, green, blue, alpha);
            if (aBold)
            {
                Rect boldBlock(left + 1, top, right > left ? right - left : 1,
                               bottom > top ? bottom - top : 1);
                boldBlock = Intersect(boldBlock, aClip);
                if (!boldBlock.IsEmpty())
                    aBitmap.FillRect(boldBlock, red, green, blue, alpha);
            }
        }
    }
}

} // namespace

Rect RgssMeasureTextUtf8(const char* aText, size_t aLength,
                         const RgssFontStyle& aStyle)
{
    const int fontSize = FontSize(aStyle);
    int width = 0;
    int maxWidth = 0;
    int lines = 1;
    size_t offset = 0;
    while (offset < aLength)
    {
        const unsigned int code = DecodeUtf8(aText, aLength, &offset);
        if (code == '\r')
            continue;
        if (code == '\n')
        {
            if (width > maxWidth) maxWidth = width;
            width = 0;
            ++lines;
            continue;
        }
        const Glyph glyph = LookupGlyph(code);
        width += ScaleWidth(glyph.width, fontSize) + (aStyle.bold ? 1 : 0);
    }
    if (width > maxWidth) maxWidth = width;
    return Rect(0, 0, maxWidth, fontSize * lines);
}

void RgssDrawTextUtf8(RgssBitmapCore& aBitmap, const Rect& aBounds,
                      const char* aText, size_t aLength,
                      const RgssFontStyle& aStyle, int aAlign)
{
    if (!aText || aLength == 0 || aBounds.IsEmpty() || !aBitmap.IsValid())
        return;

    const int fontSize = FontSize(aStyle);
    const Rect measured = RgssMeasureTextUtf8(aText, aLength, aStyle);
    int startX = aBounds.x;
    if (aAlign == 1)
        startX += (aBounds.width - measured.width) / 2;
    else if (aAlign == 2)
        startX += aBounds.width - measured.width;
    int x = startX;
    int y = aBounds.y + (aBounds.height - fontSize) / 2;
    if (y < aBounds.y) y = aBounds.y;

    const int shadowOffset = fontSize >= 18 ? 2 : 1;
    size_t offset = 0;
    while (offset < aLength)
    {
        const unsigned int code = DecodeUtf8(aText, aLength, &offset);
        if (code == '\r')
            continue;
        if (code == '\n')
        {
            x = startX;
            y += fontSize;
            continue;
        }
        const Glyph glyph = LookupGlyph(code);
        if (aStyle.shadow)
        {
            const double shadowAlpha = aStyle.color.alpha < 160.0
                                           ? aStyle.color.alpha : 160.0;
            DrawGlyph(aBitmap, aBounds, glyph,
                      x + shadowOffset, y + shadowOffset, fontSize,
                      Color(0, 0, 0, shadowAlpha), aStyle.bold, aStyle.italic);
        }
        if (aStyle.outline)
        {
            const int outlineOffset = fontSize >= 32 ? 2 : 1;
            for (int dy = -outlineOffset; dy <= outlineOffset; dy += outlineOffset)
            {
                for (int dx = -outlineOffset; dx <= outlineOffset; dx += outlineOffset)
                {
                    if (dx == 0 && dy == 0)
                        continue;
                    DrawGlyph(aBitmap, aBounds, glyph, x + dx, y + dy,
                              fontSize, aStyle.outColor,
                              aStyle.bold, aStyle.italic);
                }
            }
        }
        DrawGlyph(aBitmap, aBounds, glyph, x, y, fontSize,
                  aStyle.color, aStyle.bold, aStyle.italic);
        x += ScaleWidth(glyph.width, fontSize) + (aStyle.bold ? 1 : 0);
    }
}

} // namespace rgss
