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
    if (SymbianRenderDamage::IsFullPaintRecording())
        SymbianRenderDamage::Record(aSignature, aRect);
    const Rect* paintClip = SymbianRenderDamage::PaintClipRect();
    return paintClip &&
        (aRect.x >= paintClip->x + paintClip->width || paintClip->x >= aRect.x + aRect.width ||
         aRect.y >= paintClip->y + paintClip->height || paintClip->y >= aRect.y + aRect.height);
}

const Rect* TextPaintClip(Bitmap& aDestination)
{
    return IsDisplayDestination(aDestination) ? SymbianRenderDamage::PaintClipRect() : nullptr;
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

uint32_t MulDiv255(uint32_t aValue, uint32_t aAlpha)
{
    const uint32_t value = aValue * aAlpha + 128u;
    return (value + (value >> 8)) >> 8;
}

uint32_t PackColor(const Color& aColor)
{
    const uint32_t alpha = static_cast<uint32_t>(aColor.alpha);
    const uint32_t red = MulDiv255(static_cast<uint32_t>(aColor.red), alpha);
    const uint32_t green = MulDiv255(static_cast<uint32_t>(aColor.green), alpha);
    const uint32_t blue = MulDiv255(static_cast<uint32_t>(aColor.blue), alpha);
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

uint32_t ScalePixel(uint32_t aPixel, uint8_t aAlpha)
{
    if (aAlpha == 255)
        return aPixel;
    if (aAlpha == 0)
        return 0;

    const uint32_t alpha = MulDiv255(static_cast<uint32_t>((aPixel >> 24) & 0xffu), aAlpha);
    const uint32_t red = MulDiv255(static_cast<uint32_t>((aPixel >> 16) & 0xffu), aAlpha);
    const uint32_t green = MulDiv255(static_cast<uint32_t>((aPixel >> 8) & 0xffu), aAlpha);
    const uint32_t blue = MulDiv255(static_cast<uint32_t>(aPixel & 0xffu), aAlpha);
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

template <int Bpp>
inline uint32_t ReadColorPixelAt(const uint8_t* aRow, int aX)
{
    if (Bpp == 2)
        return ExpandRgb565(reinterpret_cast<const uint16_t*>(aRow)[aX]);
    return ExpandNative32(reinterpret_cast<const uint32_t*>(aRow)[aX]);
}

template <int Bpp>
inline void BlendColorPixelAt(uint8_t* aRow, int aX, uint32_t aSource)
{
    const uint32_t sourceAlpha = aSource >> 24;
    if (Bpp == 2)
    {
        uint16_t* destination = reinterpret_cast<uint16_t*>(aRow) + aX;
        if (sourceAlpha == 255)
            *destination = PackRgb565(aSource);
        else if (sourceAlpha != 0)
        {
            const uint16_t pixel = *destination;
            const uint32_t red5 = (pixel >> 11) & 31u;
            const uint32_t green6 = (pixel >> 5) & 63u;
            const uint32_t blue5 = pixel & 31u;
            const uint32_t red = (red5 << 3) | (red5 >> 2);
            const uint32_t green = (green6 << 2) | (green6 >> 4);
            const uint32_t blue = (blue5 << 3) | (blue5 >> 2);
            const uint32_t inverse = 255u - sourceAlpha;
            const uint32_t outRed = ((aSource >> 16) & 0xffu) + MulDiv255(red, inverse);
            const uint32_t outGreen = ((aSource >> 8) & 0xffu) + MulDiv255(green, inverse);
            const uint32_t outBlue = (aSource & 0xffu) + MulDiv255(blue, inverse);
            *destination = static_cast<uint16_t>(((outRed >> 3) << 11) |
                ((outGreen >> 2) << 5) | (outBlue >> 3));
        }
    }
    else
    {
        uint32_t* destination = reinterpret_cast<uint32_t*>(aRow) + aX;
        if (sourceAlpha == 255)
            *destination = PackNative32(aSource);
        else if (sourceAlpha != 0)
            *destination = PackNative32(OverPixel(aSource, ExpandNative32(*destination)));
    }
}

template <int Bpp>
inline uint8_t GlyphAlphaAt(const uint8_t* aRow, int aX)
{
    if (Bpp == 1)
        return aRow[aX];
    const uint32_t pixel = reinterpret_cast<const uint32_t*>(aRow)[aX];
    return Bitmap::pixel_format.a.unpack(pixel);
}

template <int GlyphBpp, int DestBpp>
void DrawSolidGlyphLoop(const uint8_t* aGlyphPixels,
                        uint8_t* aDestinationPixels,
                        int aX, int aY,
                        int aGlyphLeft, int aGlyphTop,
                        int aGlyphRight, int aGlyphBottom,
                        int aGlyphPitch, int aDestinationPitch,
                        uint32_t aColor)
{
    for (int glyphY = aGlyphTop; glyphY < aGlyphBottom; ++glyphY)
    {
        uint8_t* destination = aDestinationPixels + (aY + glyphY) * aDestinationPitch;
        const uint8_t* glyphRow = aGlyphPixels + glyphY * aGlyphPitch;
        for (int glyphX = aGlyphLeft; glyphX < aGlyphRight; ++glyphX)
        {
            const int destinationX = aX + glyphX;
            const uint8_t mask = GlyphAlphaAt<GlyphBpp>(glyphRow, glyphX);
            if (mask == 255)
                BlendColorPixelAt<DestBpp>(destination, destinationX, aColor);
            else if (mask != 0)
                BlendColorPixelAt<DestBpp>(destination, destinationX, ScalePixel(aColor, mask));
        }
    }
}

void DrawSolidGlyph(Bitmap& aDestination,
                    const Bitmap& aGlyph,
                    int aX,
                    int aY,
                    const Color& aColor)
{
    const int destinationBpp = aDestination.bpp();
    const int glyphBpp = aGlyph.bpp();
    if ((destinationBpp != 2 && destinationBpp != 4) ||
        (glyphBpp != 1 && glyphBpp != 4) ||
        !aDestination.pixels() || !aGlyph.pixels())
        return;

    const int destinationPitch = aDestination.pitch();
    const int glyphPitch = aGlyph.pitch();
    const uint32_t color = PackColor(aColor);
    int glyphLeft = aX < 0 ? -aX : 0;
    int glyphTop = aY < 0 ? -aY : 0;
    int glyphRight = aGlyph.GetWidth();
    int glyphBottom = aGlyph.GetHeight();
    if (aX + glyphRight > aDestination.GetWidth())
        glyphRight = aDestination.GetWidth() - aX;
    if (aY + glyphBottom > aDestination.GetHeight())
        glyphBottom = aDestination.GetHeight() - aY;
    const Rect* paintClip = TextPaintClip(aDestination);
    if (paintClip)
    {
        const Rect& dirty = *paintClip;
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
    const uint8_t* glyphPixels = reinterpret_cast<const uint8_t*>(aGlyph.pixels());
    if (glyphBpp == 1 && destinationBpp == 2)
        DrawSolidGlyphLoop<1, 2>(glyphPixels, destinationPixels, aX, aY,
                                 glyphLeft, glyphTop, glyphRight, glyphBottom,
                                 glyphPitch, destinationPitch, color);
    else if (glyphBpp == 1)
        DrawSolidGlyphLoop<1, 4>(glyphPixels, destinationPixels, aX, aY,
                                 glyphLeft, glyphTop, glyphRight, glyphBottom,
                                 glyphPitch, destinationPitch, color);
    else if (destinationBpp == 2)
        DrawSolidGlyphLoop<4, 2>(glyphPixels, destinationPixels, aX, aY,
                                 glyphLeft, glyphTop, glyphRight, glyphBottom,
                                 glyphPitch, destinationPitch, color);
    else
        DrawSolidGlyphLoop<4, 4>(glyphPixels, destinationPixels, aX, aY,
                                 glyphLeft, glyphTop, glyphRight, glyphBottom,
                                 glyphPitch, destinationPitch, color);
}

template <int GlyphBpp, int SystemBpp, int DestBpp>
void DrawSystemGlyphLoop(const uint8_t* aGlyphPixels,
                         const uint8_t* aSystemPixels,
                         uint8_t* aDestinationPixels,
                         int aX, int aY,
                         int aGlyphLeft, int aGlyphTop,
                         int aGlyphRight, int aGlyphBottom,
                         int aGlyphPitch, int aSystemPitch, int aDestinationPitch,
                         int aSourceX, int aSourceY)
{
    for (int glyphY = aGlyphTop; glyphY < aGlyphBottom; ++glyphY)
    {
        uint8_t* destination = aDestinationPixels + (aY + glyphY) * aDestinationPitch;
        const uint8_t* source = aSystemPixels + (aSourceY + glyphY) * aSystemPitch;
        const uint8_t* glyphRow = aGlyphPixels + glyphY * aGlyphPitch;
        for (int glyphX = aGlyphLeft; glyphX < aGlyphRight; ++glyphX)
        {
            const int destinationX = aX + glyphX;
            const uint8_t mask = GlyphAlphaAt<GlyphBpp>(glyphRow, glyphX);
            const uint32_t sourcePixel = ReadColorPixelAt<SystemBpp>(source, aSourceX + glyphX);
            if (mask == 255)
                BlendColorPixelAt<DestBpp>(destination, destinationX, sourcePixel);
            else if (mask != 0)
                BlendColorPixelAt<DestBpp>(destination, destinationX, ScalePixel(sourcePixel, mask));
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

    const int destinationBpp = aDestination.bpp();
    const int systemBpp = aSystem.bpp();
    const int glyphBpp = aGlyph.bpp();
    if ((destinationBpp != 2 && destinationBpp != 4) ||
        (systemBpp != 2 && systemBpp != 4) ||
        (glyphBpp != 1 && glyphBpp != 4) ||
        !aDestination.pixels() || !aGlyph.pixels() || !aSystem.pixels())
        return;

    const int destinationPitch = aDestination.pitch();
    const int systemPitch = aSystem.pitch();
    const int glyphPitch = aGlyph.pitch();
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
    const Rect* paintClip = TextPaintClip(aDestination);
    if (paintClip)
    {
        const Rect& dirty = *paintClip;
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
    const uint8_t* glyphPixels = reinterpret_cast<const uint8_t*>(aGlyph.pixels());
    if (glyphBpp == 1 && systemBpp == 2 && destinationBpp == 2)
        DrawSystemGlyphLoop<1, 2, 2>(glyphPixels, systemPixels, destinationPixels, aX, aY,
                                     glyphLeft, glyphTop, glyphRight, glyphBottom,
                                     glyphPitch, systemPitch, destinationPitch, sourceX, sourceY);
    else if (glyphBpp == 1 && systemBpp == 2)
        DrawSystemGlyphLoop<1, 2, 4>(glyphPixels, systemPixels, destinationPixels, aX, aY,
                                     glyphLeft, glyphTop, glyphRight, glyphBottom,
                                     glyphPitch, systemPitch, destinationPitch, sourceX, sourceY);
    else if (glyphBpp == 1 && destinationBpp == 2)
        DrawSystemGlyphLoop<1, 4, 2>(glyphPixels, systemPixels, destinationPixels, aX, aY,
                                     glyphLeft, glyphTop, glyphRight, glyphBottom,
                                     glyphPitch, systemPitch, destinationPitch, sourceX, sourceY);
    else if (glyphBpp == 1)
        DrawSystemGlyphLoop<1, 4, 4>(glyphPixels, systemPixels, destinationPixels, aX, aY,
                                     glyphLeft, glyphTop, glyphRight, glyphBottom,
                                     glyphPitch, systemPitch, destinationPitch, sourceX, sourceY);
    else if (systemBpp == 2 && destinationBpp == 2)
        DrawSystemGlyphLoop<4, 2, 2>(glyphPixels, systemPixels, destinationPixels, aX, aY,
                                     glyphLeft, glyphTop, glyphRight, glyphBottom,
                                     glyphPitch, systemPitch, destinationPitch, sourceX, sourceY);
    else if (systemBpp == 2)
        DrawSystemGlyphLoop<4, 2, 4>(glyphPixels, systemPixels, destinationPixels, aX, aY,
                                     glyphLeft, glyphTop, glyphRight, glyphBottom,
                                     glyphPitch, systemPitch, destinationPitch, sourceX, sourceY);
    else if (destinationBpp == 2)
        DrawSystemGlyphLoop<4, 4, 2>(glyphPixels, systemPixels, destinationPixels, aX, aY,
                                     glyphLeft, glyphTop, glyphRight, glyphBottom,
                                     glyphPitch, systemPitch, destinationPitch, sourceX, sourceY);
    else
        DrawSystemGlyphLoop<4, 4, 4>(glyphPixels, systemPixels, destinationPixels, aX, aY,
                                     glyphLeft, glyphTop, glyphRight, glyphBottom,
                                     glyphPitch, systemPitch, destinationPitch, sourceX, sourceY);
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
    if (SymbianRenderDamage::IsRecording() || SymbianRenderDamage::IsFullPaintRecording())
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

    SymbianRenderDamage::ScopedRecordSuppression suppressNestedRecording(
        SymbianRenderDamage::IsFullPaintRecording());

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
    aDestination.Touch(drawRect);
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
    if (SymbianRenderDamage::IsRecording() || SymbianRenderDamage::IsFullPaintRecording())
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

    SymbianRenderDamage::ScopedRecordSuppression suppressNestedRecording(
        SymbianRenderDamage::IsFullPaintRecording());

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
            y += lineHeight;
            continue;
        }

        BitmapRef glyph = CachedGlyph(font, code);
        if (glyph)
            DrawSolidGlyph(aDestination, *glyph, x, y, aColor);
        x += GlyphAdvance(glyph);
    }
    aDestination.Touch(drawRect);
}
