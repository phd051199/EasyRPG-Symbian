#include "GohuFont.h"
#include "RgssBitmapBridge.h"

namespace
{
struct TBitmapSlot
{
    unsigned short* color;
    unsigned char* alpha;
    int width;
    int height;
    unsigned short generation;
    unsigned char used;
};

struct TBitmapState
{
    EasyRpgRgssBitmapHost host;
    TBitmapSlot* slots;
    void* glyph_context;
    int initialized;
};

TBitmapState gBitmaps;

enum { KFastBlitWidth = 320 };

#if defined(__arm__)
extern "C" int __aeabi_idiv(int numerator, int denominator);
#endif

int DivideInt(int numerator, int denominator)
{
#if defined(__arm__)
    return __aeabi_idiv(numerator, denominator);
#else
    return numerator / denominator;
#endif
}

int MaxInt(int left, int right)
{
    return left > right ? left : right;
}

int ClampByte(int value)
{
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}

unsigned short Pack565(int red, int green, int blue)
{
    const unsigned int r = static_cast<unsigned int>(ClampByte(red));
    const unsigned int g = static_cast<unsigned int>(ClampByte(green));
    const unsigned int b = static_cast<unsigned int>(ClampByte(blue));
    return static_cast<unsigned short>(((r >> 3) << 11) |
                                       ((g >> 2) << 5) |
                                       (b >> 3));
}

void Unpack565(unsigned short value, int& red, int& green, int& blue)
{
    const int r = (value >> 11) & 31;
    const int g = (value >> 5) & 63;
    const int b = value & 31;
    red = (r << 3) | (r >> 2);
    green = (g << 2) | (g >> 4);
    blue = (b << 3) | (b >> 2);
}

int Divide255Round(int value)
{
    const int rounded = value + 128;
    return (rounded + (rounded >> 8)) >> 8;
}

int Divide255SignedRound(int value)
{
    return value >= 0 ? Divide255Round(value) : -Divide255Round(-value);
}

unsigned short Blend565(unsigned short destination,
                        unsigned short source,
                        int alpha)
{
    if (alpha <= 0)
        return destination;
    if (alpha >= 255)
        return source;

    const int srcR = (source >> 11) & 31;
    const int srcG = (source >> 5) & 63;
    const int srcB = source & 31;
    const int dstR = (destination >> 11) & 31;
    const int dstG = (destination >> 5) & 63;
    const int dstB = destination & 31;
    const int inverse = 255 - alpha;
    const int outR = Divide255Round(srcR * alpha + dstR * inverse);
    const int outG = Divide255Round(srcG * alpha + dstG * inverse);
    const int outB = Divide255Round(srcB * alpha + dstB * inverse);
    return static_cast<unsigned short>((outR << 11) | (outG << 5) | outB);
}

unsigned short Composite565(unsigned short destination,
                            unsigned short source,
                            int alpha,
                            int blendType)
{
    if (blendType == ERgssBitmapBlendNormal)
        return Blend565(destination, source, alpha);
    if (alpha <= 0)
        return destination;

    const int srcR = (source >> 11) & 31;
    const int srcG = (source >> 5) & 63;
    const int srcB = source & 31;
    const int dstR = (destination >> 11) & 31;
    const int dstG = (destination >> 5) & 63;
    const int dstB = destination & 31;
    const int amountR = Divide255Round(srcR * alpha);
    const int amountG = Divide255Round(srcG * alpha);
    const int amountB = Divide255Round(srcB * alpha);
    int outR = dstR;
    int outG = dstG;
    int outB = dstB;
    if (blendType == ERgssBitmapBlendAdd)
    {
        outR += amountR;
        outG += amountG;
        outB += amountB;
        if (outR > 31)
            outR = 31;
        if (outG > 63)
            outG = 63;
        if (outB > 31)
            outB = 31;
    }
    else
    {
        outR -= amountR;
        outG -= amountG;
        outB -= amountB;
        if (outR < 0)
            outR = 0;
        if (outG < 0)
            outG = 0;
        if (outB < 0)
            outB = 0;
    }
    return static_cast<unsigned short>((outR << 11) | (outG << 5) | outB);
}

TBitmapSlot* FindSlot(int handle)
{
    if (!gBitmaps.initialized || handle <= 0)
        return 0;
    const int encodedIndex = handle & 0xffff;
    const int generation = (handle >> 16) & 0x7fff;
    if (encodedIndex <= 0 || encodedIndex > gBitmaps.host.max_bitmaps || generation <= 0)
        return 0;
    TBitmapSlot* slot = &gBitmaps.slots[encodedIndex - 1];
    if (!slot->used || slot->generation != generation)
        return 0;
    return slot;
}

int MakeHandle(int index, unsigned short generation)
{
    return (static_cast<int>(generation & 0x7fff) << 16) | (index + 1);
}

void ReleaseSlot(TBitmapSlot& slot)
{
    if (slot.color)
        gBitmaps.host.release(gBitmaps.host.context, slot.color);
    if (slot.alpha)
        gBitmaps.host.release(gBitmaps.host.context, slot.alpha);
    slot.color = 0;
    slot.alpha = 0;
    slot.width = 0;
    slot.height = 0;
    slot.used = 0;
}

void ZeroDirty(EasyRpgRgssRect* rect)
{
    if (!rect)
        return;
    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;
}

struct TResolvedGlyph
{
    const unsigned char* rows8;
    const unsigned short* rows16;
    int width;
    int height;
    int advance;
};

int ResolveGlyph(unsigned int codepoint, TResolvedGlyph& glyph)
{
    glyph.rows8 = 0;
    glyph.rows16 = 0;
    glyph.width = 0;
    glyph.height = 0;
    glyph.advance = 0;
    if (gBitmaps.host.glyph)
    {
        const unsigned short* rows = 0;
        int width = 0;
        int height = 0;
        int advance = 0;
        if (gBitmaps.host.glyph(gBitmaps.glyph_context,
                                codepoint,
                                &rows,
                                &width,
                                &height,
                                &advance) > 0 &&
            rows && width > 0 && width <= 16 &&
            height > 0 && height <= 32 && advance > 0 && advance <= 32)
        {
            glyph.rows16 = rows;
            glyph.width = width;
            glyph.height = height;
            glyph.advance = advance;
            return 1;
        }
    }

    const unsigned char* rows = EasyRpgGohuGlyphRows(codepoint);
    if (!rows)
        rows = EasyRpgGohuGlyphRows('?');
    if (!rows)
        return 0;
    glyph.rows8 = rows;
    glyph.width = 6;
    glyph.height = 11;
    glyph.advance = 6;
    return 1;
}

int GlyphBit(const TResolvedGlyph& glyph, int x, int y)
{
    if (x < 0 || y < 0 || x >= glyph.width || y >= glyph.height)
        return 0;
    if (glyph.rows16)
        return (glyph.rows16[y] & (static_cast<unsigned int>(1) << x)) != 0;
    return (glyph.rows8[y] & (static_cast<unsigned int>(1) << x)) != 0;
}

int ScaleMetric(int value, int baseHeight, int fontSize)
{
    if (value <= 0 || baseHeight <= 0 || fontSize <= 0)
        return 0;
    return (value * fontSize + baseHeight - 1) / baseHeight;
}

int ValidateTextStyle(const EasyRpgRgssTextStyle* style)
{
    if (!style || style->font_size <= 0 || style->font_size > 96 ||
        style->red < 0 || style->red > 255 ||
        style->green < 0 || style->green > 255 ||
        style->blue < 0 || style->blue > 255 ||
        style->alpha < 0 || style->alpha > 255 ||
        style->align < 0 || style->align > 2)
        return ERgssBitmapErrorInvalidArgument;
    return ERgssBitmapOk;
}

void CompositeBitmapPixel(TBitmapSlot& slot,
                          int x,
                          int y,
                          unsigned short sourceColor,
                          int sourceAlpha)
{
    if (x < 0 || y < 0 || x >= slot.width || y >= slot.height || sourceAlpha <= 0)
        return;
    const int index = y * slot.width + x;
    if (sourceAlpha >= 255 || slot.alpha[index] == 0)
    {
        slot.color[index] = sourceColor;
        slot.alpha[index] = static_cast<unsigned char>(ClampByte(sourceAlpha));
        return;
    }

    const int destinationAlpha = slot.alpha[index];
    const int inverse = 255 - sourceAlpha;
    const int destinationContribution = Divide255Round(destinationAlpha * inverse);
    int outputAlpha = sourceAlpha + destinationContribution;
    if (outputAlpha > 255)
        outputAlpha = 255;
    if (outputAlpha <= 0)
        return;

    const int sourceR = (sourceColor >> 11) & 31;
    const int sourceG = (sourceColor >> 5) & 63;
    const int sourceB = sourceColor & 31;
    const int destinationR = (slot.color[index] >> 11) & 31;
    const int destinationG = (slot.color[index] >> 5) & 63;
    const int destinationB = slot.color[index] & 31;
    const int outputR = (sourceR * sourceAlpha + destinationR * destinationContribution + outputAlpha / 2) / outputAlpha;
    const int outputG = (sourceG * sourceAlpha + destinationG * destinationContribution + outputAlpha / 2) / outputAlpha;
    const int outputB = (sourceB * sourceAlpha + destinationB * destinationContribution + outputAlpha / 2) / outputAlpha;
    slot.color[index] = static_cast<unsigned short>((outputR << 11) | (outputG << 5) | outputB);
    slot.alpha[index] = static_cast<unsigned char>(outputAlpha);
}

void DrawResolvedGlyph(TBitmapSlot& slot,
                       const TResolvedGlyph& glyph,
                       int x,
                       int y,
                       const EasyRpgRgssTextStyle& style,
                       unsigned short color,
                       int alpha)
{
    const int targetHeight = style.font_size;
    const int targetWidth = ScaleMetric(glyph.width, glyph.height, targetHeight);
    const int boldOffset = style.bold ? MaxInt(1, targetHeight / 11) : 0;
    for (int dy = 0; dy < targetHeight; ++dy)
    {
        int sourceY = (dy * glyph.height) / targetHeight;
        if (sourceY >= glyph.height)
            sourceY = glyph.height - 1;
        const int italicOffset = style.italic
            ? ((targetHeight - 1 - dy) * MaxInt(1, targetHeight / 11)) / 4
            : 0;
        for (int dx = 0; dx < targetWidth; ++dx)
        {
            int sourceX = (dx * glyph.width) / targetWidth;
            if (sourceX >= glyph.width)
                sourceX = glyph.width - 1;
            if (!GlyphBit(glyph, sourceX, sourceY))
                continue;
            CompositeBitmapPixel(slot, x + dx + italicOffset, y + dy, color, alpha);
            for (int bold = 1; bold <= boldOffset; ++bold)
                CompositeBitmapPixel(slot, x + dx + italicOffset + bold, y + dy, color, alpha);
        }
    }
}

int MeasureText(const unsigned int* codepoints,
                int count,
                const EasyRpgRgssTextStyle& style,
                int& width,
                int& height)
{
    width = 0;
    height = style.font_size;
    if (count <= 0)
        return ERgssBitmapOk;
    if (!codepoints)
        return ERgssBitmapErrorInvalidArgument;

    for (int index = 0; index < count; ++index)
    {
        if (codepoints[index] == '\r' || codepoints[index] == '\n')
            continue;
        TResolvedGlyph glyph;
        if (!ResolveGlyph(codepoints[index], glyph))
            continue;
        width += ScaleMetric(glyph.advance, glyph.height, style.font_size);
        if (width < 0)
            return ERgssBitmapErrorInvalidArgument;
    }
    if (style.bold)
        width += MaxInt(1, style.font_size / 11);
    if (style.italic)
        width += MaxInt(1, style.font_size / 11) * style.font_size / 4;
    return ERgssBitmapOk;
}

int BitmapToBitmap(TBitmapSlot& destination,
                   const EasyRpgRgssRect& destinationRect,
                   TBitmapSlot& source,
                   const EasyRpgRgssRect& sourceRect,
                   int opacity)
{
    if (destinationRect.width < 0 || destinationRect.height < 0 ||
        sourceRect.width < 0 || sourceRect.height < 0 ||
        sourceRect.x < 0 || sourceRect.y < 0 ||
        sourceRect.x + sourceRect.width > source.width ||
        sourceRect.y + sourceRect.height > source.height)
        return ERgssBitmapErrorInvalidArgument;
    if (destinationRect.width == 0 || destinationRect.height == 0 ||
        sourceRect.width == 0 || sourceRect.height == 0 || opacity <= 0)
        return ERgssBitmapOk;

    unsigned short* copiedColor = 0;
    unsigned char* copiedAlpha = 0;
    if (&destination == &source)
    {
        const unsigned int sourceWidth = static_cast<unsigned int>(sourceRect.width);
        const unsigned int sourceHeight = static_cast<unsigned int>(sourceRect.height);
        if (sourceWidth > 0xffffffffu / sourceHeight)
            return ERgssBitmapErrorNoMemory;
        const unsigned int pixels = sourceWidth * sourceHeight;
        if (pixels > 0xffffffffu / 2u)
            return ERgssBitmapErrorNoMemory;
        copiedColor = static_cast<unsigned short*>(
            gBitmaps.host.allocate(gBitmaps.host.context, pixels * 2u));
        if (!copiedColor)
            return ERgssBitmapErrorNoMemory;
        copiedAlpha = static_cast<unsigned char*>(
            gBitmaps.host.allocate(gBitmaps.host.context, pixels));
        if (!copiedAlpha)
        {
            gBitmaps.host.release(gBitmaps.host.context, copiedColor);
            return ERgssBitmapErrorNoMemory;
        }
        for (int y = 0; y < sourceRect.height; ++y)
        {
            const int sourceBase = (sourceRect.y + y) * source.width + sourceRect.x;
            const int copyBase = y * sourceRect.width;
            for (int x = 0; x < sourceRect.width; ++x)
            {
                copiedColor[copyBase + x] = source.color[sourceBase + x];
                copiedAlpha[copyBase + x] = source.alpha[sourceBase + x];
            }
        }
    }

    const int globalOpacity = ClampByte(opacity);
    int left = destinationRect.x < 0 ? 0 : destinationRect.x;
    int top = destinationRect.y < 0 ? 0 : destinationRect.y;
    int right = destinationRect.x + destinationRect.width;
    int bottom = destinationRect.y + destinationRect.height;
    if (right > destination.width)
        right = destination.width;
    if (bottom > destination.height)
        bottom = destination.height;

    if (right > left && bottom > top)
    {
        for (int y = top; y < bottom; ++y)
        {
            const int relativeY = y - destinationRect.y;
            int sourceOffsetY = (relativeY * sourceRect.height) / destinationRect.height;
            if (sourceOffsetY >= sourceRect.height)
                sourceOffsetY = sourceRect.height - 1;
            for (int x = left; x < right; ++x)
            {
                const int relativeX = x - destinationRect.x;
                int sourceOffsetX = (relativeX * sourceRect.width) / destinationRect.width;
                if (sourceOffsetX >= sourceRect.width)
                    sourceOffsetX = sourceRect.width - 1;
                unsigned short sourceColor = 0;
                int sourceAlpha = 0;
                if (copiedColor)
                {
                    const int sourceIndex = sourceOffsetY * sourceRect.width + sourceOffsetX;
                    sourceColor = copiedColor[sourceIndex];
                    sourceAlpha = copiedAlpha[sourceIndex];
                }
                else
                {
                    const int sourceIndex = (sourceRect.y + sourceOffsetY) * source.width +
                                            sourceRect.x + sourceOffsetX;
                    sourceColor = source.color[sourceIndex];
                    sourceAlpha = source.alpha[sourceIndex];
                }
                const int effectiveAlpha = Divide255Round(sourceAlpha * globalOpacity);
                CompositeBitmapPixel(destination,
                                     x,
                                     y,
                                     sourceColor,
                                     effectiveAlpha);
            }
        }
    }

    if (copiedAlpha)
        gBitmaps.host.release(gBitmaps.host.context, copiedAlpha);
    if (copiedColor)
        gBitmaps.host.release(gBitmaps.host.context, copiedColor);
    return ERgssBitmapOk;
}

void RotateHue(unsigned short& color, int hueDegrees)
{
    int red = 0;
    int green = 0;
    int blue = 0;
    Unpack565(color, red, green, blue);
    const int maximum = MaxInt(red, MaxInt(green, blue));
    int minimum = red < green ? red : green;
    if (blue < minimum)
        minimum = blue;
    const int delta = maximum - minimum;
    if (delta <= 0)
        return;

    int hue = 0;
    if (maximum == red)
        hue = DivideInt((green - blue) * 256, delta);
    else if (maximum == green)
        hue = 512 + DivideInt((blue - red) * 256, delta);
    else
        hue = 1024 + DivideInt((red - green) * 256, delta);
    if (hue < 0)
        hue += 1536;
    if (hue >= 1536)
        hue -= 1536;

    const int shift = DivideInt(hueDegrees * 1536, 360);
    hue += shift;
    if (hue >= 1536)
        hue -= 1536;

    const int sector = hue >> 8;
    const int fraction = hue & 255;
    const int rising = minimum + DivideInt(delta * fraction, 255);
    const int falling = maximum - DivideInt(delta * fraction, 255);
    switch (sector)
    {
    case 0:
        red = maximum;
        green = rising;
        blue = minimum;
        break;
    case 1:
        red = falling;
        green = maximum;
        blue = minimum;
        break;
    case 2:
        red = minimum;
        green = maximum;
        blue = rising;
        break;
    case 3:
        red = minimum;
        green = falling;
        blue = maximum;
        break;
    case 4:
        red = rising;
        green = minimum;
        blue = maximum;
        break;
    default:
        red = maximum;
        green = minimum;
        blue = falling;
        break;
    }
    color = Pack565(red, green, blue);
}

unsigned short ApplyBlitEffects(unsigned short color,
                                const EasyRpgRgssBlitEffects& effects)
{
    int red = 0;
    int green = 0;
    int blue = 0;
    Unpack565(color, red, green, blue);
    if (effects.tone_gray > 0)
    {
        const int gray = (red * 77 + green * 150 + blue * 29) >> 8;
        red += Divide255SignedRound((gray - red) * effects.tone_gray);
        green += Divide255SignedRound((gray - green) * effects.tone_gray);
        blue += Divide255SignedRound((gray - blue) * effects.tone_gray);
    }
    red = ClampByte(red + effects.tone_red);
    green = ClampByte(green + effects.tone_green);
    blue = ClampByte(blue + effects.tone_blue);
    if (effects.color_alpha > 0)
    {
        red += Divide255SignedRound((effects.color_red - red) * effects.color_alpha);
        green += Divide255SignedRound((effects.color_green - green) * effects.color_alpha);
        blue += Divide255SignedRound((effects.color_blue - blue) * effects.color_alpha);
        red = ClampByte(red);
        green = ClampByte(green);
        blue = ClampByte(blue);
    }
    return Pack565(red, green, blue);
}

int ValidateBlitEffects(const EasyRpgRgssBlitEffects* effects)
{
    if (!effects)
        return ERgssBitmapOk;
    if (effects->tone_red < -255 || effects->tone_red > 255 ||
        effects->tone_green < -255 || effects->tone_green > 255 ||
        effects->tone_blue < -255 || effects->tone_blue > 255 ||
        effects->tone_gray < 0 || effects->tone_gray > 255 ||
        effects->color_red < 0 || effects->color_red > 255 ||
        effects->color_green < 0 || effects->color_green > 255 ||
        effects->color_blue < 0 || effects->color_blue > 255 ||
        effects->color_alpha < 0 || effects->color_alpha > 255 ||
        effects->bush_depth < 0 || effects->bush_opacity < 0 ||
        effects->bush_opacity > 255)
        return ERgssBitmapErrorInvalidArgument;
    return ERgssBitmapOk;
}
}

