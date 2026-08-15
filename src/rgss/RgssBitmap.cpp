#include "GohuFont.h"
#include "RgssBitmapBridge.h"
#include "shinonome.h"

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

// ARM11 (Nokia E72) has no integer divide instruction: every '/' compiles to
// a __aeabi_idiv call. The blit/text hot paths below use these division-free
// helpers instead. Both produce bit-identical results to the plain '/' forms
// they replace (Euclidean quotient stepping / exact 24-bit reciprocals).

// Steps q = floor(n * rate / modulus) as n advances by one (Advance) or by
// delta (AdvanceBy). remainder always stays in [0, modulus) so the amortized
// cost per step is a compare plus one subtract per quotient increment.
struct TRatioStepper
{
    int quotient;
    int remainder;
    int rate;
    int modulus;
};

void StepperInit(TRatioStepper& stepper, int rate, int modulus)
{
    stepper.quotient = 0;
    stepper.remainder = 0;
    stepper.rate = rate;
    stepper.modulus = modulus;
}

void StepperSeed(TRatioStepper& stepper, int numerator)
{
    // Seeds the stepper so quotient = floor(numerator / modulus). Used when
    // the mapping starts at a non-zero base offset; one division per call.
    stepper.quotient = DivideInt(numerator, stepper.modulus);
    stepper.remainder = numerator - stepper.quotient * stepper.modulus;
}

void StepperAdvance(TRatioStepper& stepper)
{
    stepper.remainder += stepper.rate;
    while (stepper.remainder >= stepper.modulus)
    {
        stepper.remainder -= stepper.modulus;
        ++stepper.quotient;
    }
}

void StepperAdvanceBy(TRatioStepper& stepper, int delta)
{
    stepper.remainder += delta * stepper.rate;
    while (stepper.remainder >= stepper.modulus)
    {
        stepper.remainder -= stepper.modulus;
        ++stepper.quotient;
    }
}

// Steps m = value % period (value non-negative, growing monotonically) without
// division: the accumulator stays in [0, period) and each advance subtracts
// period at most (delta / period + 1) times.
struct TWrapAccumulator
{
    int value;
    int period;
};

void WrapInit(TWrapAccumulator& accumulator, int base, int period)
{
    accumulator.period = period;
    accumulator.value = base - DivideInt(base, period) * period;
    if (accumulator.value < 0)
        accumulator.value += period;
}

void WrapAdvance(TWrapAccumulator& accumulator, int delta)
{
    accumulator.value += delta;
    while (accumulator.value >= accumulator.period)
        accumulator.value -= accumulator.period;
}

// ceil(2^24 / a) for a in 1..255 (0 for a == 0). Dividing any numerator
// n < 65536 by a via DivSmall below is exactly floor(n / a): with
// m = ceil(2^24 / a) and e = m*a - 2^24 (0 <= e < a), the fast quotient
// floor(n*m / 2^24) = floor(n/a + n*e/(a*2^24)) equals floor(n/a) for all
// n < 2^16 because n*e < 65536*255 < 2^24.
int gAlphaReciprocal[256];
unsigned char gAlphaReciprocalReady = 0;

void EnsureAlphaReciprocal()
{
    if (gAlphaReciprocalReady)
        return;
    for (int alpha = 1; alpha < 256; ++alpha)
    {
        const int quotient = DivideInt(1 << 24, alpha);
        gAlphaReciprocal[alpha] =
            quotient + ((1 << 24) - quotient * alpha != 0 ? 1 : 0);
    }
    gAlphaReciprocal[0] = 0;
    gAlphaReciprocalReady = 1;
}

inline int DivSmall(unsigned int numerator, int reciprocal)
{
    // (numerator * ceil(2^24/a)) >> 24 split into two 32-bit multiplies:
    // numerator * reciprocal = (numerator * high) * 2^16 + numerator * low,
    // so the quotient is (((numerator * high) + (numerator * low >> 16)) >> 8).
    // numerator < 2^16 keeps every product inside 32 bits.
    const unsigned int low = numerator * static_cast<unsigned int>(reciprocal & 0xffffu);
    const unsigned int high = numerator * static_cast<unsigned int>(static_cast<unsigned int>(reciprocal) >> 16);
    return static_cast<int>((high + (low >> 16)) >> 8);
}

