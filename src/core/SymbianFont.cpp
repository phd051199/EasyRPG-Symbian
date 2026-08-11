#include <algorithm>
#include <cstddef>

#include "bitmap.h"
#include "cache.h"
#include "color.h"
#include "font.h"
#include "rect.h"
#include "shinonome.h"
#include "utils.h"
#include "easyrpgsymbian/GohuFont.h"

bool operator<(ShinonomeGlyph const& aLeft, uint32_t aCode)
{
    return aLeft.code < aCode;
}

namespace {
ShinonomeGlyph const* FindGlyph(ShinonomeGlyph const* aData, std::size_t aSize, char32_t aCode)
{
    ShinonomeGlyph const* glyph = std::lower_bound(aData, aData + aSize, static_cast<uint32_t>(aCode));
    return glyph != aData + aSize && glyph->code == aCode ? glyph : 0;
}

ShinonomeGlyph const* FindGothicGlyph(char32_t aCode)
{
    return FindGlyph(SHINONOME_GOTHIC, sizeof(SHINONOME_GOTHIC) / sizeof(ShinonomeGlyph), aCode);
}

ShinonomeGlyph const* FindMinchoGlyph(char32_t aCode)
{
    ShinonomeGlyph const* glyph = FindGlyph(SHINONOME_MINCHO, sizeof(SHINONOME_MINCHO) / sizeof(ShinonomeGlyph), aCode);
    return glyph ? glyph : FindGothicGlyph(aCode);
}

enum TTone
{
    EToneNone,
    EToneGrave,
    EToneAcute,
    EToneHook,
    EToneTilde,
    EToneDot
};

struct TSyntheticGlyph
{
    unsigned int code;
    unsigned int base;
    bool horn;
    TTone tone;
};

const TSyntheticGlyph KSyntheticGlyphs[] = {
    {0x00dau, 'U', false, EToneAcute},
    {0x1ee4u, 'U', false, EToneDot},
    {0x1eeau, 'U', true, EToneGrave},
    {0x1eecu, 'U', true, EToneHook},
    {0x1eeeu, 'U', true, EToneTilde},
    {0x1ee8u, 'U', true, EToneAcute},
    {0x1ef0u, 'U', true, EToneDot},
    {0x1ef2u, 'Y', false, EToneGrave},
    {0x1ef6u, 'Y', false, EToneHook},
    {0x1ef8u, 'Y', false, EToneTilde},
    {0x00ddu, 'Y', false, EToneAcute},
    {0x1ef4u, 'Y', false, EToneDot}
};

const TSyntheticGlyph* FindSyntheticGlyph(unsigned int aCode)
{
    for (std::size_t index = 0; index < sizeof(KSyntheticGlyphs) / sizeof(KSyntheticGlyphs[0]); ++index)
    {
        if (KSyntheticGlyphs[index].code == aCode)
            return &KSyntheticGlyphs[index];
    }
    return 0;
}

BitmapRef CreateBitmap(int aWidth)
{
    BitmapRef bitmap = Bitmap::Create(0, aWidth, 12, 0, DynamicFormat(8, 8, 0, 8, 0, 8, 0, 8, 0, PF::Alpha));
    if (!bitmap || !bitmap->pixels())
        return bitmap;
    for (int y = 0; y < 12; ++y)
    {
        uint8_t* row = reinterpret_cast<uint8_t*>(bitmap->pixels()) + y * bitmap->pitch();
        for (int x = 0; x < aWidth; ++x)
            row[x] = 0;
    }
    return bitmap;
}

BitmapRef CreateGohuGlyph(const unsigned char* aRows)
{
    BitmapRef bitmap = CreateBitmap(6);
    if (!bitmap || !bitmap->pixels())
        return bitmap;
    for (int y = 0; y < 11; ++y)
    {
        uint8_t* row = reinterpret_cast<uint8_t*>(bitmap->pixels()) + y * bitmap->pitch();
        for (int x = 0; x < 6; ++x)
            row[x] = (aRows[y] & (1u << x)) != 0 ? 255 : 0;
    }
    return bitmap;
}

void PutPixel(Bitmap& aBitmap, int aX, int aY)
{
    if (aX < 0 || aX >= aBitmap.GetWidth() || aY < 0 || aY >= aBitmap.GetHeight())
        return;
    uint8_t* row = reinterpret_cast<uint8_t*>(aBitmap.pixels()) + aY * aBitmap.pitch();
    row[aX] = 255;
}

BitmapRef CreateSyntheticGlyph(const TSyntheticGlyph& aSpec)
{
    const unsigned char* baseRows = EasyRpgGohuGlyphRows(aSpec.base);
    if (!baseRows)
        return BitmapRef();

    BitmapRef bitmap = CreateBitmap(6);
    if (!bitmap || !bitmap->pixels())
        return bitmap;

    const bool above = aSpec.tone == EToneGrave || aSpec.tone == EToneAcute || aSpec.tone == EToneHook || aSpec.tone == EToneTilde;
    const int shift = above ? 1 : 0;
    for (int y = 0; y < 11 && y + shift < 12; ++y)
    {
        uint8_t* row = reinterpret_cast<uint8_t*>(bitmap->pixels()) + (y + shift) * bitmap->pitch();
        for (int x = 0; x < 6; ++x)
            row[x] = (baseRows[y] & (1u << x)) != 0 ? 255 : 0;
    }

    if (aSpec.horn)
    {
        PutPixel(*bitmap, 5, 2 + shift);
        PutPixel(*bitmap, 5, 3 + shift);
        PutPixel(*bitmap, 4, 3 + shift);
    }

    if (aSpec.tone == EToneGrave)
    {
        PutPixel(*bitmap, 1, 0);
        PutPixel(*bitmap, 2, 1);
    }
    else if (aSpec.tone == EToneAcute)
    {
        PutPixel(*bitmap, 4, 0);
        PutPixel(*bitmap, 3, 1);
    }
    else if (aSpec.tone == EToneHook)
    {
        PutPixel(*bitmap, 2, 0);
        PutPixel(*bitmap, 3, 0);
        PutPixel(*bitmap, 3, 1);
    }
    else if (aSpec.tone == EToneTilde)
    {
        PutPixel(*bitmap, 1, 0);
        PutPixel(*bitmap, 2, 0);
        PutPixel(*bitmap, 3, 1);
        PutPixel(*bitmap, 4, 1);
    }
    else if (aSpec.tone == EToneDot)
    {
        PutPixel(*bitmap, 2, 10);
        PutPixel(*bitmap, 3, 10);
    }

    return bitmap;
}

BitmapRef CreateShinonomeGlyph(ShinonomeGlyph const* aGlyph)
{
    if (!aGlyph)
        aGlyph = FindGothicGlyph('?');
    const int width = aGlyph && aGlyph->is_full ? 12 : 6;
    BitmapRef bitmap = CreateBitmap(width);
    if (!bitmap || !bitmap->pixels() || !aGlyph)
        return bitmap;
    for (int y = 0; y < 12; ++y)
    {
        uint8_t* row = reinterpret_cast<uint8_t*>(bitmap->pixels()) + y * bitmap->pitch();
        for (int x = 0; x < width; ++x)
            row[x] = (aGlyph->data[y] & (1u << x)) != 0 ? 255 : 0;
    }
    return bitmap;
}

struct HybridFont : public Font
{
    typedef ShinonomeGlyph const* (*TFallback)(char32_t);