extern "C" int EasyRpgRgssBitmapInitialize(const EasyRpgRgssBitmapHost* host)
{
    if (!host || !host->allocate || !host->release ||
        host->max_bitmaps <= 0 || host->max_bitmaps > 65535)
        return ERgssBitmapErrorInvalidArgument;

    EasyRpgRgssBitmapShutdown();
    const unsigned int bytes = static_cast<unsigned int>(host->max_bitmaps) *
                               static_cast<unsigned int>(sizeof(TBitmapSlot));
    TBitmapSlot* slots = static_cast<TBitmapSlot*>(host->allocate(host->context, bytes));
    if (!slots)
        return ERgssBitmapErrorNoMemory;

    gBitmaps.host = *host;
    gBitmaps.slots = slots;
    gBitmaps.glyph_context = host->context;
    gBitmaps.initialized = 1;
    for (int index = 0; index < host->max_bitmaps; ++index)
    {
        slots[index].color = 0;
        slots[index].alpha = 0;
        slots[index].width = 0;
        slots[index].height = 0;
        slots[index].generation = 0;
        slots[index].used = 0;
    }
    return ERgssBitmapOk;
}

extern "C" void EasyRpgRgssBitmapShutdown(void)
{
    if (!gBitmaps.initialized)
        return;
    for (int index = 0; index < gBitmaps.host.max_bitmaps; ++index)
    {
        if (gBitmaps.slots[index].used)
            ReleaseSlot(gBitmaps.slots[index]);
    }
    gBitmaps.host.release(gBitmaps.host.context, gBitmaps.slots);
    gBitmaps.slots = 0;
    gBitmaps.host.context = 0;
    gBitmaps.host.allocate = 0;
    gBitmaps.host.release = 0;
    gBitmaps.host.glyph = 0;
    gBitmaps.host.max_bitmaps = 0;
    gBitmaps.glyph_context = 0;
    gBitmaps.initialized = 0;
}