// Per-render-pass snapshot of the core config and framebuffer. The blit
// entry points below run once per tile/sprite command (hundreds of times per
// frame); fetching config + framebuffer through the core bridge each time
// costs two struct copies per call. EasyRpgRgssBitmapBeginFrame (invoked once
// per render pass from RgssRender) refreshes this snapshot so blits read it
// for free. Blits issued outside a render pass refresh it on demand.
struct TFrameSnapshot
{
    int valid;
    EasyRpgRgssConfig config;
    void* pixels;
    int width;
    int height;
    int pitch;
    int format;
};

TFrameSnapshot gFrameSnapshot;

int RefreshFrameSnapshot()
{
    EasyRpgRgssConfig config;
    if (EasyRpgRgssCoreGetConfig(&config) != ERgssOk)
    {
        gFrameSnapshot.valid = 0;
        return 0;
    }
    void* pixels = 0;
    int width = 0;
    int height = 0;
    int pitch = 0;
    int format = 0;
    if (EasyRpgRgssCoreGetFramebuffer(&pixels, &width, &height, &pitch, &format) != ERgssOk ||
        format != ERgssPixelRgb565)
    {
        gFrameSnapshot.valid = 0;
        return 0;
    }
    gFrameSnapshot.config = config;
    gFrameSnapshot.pixels = pixels;
    gFrameSnapshot.width = width;
    gFrameSnapshot.height = height;
    gFrameSnapshot.pitch = pitch;
    gFrameSnapshot.format = format;
    gFrameSnapshot.valid = 1;
    return 1;
}

