#include <cctype>
#include <vector>

#include "bitmap.h"
#include "baseui.h"
#include "cache.h"
#include "color.h"
#include "font.h"
#include "rect.h"
#include "text.h"
#include "utils.h"
#include "RenderDamage.h"

namespace {
const unsigned int KGlyphCacheSize = 512;

struct TGlyphCacheEntry
{
    const Font* font;
    char32_t code;
    unsigned int owner;
};

TGlyphCacheEntry gGlyphCache[KGlyphCacheSize] = {};
std::vector<BitmapRef> gGlyphOwners;

unsigned int HashValue(unsigned int aHash, unsigned int aValue)
{
    return SymbianRenderDamage::HashValue(aHash, aValue);
}

unsigned int HashText(unsigned int aHash, const std::string& aText)
{
    for (std::size_t index = 0; index < aText.size(); ++index)
        aHash = HashValue(aHash, static_cast<unsigned char>(aText[index]));
    return aHash;
}

bool IsDisplayDestination(Bitmap& aDestination)
{
    return DisplayUi && DisplayUi->GetDisplaySurface() &&
        DisplayUi->GetDisplaySurface().get() == &aDestination;
}

bool SkipTextDraw(Bitmap& aDestination, unsigned int aSignature, const Rect& aRect)
{
    if (!IsDisplayDestination(aDestination))
        return false;
    if (SymbianRenderDamage::IsRecording())
    {
        SymbianRenderDamage::Record(aSignature, aRect);
        return true;
    }
    return SymbianRenderDamage::IsPainting() && !SymbianRenderDamage::IntersectsDirty(aRect);
}

bool ClipTextPixels(Bitmap& aDestination)
{
    return IsDisplayDestination(aDestination) && SymbianRenderDamage::IsPainting();
}

BitmapRef CachedGlyph(const FontRef& aFont, char32_t aCode)
{
    const unsigned int fontKey = static_cast<unsigned int>(reinterpret_cast<std::size_t>(aFont.get()) >> 3);
    const unsigned int codeKey = static_cast<unsigned int>(aCode);
    const unsigned int index = (fontKey ^ (codeKey * 2654435761u)) & (KGlyphCacheSize - 1u);
    TGlyphCacheEntry& entry = gGlyphCache[index];
    if (entry.font == aFont.get() && entry.code == aCode && entry.owner < gGlyphOwners.size())
        return gGlyphOwners[entry.owner];

    BitmapRef glyph = aFont->Glyph(aCode);
    if (!glyph)
        return glyph;

    if (entry.font && entry.owner < gGlyphOwners.size())
        gGlyphOwners[entry.owner] = glyph;
    else
    {
        entry.owner = static_cast<unsigned int>(gGlyphOwners.size());
        gGlyphOwners.push_back(glyph);
    }
    entry.font = aFont.get();
    entry.code = aCode;
    return glyph;
}

uint32_t PackColor(const Color& aColor)
{
    const uint32_t alpha = static_cast<uint32_t>(aColor.alpha);
    const uint32_t red = (static_cast<uint32_t>(aColor.red) * alpha + 127u) / 255u;
    const uint32_t green = (static_cast<uint32_t>(aColor.green) * alpha + 127u) / 255u;
    const uint32_t blue = (static_cast<uint32_t>(aColor.blue) * alpha + 127u) / 255u;
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

uint32_t ScalePixel(uint32_t aPixel, uint8_t aAlpha)
{
    if (aAlpha == 255)
        return aPixel;
    if (aAlpha == 0)
        return 0;

    const uint32_t alpha = (static_cast<uint32_t>((aPixel >> 24) & 0xffu) * aAlpha + 127u) / 255u;
    const uint32_t red = (static_cast<uint32_t>((aPixel >> 16) & 0xffu) * aAlpha + 127u) / 255u;
    const uint32_t green = (static_cast<uint32_t>((aPixel >> 8) & 0xffu) * aAlpha + 127u) / 255u;
    const uint32_t blue = (static_cast<uint32_t>(aPixel & 0xffu) * aAlpha + 127u) / 255u;
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

uint32_t OverPixel(uint32_t aSource, uint32_t aDestination)
{
    const uint32_t sourceAlpha = aSource >> 24;
    if (sourceAlpha == 0)
        return aDestination;
    if (sourceAlpha == 255)
        return aSource;

    const uint8_t inverseAlpha = static_cast<uint8_t>(255u - sourceAlpha);
    return aSource + ScalePixel(aDestination, inverseAlpha);
}

uint32_t ExpandRgb565(uint16_t aPixel)
{
    uint32_t red = (aPixel >> 11) & 31u;
    uint32_t green = (aPixel >> 5) & 63u;
    uint32_t blue = aPixel & 31u;
    red = (red << 3) | (red >> 2);
    green = (green << 2) | (green >> 4);
    blue = (blue << 3) | (blue >> 2);
    return 0xff000000u | (red << 16) | (green << 8) | blue;
}

uint16_t PackRgb565(uint32_t aPixel)
{
    return static_cast<uint16_t>(
        (((aPixel >> 19) & 31u) << 11) |
        (((aPixel >> 10) & 63u) << 5) |
        ((aPixel >> 3) & 31u));
}

uint32_t ExpandNative32(uint32_t aPixel)
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t alpha = 0;
    Bitmap::pixel_format.uint32_to_rgba(aPixel, red, green, blue, alpha);
    return (static_cast<uint32_t>(alpha) << 24) |
        (static_cast<uint32_t>(red) << 16) |
        (static_cast<uint32_t>(green) << 8) |
        static_cast<uint32_t>(blue);
}

uint32_t PackNative32(uint32_t aPixel)
{
    const uint8_t alpha = static_cast<uint8_t>(aPixel >> 24);
    const uint8_t red = static_cast<uint8_t>(aPixel >> 16);
    const uint8_t green = static_cast<uint8_t>(aPixel >> 8);
    const uint8_t blue = static_cast<uint8_t>(aPixel);
    return Bitmap::pixel_format.rgba_to_uint32_t(red, green, blue, alpha);
}

uint32_t ReadColorPixel(const Bitmap& aBitmap, const uint8_t* aRow, int aX)
{
    if (aBitmap.bpp() == 2)
        return ExpandRgb565(reinterpret_cast<const uint16_t*>(aRow)[aX]);
    if (aBitmap.bpp() == 4)
        return ExpandNative32(reinterpret_cast<const uint32_t*>(aRow)[aX]);
    return 0;
}

void BlendColorPixel(Bitmap& aBitmap, uint8_t* aRow, int aX, uint32_t aSource)
{
    const uint32_t sourceAlpha = aSource >> 24;
    if (aBitmap.bpp() == 2)
    {
        uint16_t* destination = reinterpret_cast<uint16_t*>(aRow) + aX;
        if (sourceAlpha == 255)
            *destination = PackRgb565(aSource);
        else if (sourceAlpha != 0)
            *destination = PackRgb565(OverPixel(aSource, ExpandRgb565(*destination)));
    }
    else if (aBitmap.bpp() == 4)
    {
        uint32_t* destination = reinterpret_cast<uint32_t*>(aRow) + aX;
        if (sourceAlpha == 255)
            *destination = PackNative32(aSource);
        else if (sourceAlpha != 0)
            *destination = PackNative32(OverPixel(aSource, ExpandNative32(*destination)));
    }
}

uint8_t GlyphAlpha(const Bitmap& aGlyph, int aX, int aY)
{
    const uint8_t* row = reinterpret_cast<const uint8_t*>(aGlyph.pixels()) + aY * aGlyph.pitch();
    if (aGlyph.bpp() == 1)
        return row[aX];
    if (aGlyph.bpp() == 4)
    {
        const uint32_t pixel = reinterpret_cast<const uint32_t*>(row)[aX];
        return Bitmap::pixel_format.a.unpack(pixel);
    }
    return 0;
}

void DrawSolidGlyph(Bitmap& aDestination,
                    const Bitmap& aGlyph,
                    int aX,
                    int aY,
                    const Color& aColor)
{
    if ((aDestination.bpp() != 2 && aDestination.bpp() != 4) ||
        !aDestination.pixels() || !aGlyph.pixels())
        return;

    const uint32_t color = PackColor(aColor);
    int glyphLeft = aX < 0 ? -aX : 0;
    int glyphTop = aY < 0 ? -aY : 0;
    int glyphRight = aGlyph.GetWidth();
    int glyphBottom = aGlyph.GetHeight();
    if (aX + glyphRight > aDestination.GetWidth())
        glyphRight = aDestination.GetWidth() - aX;
    if (aY + glyphBottom > aDestination.GetHeight())
        glyphBottom = aDestination.GetHeight() - aY;
    const bool clip = ClipTextPixels(aDestination);
    const bool multiClip = clip && SymbianRenderDamage::DirtyCount() > 1;
    if (clip)
    {
        const Rect& dirty = SymbianRenderDamage::CurrentPaintRect();
        if (dirty.x - aX > glyphLeft)
            glyphLeft = dirty.x - aX;
        if (dirty.y - aY > glyphTop)
            glyphTop = dirty.y - aY;
        if (dirty.x + dirty.width - aX < glyphRight)
            glyphRight = dirty.x + dirty.width - aX;
        if (dirty.y + dirty.height - aY < glyphBottom)
            glyphBottom = dirty.y + dirty.height - aY;
    }
    if (glyphLeft >= glyphRight || glyphTop >= glyphBottom)
        return;

    uint8_t* destinationPixels = reinterpret_cast<uint8_t*>(aDestination.pixels());
    for (int glyphY = glyphTop; glyphY < glyphBottom; ++glyphY)
    {
        const int destinationY = aY + glyphY;
        uint8_t* destination = destinationPixels + destinationY * aDestination.pitch();
        for (int glyphX = glyphLeft; glyphX < glyphRight; ++glyphX)
        {
            const int destinationX = aX + glyphX;
            if (multiClip && !SymbianRenderDamage::ContainsDirtyPoint(destinationX, destinationY))
                continue;
            const uint8_t mask = GlyphAlpha(aGlyph, glyphX, glyphY);
            if (mask == 255)
                BlendColorPixel(aDestination, destination, destinationX, color);
            else if (mask != 0)
                BlendColorPixel(aDestination, destination, destinationX, ScalePixel(color, mask));
        }
    }
}

void DrawSystemGlyph(Bitmap& aDestination,
                     const Bitmap& aGlyph,
                     int aX,
                     int aY,
                     const Bitmap& aSystem,
                     int aColor)
{
    if (aColor != Font::ColorShadow)
        DrawSolidGlyph(aDestination, aGlyph, aX + 1, aY + 1, aSystem.GetShadowColor());

    if ((aDestination.bpp() != 2 && aDestination.bpp() != 4) ||
        (aSystem.bpp() != 2 && aSystem.bpp() != 4) ||
        !aDestination.pixels() || !aGlyph.pixels() || !aSystem.pixels())
        return;

    const int sourceX = aColor == Font::ColorShadow ? 16 : aColor % 10 * 16 + 2;
    const int sourceY = aColor == Font::ColorShadow
        ? 32
        : aColor / 10 * 16 + 48 + 16 - aGlyph.GetHeight();
    int glyphLeft = aX < 0 ? -aX : 0;
    int glyphTop = aY < 0 ? -aY : 0;
    int glyphRight = aGlyph.GetWidth();
    int glyphBottom = aGlyph.GetHeight();
    if (sourceX < 0 && -sourceX > glyphLeft)
        glyphLeft = -sourceX;
    if (sourceY < 0 && -sourceY > glyphTop)
        glyphTop = -sourceY;
    if (aX + glyphRight > aDestination.GetWidth())
        glyphRight = aDestination.GetWidth() - aX;
    if (aY + glyphBottom > aDestination.GetHeight())
        glyphBottom = aDestination.GetHeight() - aY;
    if (sourceX + glyphRight > aSystem.GetWidth())
        glyphRight = aSystem.GetWidth() - sourceX;
    if (sourceY + glyphBottom > aSystem.GetHeight())
        glyphBottom = aSystem.GetHeight() - sourceY;
    const bool clip = ClipTextPixels(aDestination);
    const bool multiClip = clip && SymbianRenderDamage::DirtyCount() > 1;
    if (clip)
    {
        const Rect& dirty = SymbianRenderDamage::CurrentPaintRect();
        if (dirty.x - aX > glyphLeft)
            glyphLeft = dirty.x - aX;
        if (dirty.y - aY > glyphTop)
            glyphTop = dirty.y - aY;
        if (dirty.x + dirty.width - aX < glyphRight)
            glyphRight = dirty.x + dirty.width - aX;
        if (dirty.y + dirty.height - aY < glyphBottom)
            glyphBottom = dirty.y + dirty.height - aY;
    }
    if (glyphLeft >= glyphRight || glyphTop >= glyphBottom)
        return;

    uint8_t* destinationPixels = reinterpret_cast<uint8_t*>(aDestination.pixels());
    const uint8_t* systemPixels = reinterpret_cast<const uint8_t*>(aSystem.pixels());
    for (int glyphY = glyphTop; glyphY < glyphBottom; ++glyphY)
    {
        const int destinationY = aY + glyphY;
        const int systemY = sourceY + glyphY;
        uint8_t* destination = destinationPixels + destinationY * aDestination.pitch();
        const uint8_t* source = systemPixels + systemY * aSystem.pitch();
        for (int glyphX = glyphLeft; glyphX < glyphRight; ++glyphX)
        {
            const int destinationX = aX + glyphX;
            if (multiClip && !SymbianRenderDamage::ContainsDirtyPoint(destinationX, destinationY))
                continue;
            const uint8_t mask = GlyphAlpha(aGlyph, glyphX, glyphY);
            const uint32_t sourcePixel = ReadColorPixel(aSystem, source, sourceX + glyphX);
            if (mask == 255)
                BlendColorPixel(aDestination, destination, destinationX, sourcePixel);
            else if (mask != 0)
                BlendColorPixel(aDestination, destination, destinationX, ScalePixel(sourcePixel, mask));
        }
    }
}

bool IsExFontCode(char32_t aCode)
{
    return aCode < 128 && std::isalpha(static_cast<unsigned char>(aCode)) != 0;
}

char32_t ExFontCode(char32_t aCode)
{
    if (aCode >= U'a' && aCode <= U'z')
        return 26 + aCode - U'a';
    return aCode - U'A';
}

int GlyphAdvance(const BitmapRef& aGlyph)
{
    return aGlyph ? aGlyph->GetWidth() : 0;
}

Rect MeasureText(const FontRef& aFont, const std::string& aText, bool aExFont)
{
    const std::u32string text = Utils::DecodeUTF32(aText);
    BitmapRef baseGlyph = CachedGlyph(aFont, U'A');
    const int lineHeight = baseGlyph && baseGlyph->GetHeight() > 0 ? baseGlyph->GetHeight() : 12;
    int width = 0;
    int maxWidth = 0;
    int lines = 1;

    for (std::size_t index = 0; index < text.size(); ++index)
    {
        const char32_t code = text[index];
        if (code == U'\n')
        {
            if (width > maxWidth)
                maxWidth = width;
            width = 0;
            ++lines;
            continue;
        }
        if (aExFont && code == U'$' && index + 1 < text.size() &&
            IsExFontCode(text[index + 1]) && Font::exfont)
        {
            width += 12;
            ++index;
            continue;
        }
        width += GlyphAdvance(CachedGlyph(aFont, code));
    }

    if (width > maxWidth)
        maxWidth = width;
    return Rect(0, 0, maxWidth, lines * lineHeight);
}
}

void Text::Draw(Bitmap& aDestination,
                int aX,
                int aY,
                int aColor,
                std::string const& aText,
                Text::Alignment aAlignment)
{
    if (aText.empty())
        return;

    FontRef font = aDestination.GetFont();
    Rect size = MeasureText(font, aText, true);
    int lineX = aX;
    if (aAlignment == Text::AlignCenter)
        lineX -= size.width / 2;
    else if (aAlignment == Text::AlignRight)
        lineX -= size.width;

    BitmapRef system = Cache::System();
    if (!system)
        return;

    unsigned int signature = 0;
    if (SymbianRenderDamage::IsRecording())
    {
        signature = SymbianRenderDamage::HashSeed(1);
        signature = HashValue(signature, reinterpret_cast<std::size_t>(font.get()));
        signature = HashValue(signature, reinterpret_cast<std::size_t>(system.get()));
        signature = HashValue(signature, system->Revision());
        signature = HashValue(signature, static_cast<unsigned int>(aColor));
        signature = HashValue(signature, static_cast<unsigned int>(aAlignment));
        signature = HashText(signature, aText);
    }
    const Rect drawRect(lineX - 1, aY - 1, size.width + 2, size.height + 2);
    if (SkipTextDraw(aDestination, signature, drawRect))
        return;

    const std::u32string text = Utils::DecodeUTF32(aText);
    int x = lineX;
    int y = aY;
    BitmapRef baseGlyph = CachedGlyph(font, U'A');
    const int lineHeight = baseGlyph && baseGlyph->GetHeight() > 0 ? baseGlyph->GetHeight() : 12;

    for (std::size_t i = 0; i < text.size(); ++i)
    {
        const char32_t code = text[i];
        if (code == U'\n')
        {
            x = lineX;
            y += lineHeight;
            continue;
        }

        if (code == U'$' && i + 1 < text.size() && IsExFontCode(text[i + 1]) && Font::exfont)
        {
            BitmapRef glyph = CachedGlyph(Font::exfont, ExFontCode(text[i + 1]));
            if (glyph)
                DrawSystemGlyph(aDestination, *glyph, x, y, *system, aColor);
            x += 12;
            ++i;
            continue;
        }

        BitmapRef glyph = CachedGlyph(font, code);
        if (glyph)
            DrawSystemGlyph(aDestination, *glyph, x, y, *system, aColor);
        x += GlyphAdvance(glyph);
    }
    aDestination.Touch();
}

void Text::Draw(Bitmap& aDestination,
                int aX,
                int aY,
                Color aColor,
                std::string const& aText)
{
    if (aText.empty())
        return;

    FontRef font = aDestination.GetFont();
    Rect size = MeasureText(font, aText, false);
    unsigned int signature = 0;
    if (SymbianRenderDamage::IsRecording())
    {
        signature = SymbianRenderDamage::HashSeed(2);
        signature = HashValue(signature, reinterpret_cast<std::size_t>(font.get()));
        signature = HashValue(signature, static_cast<unsigned int>(aColor.red));
        signature = HashValue(signature, static_cast<unsigned int>(aColor.green));
        signature = HashValue(signature, static_cast<unsigned int>(aColor.blue));
        signature = HashValue(signature, static_cast<unsigned int>(aColor.alpha));
        signature = HashText(signature, aText);
    }
    const Rect drawRect(aX - 1, aY - 1, size.width + 2, size.height + 2);
    if (SkipTextDraw(aDestination, signature, drawRect))
        return;

    const std::u32string text = Utils::DecodeUTF32(aText);
    int x = aX;
    int y = aY;
    BitmapRef baseGlyph = CachedGlyph(font, U'A');
    const int lineHeight = baseGlyph && baseGlyph->GetHeight() > 0 ? baseGlyph->GetHeight() : 12;

    for (std::size_t i = 0; i < text.size(); ++i)
    {
        const char32_t code = text[i];
        if (code == U'\n')
        {
            x = aX;
            y += lineHeight > 0 ? lineHeight : 12;
            continue;
        }

        BitmapRef glyph = CachedGlyph(font, code);
        if (glyph)
            DrawSolidGlyph(aDestination, *glyph, x, y, aColor);
        x += GlyphAdvance(glyph);
    }
    aDestination.Touch();
}