    HybridFont(const char* aName, TFallback aFallback)
        : Font(aName, 9, false, false), fallback(aFallback)
    {
    }

    Rect GetSize(std::u32string const& aText) const override
    {
        int width = 0;
        for (std::u32string::const_iterator it = aText.begin(); it != aText.end(); ++it)
        {
            const unsigned int code = static_cast<unsigned int>(*it);
            if (EasyRpgGohuGlyphRows(code) || FindSyntheticGlyph(code))
            {
                width += 6;
                continue;
            }
            ShinonomeGlyph const* glyph = fallback(*it);
            width += glyph && glyph->is_full ? 12 : 6;
        }
        return Rect(0, 0, width, 12);
    }

    BitmapRef Glyph(char32_t aCode) override
    {
        const unsigned int code = static_cast<unsigned int>(aCode);
        const unsigned char* rows = EasyRpgGohuGlyphRows(code);
        if (rows)
            return CreateGohuGlyph(rows);
        const TSyntheticGlyph* synthetic = FindSyntheticGlyph(code);
        if (synthetic)
            return CreateSyntheticGlyph(*synthetic);
        return CreateShinonomeGlyph(fallback(aCode));
    }

    TFallback fallback;
};

struct ExFont : public Font
{
    ExFont() : Font("exfont", 12, false, false)
    {
    }

    Rect GetSize(std::u32string const&) const override
    {
        return Rect(0, 0, 12, 12);
    }

    BitmapRef Glyph(char32_t aCode) override
    {
        BitmapRef exfont = Cache::Exfont();
        Rect const rect((aCode % 13) * 12, (aCode / 13) * 12, 12, 12);
        return Bitmap::Create(*exfont, rect, true);
    }
};

FontRef const gothic = std::make_shared<HybridFont>("GohuGothic", &FindGothicGlyph);
FontRef const mincho = std::make_shared<HybridFont>("GohuMincho", &FindMinchoGlyph);
}

FontRef Font::Default(bool aMincho)
{
    return aMincho ? mincho : gothic;
}

FontRef Font::Create(const std::string&, int, bool, bool)
{
    return Font::Default();
}

void Font::Dispose()
{
}

Font::Font(const std::string& aName, int aSize, bool aBold, bool aItalic)
    : name(aName), size(aSize), bold(aBold), italic(aItalic)
{
}

Rect Font::GetSize(std::string const& aText) const
{
    return GetSize(Utils::DecodeUTF32(aText));
}

void Font::Render(Bitmap& aBitmap, int aX, int aY, Bitmap const& aSystem, int aColor, char32_t aCode)
{
    if (aColor != ColorShadow)
    {
        BitmapRef system = Cache::System();
        Render(aBitmap, aX + 1, aY + 1, system->GetShadowColor(), aCode);
    }

    BitmapRef glyph = Glyph(aCode);
    unsigned const sourceX = aColor == ColorShadow ? 16 : aColor % 10 * 16 + 2;
    unsigned const sourceY = aColor == ColorShadow ? 32 : aColor / 10 * 16 + 48 + 16 - glyph->height();
    aBitmap.MaskedBlit(Rect(aX, aY, glyph->width(), glyph->height()), *glyph, 0, 0, aSystem, sourceX, sourceY);
}

void Font::Render(Bitmap& aBitmap, int aX, int aY, Color const& aColor, char32_t aCode)
{
    BitmapRef glyph = Glyph(aCode);
    aBitmap.MaskedBlit(Rect(aX, aY, glyph->width(), glyph->height()), *glyph, 0, 0, aColor);
}

FontRef Font::exfont = std::make_shared<ExFont>();