const TFrameSnapshot* AcquireFrameSnapshot()
{
    if (!gFrameSnapshot.valid && !RefreshFrameSnapshot())
        return 0;
    return &gFrameSnapshot;
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

const ShinonomeGlyph* FindShinonomeGlyph(unsigned int codepoint)
{
    if (codepoint > 0xffffu)
        return 0;
    int first = 0;
    int last = 11217;
    while (first < last)
    {
        const int middle = first + (last - first) / 2;
        if (SHINONOME_GOTHIC[middle].code < codepoint)
            first = middle + 1;
        else
            last = middle;
    }
    return first < 11217 && SHINONOME_GOTHIC[first].code == codepoint
        ? &SHINONOME_GOTHIC[first]
        : 0;
}

int ResolveGlyph(unsigned int codepoint, TResolvedGlyph& glyph)
{
    glyph.rows8 = 0;
    glyph.rows16 = 0;
    glyph.width = 0;
    glyph.height = 0;
    glyph.advance = 0;

    // Keep RGSS text deterministic on S60. CFont::GetCharacterData uses a
    // platform-specific packed glyph format on 3rd FP2; treating that buffer
    // as scanline bits produced the large square/gibberish glyphs seen in XP.
    // Reuse the proven 2k font path first: Gohu for Latin/Vietnamese and
    // Shinonome Gothic for Japanese.
    const unsigned char* rows = EasyRpgGohuGlyphRows(codepoint);
    if (rows)
    {
        glyph.rows8 = rows;
        glyph.width = 6;
        glyph.height = 11;
        glyph.advance = 6;
        return 1;
    }

    const ShinonomeGlyph* shinonome = FindShinonomeGlyph(codepoint);
    if (shinonome)
    {
        glyph.rows16 = shinonome->data;
        glyph.width = shinonome->is_full ? 12 : 6;
        glyph.height = 12;
        glyph.advance = glyph.width;
        return 1;
    }

    // Retain the platform provider only as a last-resort fallback for glyphs
    // outside the embedded 2k/Japanese sets.
    if (gBitmaps.host.glyph)
    {
        const unsigned short* platformRows = 0;
        int width = 0;
        int height = 0;
        int advance = 0;
        if (gBitmaps.host.glyph(gBitmaps.glyph_context,
                                codepoint,
                                &platformRows,
                                &width,
                                &height,
                                &advance) > 0 &&
            platformRows && width > 0 && width <= 16 &&
            height > 0 && height <= 32 && advance > 0 && advance <= 32)
        {
            glyph.rows16 = platformRows;
            glyph.width = width;
            glyph.height = height;
            glyph.advance = advance;
            return 1;
        }
    }

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
    // Three divisions by outputAlpha replaced with one exact reciprocal
    // multiply per channel (numerators stay below 2^16: 63*255 + 63*255 + 127).
    EnsureAlphaReciprocal();
    const int reciprocal = gAlphaReciprocal[outputAlpha];
    const int half = outputAlpha >> 1;
    const int outputR = DivSmall(static_cast<unsigned int>(sourceR * sourceAlpha + destinationR * destinationContribution + half), reciprocal);
    const int outputG = DivSmall(static_cast<unsigned int>(sourceG * sourceAlpha + destinationG * destinationContribution + half), reciprocal);
    const int outputB = DivSmall(static_cast<unsigned int>(sourceB * sourceAlpha + destinationB * destinationContribution + half), reciprocal);
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
    const int italicUnit = MaxInt(1, targetHeight / 11);
    TRatioStepper rowStepper;
    TRatioStepper columnStepper;
    StepperInit(rowStepper, glyph.height, targetHeight > 0 ? targetHeight : 1);
    StepperInit(columnStepper, glyph.width, targetWidth > 0 ? targetWidth : 1);
    for (int dy = 0; dy < targetHeight; ++dy)
    {
        StepperInit(columnStepper, glyph.width, targetWidth > 0 ? targetWidth : 1);
        int sourceY = rowStepper.quotient;
        if (sourceY >= glyph.height)
            sourceY = glyph.height - 1;
        const int italicOffset = style.italic
            ? ((targetHeight - 1 - dy) * italicUnit) / 4
            : 0;
        for (int dx = 0; dx < targetWidth; ++dx)
        {
            int sourceX = columnStepper.quotient;
            if (sourceX >= glyph.width)
                sourceX = glyph.width - 1;
            if (!GlyphBit(glyph, sourceX, sourceY))
            {
                StepperAdvance(columnStepper);
                continue;
            }
            CompositeBitmapPixel(slot, x + dx + italicOffset, y + dy, color, alpha);
            for (int bold = 1; bold <= boldOffset; ++bold)
                CompositeBitmapPixel(slot, x + dx + italicOffset + bold, y + dy, color, alpha);
            StepperAdvance(columnStepper);
        }
        StepperAdvance(rowStepper);
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
        // Exact steppers replace the per-pixel/per-row mapping divisions:
        // sourceOffset = (relative * sourceRect.extent) / destinationRect.extent.
        TRatioStepper rowStepper;
        StepperInit(rowStepper, sourceRect.height, destinationRect.height);
        StepperSeed(rowStepper, (top - destinationRect.y) * sourceRect.height);
        TRatioStepper columnStepper;
        StepperInit(columnStepper, sourceRect.width, destinationRect.width);
        for (int y = top; y < bottom; ++y)
        {
            int sourceOffsetY = rowStepper.quotient;
            if (sourceOffsetY >= sourceRect.height)
                sourceOffsetY = sourceRect.height - 1;
            StepperAdvance(rowStepper);
            StepperInit(columnStepper, sourceRect.width, destinationRect.width);
            StepperSeed(columnStepper, (left - destinationRect.x) * sourceRect.width);
            for (int x = left; x < right; ++x)
            {
                int sourceOffsetX = columnStepper.quotient;
                if (sourceOffsetX >= sourceRect.width)
                    sourceOffsetX = sourceRect.width - 1;
                StepperAdvance(columnStepper);
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

extern "C" int EasyRpgRgssBitmapClearRect(int handle,
                                           int x,
                                           int y,
                                           int width,
                                           int height)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (width < 0 || height < 0)
        return ERgssBitmapErrorInvalidArgument;
    if (width == 0 || height == 0)
        return ERgssBitmapOk;

    const int left = x < 0 ? 0 : x;
    const int top = y < 0 ? 0 : y;
    int right = x + width;
    int bottom = y + height;
    if (right > slot->width)
        right = slot->width;
    if (bottom > slot->height)
        bottom = slot->height;
    if (right <= left || bottom <= top)
        return ERgssBitmapOk;

    for (int row = top; row < bottom; ++row)
    {
        const int base = row * slot->width;
        for (int column = left; column < right; ++column)
        {
            slot->color[base + column] = 0;
            slot->alpha[base + column] = 0;
        }
    }
    return ERgssBitmapOk;
}

extern "C" int EasyRpgRgssBitmapGradientFillRect(int handle,
                                                  int x,
                                                  int y,
                                                  int width,
                                                  int height,
                                                  int red1,
                                                  int green1,
                                                  int blue1,
                                                  int alpha1,
                                                  int red2,
                                                  int green2,
                                                  int blue2,
                                                  int alpha2,
                                                  int vertical)
{
    TBitmapSlot* slot = FindSlot(handle);
    if (!slot)
        return ERgssBitmapErrorInvalidHandle;
    if (width < 0 || height < 0)
        return ERgssBitmapErrorInvalidArgument;
    if (width == 0 || height == 0)
        return ERgssBitmapOk;

    const int left = x < 0 ? 0 : x;
    const int top = y < 0 ? 0 : y;
    int right = x + width;
    int bottom = y + height;
    if (right > slot->width)
        right = slot->width;
    if (bottom > slot->height)
        bottom = slot->height;
    if (right <= left || bottom <= top)
        return ERgssBitmapOk;

    const int span = vertical ? height : width;
    const int denominator = span > 1 ? span - 1 : 1;
    for (int row = top; row < bottom; ++row)
    {
        const int base = row * slot->width;
        for (int column = left; column < right; ++column)
        {
            const int position = vertical ? row - y : column - x;
            const int red = red1 + (red2 - red1) * position / denominator;
            const int green = green1 + (green2 - green1) * position / denominator;
            const int blue = blue1 + (blue2 - blue1) * position / denominator;
            const int alpha = alpha1 + (alpha2 - alpha1) * position / denominator;
            slot->color[base + column] = Pack565(red, green, blue);
            slot->alpha[base + column] = static_cast<unsigned char>(ClampByte(alpha));
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

extern "C" int EasyRpgRgssBitmapBeginFrame(void)
{
    // Called once per render pass (EasyRpgRgssRenderRange) so the per-command
    // blits below can use the cached config/framebuffer snapshot.
    return RefreshFrameSnapshot() ? ERgssBitmapOk : ERgssBitmapErrorFramebuffer;
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

    const TFrameSnapshot* frameSnapshot = AcquireFrameSnapshot();
    if (!frameSnapshot)
        return ERgssBitmapErrorFramebuffer;
    const EasyRpgRgssConfig& config = frameSnapshot->config;

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

    void* framebuffer = frameSnapshot->pixels;
    const int framebufferWidth = frameSnapshot->width;
    const int framebufferHeight = frameSnapshot->height;
    const int framebufferPitch = frameSnapshot->pitch;

    const int globalOpacity = ClampByte(opacity);
    unsigned char* targetBytes = static_cast<unsigned char*>(framebuffer);

    if (logicalWidth == physicalRect.width && logicalHeight == physicalRect.height)
    {
        // Fast path: no scaling, so every physical pixel maps 1:1 onto the
        // source rect. Straight index loop, no per-pixel mapping at all.
        for (int py = 0; py < physicalRect.height; ++py)
        {
            const int sy = sourceY + py;
            unsigned short* target = reinterpret_cast<unsigned short*>(
                targetBytes + (physicalRect.y + py) * framebufferPitch);
            for (int px = 0; px < physicalRect.width; ++px)
            {
                const int sourceIndex = sy * slot->width + sourceX + px;
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

    int sourceXLookup[KFastBlitWidth];
    const int useSourceXLookup = physicalRect.width <= KFastBlitWidth;
    if (useSourceXLookup)
    {
        TRatioStepper columnStepper;
        StepperInit(columnStepper, logicalWidth, physicalRect.width);
        for (int px = 0; px < physicalRect.width; ++px)
        {
            sourceXLookup[px] = sourceX + columnStepper.quotient;
            StepperAdvance(columnStepper);
        }
    }

    TRatioStepper rowStepper;
    StepperInit(rowStepper, logicalHeight, physicalRect.height);
    TRatioStepper liveColumnStepper;
    for (int py = 0; py < physicalRect.height; ++py)
    {
        const int sy = sourceY + rowStepper.quotient;
        StepperAdvance(rowStepper);
        if (!useSourceXLookup)
            StepperInit(liveColumnStepper, logicalWidth, physicalRect.width);
        unsigned short* target = reinterpret_cast<unsigned short*>(
            targetBytes + (physicalRect.y + py) * framebufferPitch);
        for (int px = 0; px < physicalRect.width; ++px)
        {
            int sx = 0;
            if (useSourceXLookup)
            {
                sx = sourceXLookup[px];
            }
            else
            {
                sx = sourceX + liveColumnStepper.quotient;
                StepperAdvance(liveColumnStepper);
            }
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

    const TFrameSnapshot* frameSnapshot = AcquireFrameSnapshot();
    if (!frameSnapshot)
        return ERgssBitmapErrorFramebuffer;
    const EasyRpgRgssConfig& config = frameSnapshot->config;

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

    void* framebuffer = frameSnapshot->pixels;
    const int framebufferWidth = frameSnapshot->width;
    const int framebufferHeight = frameSnapshot->height;
    const int framebufferPitch = frameSnapshot->pitch;

    const int globalOpacity = ClampByte(opacity);
    int sourceXLookup[KFastBlitWidth];
    const int useSourceXLookup = physicalRect.width <= KFastBlitWidth;

    // Two chained exact mappings per pixel:
    //   logicalX  = clippedDestination.x + (px * clippedDestination.width) / physicalRect.width
    //   sourceOff = (logicalX - destination_rect->x) * source_rect->width / destination_rect->width
    // Both run as division-free TRatioSteppers: stepping the logical mapping
    // yields a 0/1/2 delta that advances the source mapping.
    const int relativeBaseX = clippedDestination.x - destination_rect->x;
    const int relativeBaseY = clippedDestination.y - destination_rect->y;
    TRatioStepper stepLogicalX;
    StepperInit(stepLogicalX, clippedDestination.width, physicalRect.width);
    TRatioStepper stepSourceX;
    StepperInit(stepSourceX, source_rect->width, destination_rect->width);
    StepperSeed(stepSourceX, relativeBaseX * source_rect->width);
    if (useSourceXLookup)
    {
        for (int px = 0; px < physicalRect.width; ++px)
        {
            int sourceOffsetX = stepSourceX.quotient;
            if (mirror)
                sourceOffsetX = source_rect->width - 1 - sourceOffsetX;
            sourceXLookup[px] = source_rect->x + sourceOffsetX;
            const int previousLogicalX = stepLogicalX.quotient;
            StepperAdvance(stepLogicalX);
            StepperAdvanceBy(stepSourceX, stepLogicalX.quotient - previousLogicalX);
        }
    }

    TRatioStepper stepLogicalY;
    StepperInit(stepLogicalY, clippedDestination.height, physicalRect.height);
    TRatioStepper stepSourceY;
    StepperInit(stepSourceY, source_rect->height, destination_rect->height);
    StepperSeed(stepSourceY, relativeBaseY * source_rect->height);

    unsigned char* targetBytes = static_cast<unsigned char*>(framebuffer);
    for (int py = 0; py < physicalRect.height; ++py)
    {
        // Steppers hold this row's mapping; advance them for the next row at
        // the bottom of the loop.
        const int sourceY = source_rect->y + stepSourceY.quotient;
        const int relativeY = relativeBaseY + stepLogicalY.quotient;
        const int targetY = physicalRect.y + py;
        if (sourceY < 0 || sourceY >= slot->height ||
            targetY < 0 || targetY >= framebufferHeight)
        {
            const int previousLogicalY = stepLogicalY.quotient;
            StepperAdvance(stepLogicalY);
            StepperAdvanceBy(stepSourceY, stepLogicalY.quotient - previousLogicalY);
            continue;
        }
        unsigned short* target = reinterpret_cast<unsigned short*>(
            targetBytes + targetY * framebufferPitch);
        if (!useSourceXLookup)
        {
            // Live mapping: restart the X chain from px = 0 for this row.
            StepperInit(stepLogicalX, clippedDestination.width, physicalRect.width);
            StepperInit(stepSourceX, source_rect->width, destination_rect->width);
            StepperSeed(stepSourceX, relativeBaseX * source_rect->width);
        }
        for (int px = 0; px < physicalRect.width; ++px)
        {
            int sourceX = 0;
            if (useSourceXLookup)
            {
                sourceX = sourceXLookup[px];
            }
            else
            {
                int sourceOffsetX = stepSourceX.quotient;
                if (mirror)
                    sourceOffsetX = source_rect->width - 1 - sourceOffsetX;
                sourceX = source_rect->x + sourceOffsetX;
                const int previousLogicalX = stepLogicalX.quotient;
                StepperAdvance(stepLogicalX);
                StepperAdvanceBy(stepSourceX, stepLogicalX.quotient - previousLogicalX);
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
        const int previousLogicalY = stepLogicalY.quotient;
        StepperAdvance(stepLogicalY);
        StepperAdvanceBy(stepSourceY, stepLogicalY.quotient - previousLogicalY);
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

    const TFrameSnapshot* frameSnapshot = AcquireFrameSnapshot();
    if (!frameSnapshot)
        return ERgssBitmapErrorFramebuffer;
    const EasyRpgRgssConfig& config = frameSnapshot->config;

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

    void* framebuffer = frameSnapshot->pixels;
    const int framebufferWidth = frameSnapshot->width;
    const int framebufferHeight = frameSnapshot->height;
    const int framebufferPitch = frameSnapshot->pitch;

    const int globalOpacity = ClampByte(opacity);
    int sourceXLookup[KFastBlitWidth];
    const int useSourceXLookup = physicalRect.width <= KFastBlitWidth;

    // Exact division-free tile mapping per pixel:
    //   logicalX = clippedDestination.x + (px * cd.width) / physical.width   (TRatioStepper)
    //   tileX    = (logicalX - dest.x + normalizedOffset) % periodX          (TWrapAccumulator)
    //   sourceX  = (tileX * 1000) / zoom  -> multiply when zoom divides 1000.
    const int zoomFactorX = 1000 % zoom_x_milli == 0 ? 1000 / zoom_x_milli : 0;
    const int zoomFactorY = 1000 % zoom_y_milli == 0 ? 1000 / zoom_y_milli : 0;
    TRatioStepper stepLogicalX;
    StepperInit(stepLogicalX, clippedDestination.width, physicalRect.width);
    TWrapAccumulator wrapX;
    WrapInit(wrapX,
             clippedDestination.x - destination_rect->x + normalizedOffsetX,
             periodX);
    if (useSourceXLookup)
    {
        for (int px = 0; px < physicalRect.width; ++px)
        {
            const int tileX = wrapX.value;
            int sourceOffsetX = zoomFactorX
                ? tileX * zoomFactorX
                : DivideInt(tileX * 1000, zoom_x_milli);
            if (sourceOffsetX >= source_rect->width)
                sourceOffsetX = source_rect->width - 1;
            sourceXLookup[px] = source_rect->x + sourceOffsetX;
            const int previousLogicalX = stepLogicalX.quotient;
            StepperAdvance(stepLogicalX);
            WrapAdvance(wrapX, stepLogicalX.quotient - previousLogicalX);
        }
    }

    TRatioStepper stepLogicalY;
    StepperInit(stepLogicalY, clippedDestination.height, physicalRect.height);
    TWrapAccumulator wrapY;
    WrapInit(wrapY,
             clippedDestination.y - destination_rect->y + normalizedOffsetY,
             periodY);

    unsigned char* targetBytes = static_cast<unsigned char*>(framebuffer);
    for (int py = 0; py < physicalRect.height; ++py)
    {
        const int tileY = wrapY.value;
        int sourceOffsetY = zoomFactorY
            ? tileY * zoomFactorY
            : DivideInt(tileY * 1000, zoom_y_milli);
        if (sourceOffsetY >= source_rect->height)
            sourceOffsetY = source_rect->height - 1;
        const int sourceY = source_rect->y + sourceOffsetY;
        const int targetY = physicalRect.y + py;
        const int previousLogicalY = stepLogicalY.quotient;
        StepperAdvance(stepLogicalY);
        WrapAdvance(wrapY, stepLogicalY.quotient - previousLogicalY);
        if (targetY < 0 || targetY >= framebufferHeight)
            continue;
        unsigned short* target = reinterpret_cast<unsigned short*>(
            targetBytes + targetY * framebufferPitch);
        if (!useSourceXLookup)
        {
            // Live mapping: restart the X chain from px = 0 for this row.
            StepperInit(stepLogicalX, clippedDestination.width, physicalRect.width);
            WrapInit(wrapX,
                     clippedDestination.x - destination_rect->x + normalizedOffsetX,
                     periodX);
        }
        for (int px = 0; px < physicalRect.width; ++px)
        {
            int sourceX = 0;
            if (useSourceXLookup)
            {
                sourceX = sourceXLookup[px];
            }
            else
            {
                const int tileX = wrapX.value;
                int sourceOffsetX = zoomFactorX
                    ? tileX * zoomFactorX
                    : DivideInt(tileX * 1000, zoom_x_milli);
                if (sourceOffsetX >= source_rect->width)
                    sourceOffsetX = source_rect->width - 1;
                sourceX = source_rect->x + sourceOffsetX;
                const int previousLogicalX = stepLogicalX.quotient;
                StepperAdvance(stepLogicalX);
                WrapAdvance(wrapX, stepLogicalX.quotient - previousLogicalX);
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