extern "C" void EasyRpgRgssBitmapSetGlyphProvider(void* context,
                                                    EasyRpgRgssGlyphFn glyph)
{
    if (!gBitmaps.initialized)
        return;
    gBitmaps.glyph_context = context;
    gBitmaps.host.glyph = glyph;
}

extern "C" int EasyRpgRgssBitmapCreate(int width, int height, int* handle)
{
    if (!gBitmaps.initialized)
        return ERgssBitmapErrorNotInitialized;
    if (!handle || width <= 0 || height <= 0 || width > 32767 || height > 32767)
        return ERgssBitmapErrorInvalidArgument;

    const unsigned int w = static_cast<unsigned int>(width);
    const unsigned int h = static_cast<unsigned int>(height);
    if (w > 0xffffffffu / h)
        return ERgssBitmapErrorNoMemory;
    const unsigned int pixels = w * h;
    if (pixels > 0xffffffffu / 2u)
        return ERgssBitmapErrorNoMemory;

    int slotIndex = -1;
    for (int index = 0; index < gBitmaps.host.max_bitmaps; ++index)
    {
        if (!gBitmaps.slots[index].used)
        {
            slotIndex = index;
            break;
        }
    }
    if (slotIndex < 0)
        return ERgssBitmapErrorCapacity;

    unsigned short* color = static_cast<unsigned short*>(
        gBitmaps.host.allocate(gBitmaps.host.context, pixels * 2u));
    if (!color)
        return ERgssBitmapErrorNoMemory;
    unsigned char* alpha = static_cast<unsigned char*>(
        gBitmaps.host.allocate(gBitmaps.host.context, pixels));
    if (!alpha)
    {
        gBitmaps.host.release(gBitmaps.host.context, color);
        return ERgssBitmapErrorNoMemory;
    }

    for (unsigned int pixel = 0; pixel < pixels; ++pixel)
    {
        color[pixel] = 0;
        alpha[pixel] = 0;
    }

    TBitmapSlot& slot = gBitmaps.slots[slotIndex];
    unsigned short generation = static_cast<unsigned short>((slot.generation + 1u) & 0x7fffu);
    if (generation == 0)
        generation = 1;
    slot.color = color;
    slot.alpha = alpha;
    slot.width = width;
    slot.height = height;
    slot.generation = generation;
    slot.used = 1;
    *handle = MakeHandle(slotIndex, generation);
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapDestroy(int handle)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return gBitmaps.initialized ? ERgssBitmapErrorInvalidHandle : ERgssBitmapErrorNotInitialized;
    ReleaseSlot(*slot);
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapWidth(int handle)
{
    TBitmapSlot* slot = FindSlot(handle);
    return slot ? slot->width : ERgssBitmapErrorInvalidHandle;
}

extern "C" int EasyRpgRgssBitmapHeight(int handle)
{
    TBitmapSlot* slot = FindSlot(handle);
    return slot ? slot->height : ERgssBitmapErrorInvalidHandle;
}

extern "C" int EasyRpgRgssBitmapClear(int handle)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    const unsigned int pixels = static_cast<unsigned int>(slot->width) *
                                static_cast<unsigned int>(slot->height);
    for (unsigned int pixel = 0; pixel < pixels; ++pixel)
    {
        slot->color[pixel] = 0;
        slot->alpha[pixel] = 0;
    }
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapFillRect(int handle,
                                          int x,
                                          int y,
                                          int width,
                                          int height,
                                          int red,
                                          int green,
                                          int blue,
                                          int alpha)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (width < 0 || height < 0)
        return ERgssBitmapErrorInvalidArgument;
    if (width == 0 || height == 0)
        return ERgssBitmapOk;

    int left = x < 0 ? 0 : x;
    int top = y < 0 ? 0 : y;
    int right = x + width;
    int bottom = y + height;
    if (right > slot->width)
        right = slot->width;
    if (bottom > slot->height)
        bottom = slot->height;
    if (right <= left || bottom <= top)
        return ERgssBitmapOk;

    const unsigned short color = Pack565(red, green, blue);
    const unsigned char opacity = static_cast<unsigned char>(ClampByte(alpha));
    for (int row = top; row < bottom; ++row)
    {
        const int base = row * slot->width;
        for (int column = left; column < right; ++column)
        {
            slot->color[base + column] = color;
            slot->alpha[base + column] = opacity;
        }
    }
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapSetPixel(int handle,
                                          int x,
                                          int y,
                                          int red,
                                          int green,
                                          int blue,
                                          int alpha)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (x < 0 || y < 0 || x >= slot->width || y >= slot->height)
        return ERgssBitmapOk;
    const int index = y * slot->width + x;
    slot->color[index] = Pack565(red, green, blue);
    slot->alpha[index] = static_cast<unsigned char>(ClampByte(alpha));
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapBlt(int destination_handle,
                                     int destination_x,
                                     int destination_y,
                                     int source_handle,
                                     const EasyRpgRgssRect* source_rect,
                                     int opacity)
{
    TBitmapSlot* destination = FindSlot(destination_handle);
    TBitmapSlot* source = FindSlot(source_handle);
    if (!destination || !source)
        return ERgssBitmapErrorInvalidHandle;
    if (!source_rect)
        return ERgssBitmapErrorInvalidArgument;
    EasyRpgRgssRect destinationRect;
    destinationRect.x = destination_x;
    destinationRect.y = destination_y;
    destinationRect.width = source_rect->width;
    destinationRect.height = source_rect->height;
    return BitmapToBitmap(*destination,
                          destinationRect,
                          *source,
                          *source_rect,
                          opacity);
}

extern "C" int EasyRpgRgssBitmapStretchBlt(int destination_handle,
                                            const EasyRpgRgssRect* destination_rect,
                                            int source_handle,
                                            const EasyRpgRgssRect* source_rect,
                                            int opacity)
{
    TBitmapSlot* destination = FindSlot(destination_handle);
    TBitmapSlot* source = FindSlot(source_handle);
    if (!destination || !source)
        return ERgssBitmapErrorInvalidHandle;
    if (!destination_rect || !source_rect)
        return ERgssBitmapErrorInvalidArgument;
    return BitmapToBitmap(*destination,
                          *destination_rect,
                          *source,
                          *source_rect,
                          opacity);
}

extern "C" int EasyRpgRgssBitmapHueChange(int handle, int hue_degrees)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    int normalized = hue_degrees % 360;
    if (normalized < 0)
        normalized += 360;
    if (normalized == 0)
        return ERgssBitmapOk;
    const unsigned int pixels = static_cast<unsigned int>(slot->width) *
                                static_cast<unsigned int>(slot->height);
    for (unsigned int index = 0; index < pixels; ++index)
    {
        if (slot->alpha[index] != 0)
            RotateHue(slot->color[index], normalized);
    }
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapGetPixel(int handle,
                                          int x,
                                          int y,
                                          int* red,
                                          int* green,
                                          int* blue,
                                          int* alpha)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!red || !green || !blue || !alpha)
        return ERgssBitmapErrorInvalidArgument;
    if (x < 0 || y < 0 || x >= slot->width || y >= slot->height)
    {
        *red = 0;
        *green = 0;
        *blue = 0;
        *alpha = 0;
        return ERgssBitmapOk;
    }
    const int index = y * slot->width + x;
    Unpack565(slot->color[index], *red, *green, *blue);
    *alpha = slot->alpha[index];
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapLock(int handle,
                                      unsigned short** rgb565,
                                      unsigned char** alpha,
                                      int* pitch_pixels)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!rgb565 || !alpha || !pitch_pixels)
        return ERgssBitmapErrorInvalidArgument;
    *rgb565 = slot->color;
    *alpha = slot->alpha;
    *pitch_pixels = slot->width;
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapBlit(int handle,
                                      const EasyRpgRgssRect* source_rect,
                                      int destination_x,
                                      int destination_y,
                                      int opacity,
                                      EasyRpgRgssRect* output_dirty)
{
    ZeroDirty(output_dirty);
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!source_rect || source_rect->width < 0 || source_rect->height < 0)
        return ERgssBitmapErrorInvalidArgument;
    if (source_rect->width == 0 || source_rect->height == 0 || opacity <= 0)
        return ERgssBitmapOk;

    int sourceX = source_rect->x;
    int sourceY = source_rect->y;
    int sourceWidth = source_rect->width;
    int sourceHeight = source_rect->height;
    if (sourceX < 0)
    {
        const int skip = -sourceX;
        sourceX = 0;
        sourceWidth -= skip;
        destination_x += skip;
    }
    if (sourceY < 0)
    {
        const int skip = -sourceY;
        sourceY = 0;
        sourceHeight -= skip;
        destination_y += skip;
    }
    if (sourceX + sourceWidth > slot->width)
        sourceWidth = slot->width - sourceX;
    if (sourceY + sourceHeight > slot->height)
        sourceHeight = slot->height - sourceY;
    if (sourceWidth <= 0 || sourceHeight <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
        return ERgssBitmapErrorFramebuffer;

    int logicalX = destination_x;
    int logicalY = destination_y;
    int logicalWidth = sourceWidth;
    int logicalHeight = sourceHeight;
    if (logicalX < 0)
    {
        const int skip = -logicalX;
        logicalX = 0;
        logicalWidth -= skip;
        sourceX += skip;
    }
    if (logicalY < 0)
    {
        const int skip = -logicalY;
        logicalY = 0;
        logicalHeight -= skip;
        sourceY += skip;
    }
    if (logicalX + logicalWidth > config.logical_width)
        logicalWidth = config.logical_width - logicalX;
    if (logicalY + logicalHeight > config.logical_height)
        logicalHeight = config.logical_height - logicalY;
    if (logicalWidth <= 0 || logicalHeight <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssRect logicalRect;
    logicalRect.x = logicalX;
    logicalRect.y = logicalY;
    logicalRect.width = logicalWidth;
    logicalRect.height = logicalHeight;
    EasyRpgRgssRect physicalRect;
    if (EasyRpgRgssCoreMapRect(&logicalRect, &physicalRect) != ERgssOk ||
        physicalRect.width <= 0 || physicalRect.height <= 0)
        return ERgssBitmapErrorFramebuffer;

    void* framebuffer = 0;
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    int framebufferPitch = 0;
    int framebufferFormat = 0;
    if (EasyRpgRgssCoreGetFramebuffer(&framebuffer,
                                       &framebufferWidth,
                                       &framebufferHeight,
                                       &framebufferPitch,
                                       &framebufferFormat) != ERgssOk ||
        framebufferFormat != ERgssPixelRgb565)
        return ERgssBitmapErrorFramebuffer;

    const int globalOpacity = ClampByte(opacity);
    int sourceXLookup[KFastBlitWidth];
    const int useSourceXLookup = physicalRect.width <= KFastBlitWidth;
    if (useSourceXLookup)
    {
        for (int px = 0; px < physicalRect.width; ++px)
            sourceXLookup[px] = sourceX + (px * logicalWidth) / physicalRect.width;
    }

    unsigned char* targetBytes = static_cast<unsigned char*>(framebuffer);
    for (int py = 0; py < physicalRect.height; ++py)
    {
        const int sourceOffsetY = (py * logicalHeight) / physicalRect.height;
        const int sy = sourceY + sourceOffsetY;
        unsigned short* target = reinterpret_cast<unsigned short*>(
            targetBytes + (physicalRect.y + py) * framebufferPitch);
        for (int px = 0; px < physicalRect.width; ++px)
        {
            const int sx = useSourceXLookup
                ? sourceXLookup[px]
                : sourceX + (px * logicalWidth) / physicalRect.width;
            const int sourceIndex = sy * slot->width + sx;
            const int sourceAlpha = slot->alpha[sourceIndex];
            const int effectiveAlpha = globalOpacity == 255
                ? sourceAlpha
                : Divide255Round(sourceAlpha * globalOpacity);
            const int targetX = physicalRect.x + px;
            if (targetX < 0 || targetX >= framebufferWidth ||
                physicalRect.y + py < 0 || physicalRect.y + py >= framebufferHeight)
                continue;
            target[targetX] = Blend565(target[targetX],
                                       slot->color[sourceIndex],
                                       effectiveAlpha);
        }
    }

    if (output_dirty)
        *output_dirty = physicalRect;
    return ERgssBitmapOk;
}

static int BitmapBlitEffectsInternal(int handle,
                                     const EasyRpgRgssRect* source_rect,
                                     const EasyRpgRgssRect* destination_rect,
                                     const EasyRpgRgssRect* clip_rect,
                                     int opacity,
                                     int mirror,
                                     int blend_type,
                                     const EasyRpgRgssBlitEffects* effects,
                                     EasyRpgRgssRect* output_dirty)
{
    ZeroDirty(output_dirty);
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!source_rect || !destination_rect ||
        source_rect->width < 0 || source_rect->height < 0 ||
        destination_rect->width < 0 || destination_rect->height < 0 ||
        (clip_rect && (clip_rect->width < 0 || clip_rect->height < 0)) ||
        blend_type < ERgssBitmapBlendNormal ||
        blend_type > ERgssBitmapBlendSubtract)
        return ERgssBitmapErrorInvalidArgument;
    const int effectsResult = ValidateBlitEffects(effects);
    if (effectsResult != ERgssBitmapOk)
        return effectsResult;
    if (source_rect->width == 0 || source_rect->height == 0 ||
        destination_rect->width == 0 || destination_rect->height == 0 ||
        opacity <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
        return ERgssBitmapErrorFramebuffer;

    EasyRpgRgssRect clippedDestination = *destination_rect;
    if (clippedDestination.x < 0)
    {
        clippedDestination.width += clippedDestination.x;
        clippedDestination.x = 0;
    }
    if (clippedDestination.y < 0)
    {
        clippedDestination.height += clippedDestination.y;
        clippedDestination.y = 0;
    }
    if (clippedDestination.x + clippedDestination.width > config.logical_width)
        clippedDestination.width = config.logical_width - clippedDestination.x;
    if (clippedDestination.y + clippedDestination.height > config.logical_height)
        clippedDestination.height = config.logical_height - clippedDestination.y;
    if (clip_rect)
    {
        const int left = clippedDestination.x > clip_rect->x
            ? clippedDestination.x : clip_rect->x;
        const int top = clippedDestination.y > clip_rect->y
            ? clippedDestination.y : clip_rect->y;
        const int destinationRight = clippedDestination.x + clippedDestination.width;
        const int destinationBottom = clippedDestination.y + clippedDestination.height;
        const int clipRight = clip_rect->x + clip_rect->width;
        const int clipBottom = clip_rect->y + clip_rect->height;
        const int right = destinationRight < clipRight ? destinationRight : clipRight;
        const int bottom = destinationBottom < clipBottom ? destinationBottom : clipBottom;
        clippedDestination.x = left;
        clippedDestination.y = top;
        clippedDestination.width = right - left;
        clippedDestination.height = bottom - top;
    }
    if (clippedDestination.width <= 0 || clippedDestination.height <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssRect physicalRect;
    if (EasyRpgRgssCoreMapRect(&clippedDestination, &physicalRect) != ERgssOk ||
        physicalRect.width <= 0 || physicalRect.height <= 0)
        return ERgssBitmapErrorFramebuffer;

    void* framebuffer = 0;
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    int framebufferPitch = 0;
    int framebufferFormat = 0;
    if (EasyRpgRgssCoreGetFramebuffer(&framebuffer,
                                       &framebufferWidth,
                                       &framebufferHeight,
                                       &framebufferPitch,
                                       &framebufferFormat) != ERgssOk ||
        framebufferFormat != ERgssPixelRgb565)
        return ERgssBitmapErrorFramebuffer;

    const int globalOpacity = ClampByte(opacity);
    int sourceXLookup[KFastBlitWidth];
    const int useSourceXLookup = physicalRect.width <= KFastBlitWidth;
    if (useSourceXLookup)
    {
        for (int px = 0; px < physicalRect.width; ++px)
        {
            const int logicalX = clippedDestination.x +
                (px * clippedDestination.width) / physicalRect.width;
            const int relativeX = logicalX - destination_rect->x;
            int sourceOffsetX =
                (relativeX * source_rect->width) / destination_rect->width;
            if (mirror)
                sourceOffsetX = source_rect->width - 1 - sourceOffsetX;
            sourceXLookup[px] = source_rect->x + sourceOffsetX;
        }
    }

    unsigned char* targetBytes = static_cast<unsigned char*>(framebuffer);
    for (int py = 0; py < physicalRect.height; ++py)
    {
        const int logicalY = clippedDestination.y +
            (py * clippedDestination.height) / physicalRect.height;
        const int relativeY = logicalY - destination_rect->y;
        const int sourceOffsetY =
            (relativeY * source_rect->height) / destination_rect->height;
        const int sourceY = source_rect->y + sourceOffsetY;
        const int targetY = physicalRect.y + py;
        if (sourceY < 0 || sourceY >= slot->height ||
            targetY < 0 || targetY >= framebufferHeight)
            continue;
        unsigned short* target = reinterpret_cast<unsigned short*>(
            targetBytes + targetY * framebufferPitch);
        for (int px = 0; px < physicalRect.width; ++px)
        {
            int sourceX = 0;
            if (useSourceXLookup)
            {
                sourceX = sourceXLookup[px];
            }
            else
            {
                const int logicalX = clippedDestination.x +
                    (px * clippedDestination.width) / physicalRect.width;
                const int relativeX = logicalX - destination_rect->x;
                int sourceOffsetX =
                    (relativeX * source_rect->width) / destination_rect->width;
                if (mirror)
                    sourceOffsetX = source_rect->width - 1 - sourceOffsetX;
                sourceX = source_rect->x + sourceOffsetX;
            }
            const int targetX = physicalRect.x + px;
            if (sourceX < 0 || sourceX >= slot->width ||
                targetX < 0 || targetX >= framebufferWidth)
                continue;
            const int sourceIndex = sourceY * slot->width + sourceX;
            const int sourceAlpha = slot->alpha[sourceIndex];
            int effectiveAlpha = globalOpacity == 255
                ? sourceAlpha
                : Divide255Round(sourceAlpha * globalOpacity);
            unsigned short sourceColor = slot->color[sourceIndex];
            if (effects)
            {
                sourceColor = ApplyBlitEffects(sourceColor, *effects);
                if (effects->bush_depth > 0 &&
                    relativeY >= destination_rect->height - effects->bush_depth)
                    effectiveAlpha = Divide255Round(effectiveAlpha * effects->bush_opacity);
            }
            target[targetX] = Composite565(target[targetX],
                                           sourceColor,
                                           effectiveAlpha,
                                           blend_type);
        }
    }

    if (output_dirty)
        *output_dirty = physicalRect;
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapBlitExClipped(int handle,
                                               const EasyRpgRgssRect* source_rect,
                                               const EasyRpgRgssRect* destination_rect,
                                               const EasyRpgRgssRect* clip_rect,
                                               int opacity,
                                               int mirror,
                                               int blend_type,
                                               EasyRpgRgssRect* output_dirty)
{
    return BitmapBlitEffectsInternal(handle,
                                     source_rect,
                                     destination_rect,
                                     clip_rect,
                                     opacity,
                                     mirror,
                                     blend_type,
                                     0,
                                     output_dirty);
}

extern "C" int EasyRpgRgssBitmapBlitEffects(int handle,
                                             const EasyRpgRgssRect* source_rect,
                                             const EasyRpgRgssRect* destination_rect,
                                             const EasyRpgRgssRect* clip_rect,
                                             int opacity,
                                             int mirror,
                                             int blend_type,
                                             const EasyRpgRgssBlitEffects* effects,
                                             EasyRpgRgssRect* output_dirty)
{
    return BitmapBlitEffectsInternal(handle,
                                     source_rect,
                                     destination_rect,
                                     clip_rect,
                                     opacity,
                                     mirror,
                                     blend_type,
                                     effects,
                                     output_dirty);
}

extern "C" int EasyRpgRgssBitmapBlitEx(int handle,
                                        const EasyRpgRgssRect* source_rect,
                                        const EasyRpgRgssRect* destination_rect,
                                        int opacity,
                                        int mirror,
                                        int blend_type,
                                        EasyRpgRgssRect* output_dirty)
{
    return EasyRpgRgssBitmapBlitExClipped(handle,
                                          source_rect,
                                          destination_rect,
                                          0,
                                          opacity,
                                          mirror,
                                          blend_type,
                                          output_dirty);
}

extern "C" int EasyRpgRgssBitmapTile(int handle,
                                      const EasyRpgRgssRect* source_rect,
                                      const EasyRpgRgssRect* destination_rect,
                                      int offset_x,
                                      int offset_y,
                                      int zoom_x_milli,
                                      int zoom_y_milli,
                                      int opacity,
                                      int blend_type,
                                      EasyRpgRgssRect* output_dirty)
{
    ZeroDirty(output_dirty);
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!source_rect || !destination_rect ||
        source_rect->x < 0 || source_rect->y < 0 ||
        source_rect->width <= 0 || source_rect->height <= 0 ||
        source_rect->x + source_rect->width > slot->width ||
        source_rect->y + source_rect->height > slot->height ||
        destination_rect->width < 0 || destination_rect->height < 0 ||
        zoom_x_milli <= 0 || zoom_y_milli <= 0 ||
        zoom_x_milli > 16000 || zoom_y_milli > 16000 ||
        blend_type < ERgssBitmapBlendNormal ||
        blend_type > ERgssBitmapBlendSubtract)
        return ERgssBitmapErrorInvalidArgument;
    if (destination_rect->width == 0 || destination_rect->height == 0 || opacity <= 0)
        return ERgssBitmapOk;
    if (source_rect->width > (2147483647 - 999) / zoom_x_milli ||
        source_rect->height > (2147483647 - 999) / zoom_y_milli)
        return ERgssBitmapErrorInvalidArgument;

    const int periodX = DivideInt(source_rect->width * zoom_x_milli + 999, 1000);
    const int periodY = DivideInt(source_rect->height * zoom_y_milli + 999, 1000);
    if (periodX <= 0 || periodY <= 0)
        return ERgssBitmapOk;
    int normalizedOffsetX = offset_x % periodX;
    int normalizedOffsetY = offset_y % periodY;
    if (normalizedOffsetX < 0)
        normalizedOffsetX += periodX;
    if (normalizedOffsetY < 0)
        normalizedOffsetY += periodY;

    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
        return ERgssBitmapErrorFramebuffer;

    EasyRpgRgssRect clippedDestination = *destination_rect;
    if (clippedDestination.x < 0)
    {
        clippedDestination.width += clippedDestination.x;
        clippedDestination.x = 0;
    }
    if (clippedDestination.y < 0)
    {
        clippedDestination.height += clippedDestination.y;
        clippedDestination.y = 0;
    }
    if (clippedDestination.x + clippedDestination.width > config.logical_width)
        clippedDestination.width = config.logical_width - clippedDestination.x;
    if (clippedDestination.y + clippedDestination.height > config.logical_height)
        clippedDestination.height = config.logical_height - clippedDestination.y;
    if (clippedDestination.width <= 0 || clippedDestination.height <= 0)
        return ERgssBitmapOk;

    EasyRpgRgssRect physicalRect;
    if (EasyRpgRgssCoreMapRect(&clippedDestination, &physicalRect) != ERgssOk ||
        physicalRect.width <= 0 || physicalRect.height <= 0)
        return ERgssBitmapErrorFramebuffer;

    void* framebuffer = 0;
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    int framebufferPitch = 0;
    int framebufferFormat = 0;
    if (EasyRpgRgssCoreGetFramebuffer(&framebuffer,
                                       &framebufferWidth,
                                       &framebufferHeight,
                                       &framebufferPitch,
                                       &framebufferFormat) != ERgssOk ||
        framebufferFormat != ERgssPixelRgb565)
        return ERgssBitmapErrorFramebuffer;

    const int globalOpacity = ClampByte(opacity);
    int sourceXLookup[KFastBlitWidth];
    const int useSourceXLookup = physicalRect.width <= KFastBlitWidth;
    if (useSourceXLookup)
    {
        for (int px = 0; px < physicalRect.width; ++px)
        {
            const int logicalX = clippedDestination.x +
                (px * clippedDestination.width) / physicalRect.width;
            int tileX = (logicalX - destination_rect->x + normalizedOffsetX) % periodX;
            if (tileX < 0)
                tileX += periodX;
            int sourceOffsetX = (tileX * 1000) / zoom_x_milli;
            if (sourceOffsetX >= source_rect->width)
                sourceOffsetX = source_rect->width - 1;
            sourceXLookup[px] = source_rect->x + sourceOffsetX;
        }
    }

    unsigned char* targetBytes = static_cast<unsigned char*>(framebuffer);
    for (int py = 0; py < physicalRect.height; ++py)
    {
        const int logicalY = clippedDestination.y +
            (py * clippedDestination.height) / physicalRect.height;
        int tileY = (logicalY - destination_rect->y + normalizedOffsetY) % periodY;
        if (tileY < 0)
            tileY += periodY;
        int sourceOffsetY = (tileY * 1000) / zoom_y_milli;
        if (sourceOffsetY >= source_rect->height)
            sourceOffsetY = source_rect->height - 1;
        const int sourceY = source_rect->y + sourceOffsetY;
        const int targetY = physicalRect.y + py;
        if (targetY < 0 || targetY >= framebufferHeight)
            continue;
        unsigned short* target = reinterpret_cast<unsigned short*>(
            targetBytes + targetY * framebufferPitch);
        for (int px = 0; px < physicalRect.width; ++px)
        {
            int sourceX = 0;
            if (useSourceXLookup)
            {
                sourceX = sourceXLookup[px];
            }
            else
            {
                const int logicalX = clippedDestination.x +
                    (px * clippedDestination.width) / physicalRect.width;
                int tileX = (logicalX - destination_rect->x + normalizedOffsetX) % periodX;
                if (tileX < 0)
                    tileX += periodX;
                int sourceOffsetX = (tileX * 1000) / zoom_x_milli;
                if (sourceOffsetX >= source_rect->width)
                    sourceOffsetX = source_rect->width - 1;
                sourceX = source_rect->x + sourceOffsetX;
            }
            const int targetX = physicalRect.x + px;
            if (targetX < 0 || targetX >= framebufferWidth)
                continue;
            const int sourceIndex = sourceY * slot->width + sourceX;
            const int sourceAlpha = slot->alpha[sourceIndex];
            const int effectiveAlpha = globalOpacity == 255
                ? sourceAlpha
                : Divide255Round(sourceAlpha * globalOpacity);
            target[targetX] = Composite565(target[targetX],
                                           slot->color[sourceIndex],
                                           effectiveAlpha,
                                           blend_type);
        }
    }

    if (output_dirty)
        *output_dirty = physicalRect;
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapTextSize(int handle,
                                          const unsigned int* codepoints,
                                          int count,
                                          const EasyRpgRgssTextStyle* style,
                                          int* width,
                                          int* height)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (count < 0 || !width || !height)
        return ERgssBitmapErrorInvalidArgument;
    const int styleResult = ValidateTextStyle(style);
    if (styleResult != ERgssBitmapOk)
        return styleResult;
    return MeasureText(codepoints, count, *style, *width, *height);
}

extern "C" int EasyRpgRgssBitmapDrawText(int handle,
                                          const EasyRpgRgssRect* rect,
                                          const unsigned int* codepoints,
                                          int count,
                                          const EasyRpgRgssTextStyle* style)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (!rect || rect->width < 0 || rect->height < 0 || count < 0 ||
        (count > 0 && !codepoints))
        return ERgssBitmapErrorInvalidArgument;
    const int styleResult = ValidateTextStyle(style);
    if (styleResult != ERgssBitmapOk)
        return styleResult;
    if (rect->width == 0 || rect->height == 0 || count == 0 || style->alpha == 0)
        return ERgssBitmapOk;

    int textWidth = 0;
    int textHeight = 0;
    const int measureResult = MeasureText(codepoints,
                                          count,
                                          *style,
                                          textWidth,
                                          textHeight);
    if (measureResult != ERgssBitmapOk)
        return measureResult;

    int penX = rect->x;
    if (style->align == 1)
        penX += (rect->width - textWidth) / 2;
    else if (style->align == 2)
        penX += rect->width - textWidth;
    const int penY = rect->y + (rect->height - textHeight) / 2;
    const unsigned short color = Pack565(style->red,
                                         style->green,
                                         style->blue);
    const unsigned short shadowColor = 0;
    const int shadowAlpha = Divide255Round(style->alpha * 128);

    for (int index = 0; index < count; ++index)
    {
        const unsigned int codepoint = codepoints[index];
        if (codepoint == '\r' || codepoint == '\n')
            continue;
        TResolvedGlyph glyph;
        if (!ResolveGlyph(codepoint, glyph))
            continue;
        if (style->shadow)
            DrawResolvedGlyph(*slot,
                              glyph,
                              penX + MaxInt(1, style->font_size / 11),
                              penY + MaxInt(1, style->font_size / 11),
                              *style,
                              shadowColor,
                              shadowAlpha);
        DrawResolvedGlyph(*slot,
                          glyph,
                          penX,
                          penY,
                          *style,
                          color,
                          style->alpha);
        penX += ScaleMetric(glyph.advance, glyph.height, style->font_size);
    }
    return ERgssBitmapOk;
}
