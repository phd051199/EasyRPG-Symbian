#include <stdio.h>

#include <fbs.h>
#include <gdi.h>
#include <openfont.h>

#include "RgssGlyphCache.h"

extern "C" void EasyRpgRgssVmTrace(const char* aText);

namespace
{
_LIT(KRgssGlyphTypeface, "Series 60 Sans");
const TInt KRgssGlyphBaseHeight = 12;
const TInt KRgssGlyphCacheMask = 127;
const TUint8 KRgssGlyphCoverageThreshold = 128;
// Hard cap while walking a run-length-encoded mono glyph: the real band
// stream of a 16x32 glyph is a few dozen bits, so anything larger means the
// stream is corrupt and the walk must bail out instead of chasing garbage.
const TInt KRgssGlyphMaxMonoBits = 65536;

int gGlyphTraceLeft = 3;

void TraceFirstGlyphs(TUint aCodepoint,
                      TInt aType,
                      TInt aWidth,
                      TInt aHeight,
                      TInt aAdvance,
                      const TUint8* aData,
                      const TUint16* aRows)
{
    if (gGlyphTraceLeft <= 0)
        return;
    --gGlyphTraceLeft;
    char line[160];
    snprintf(line,
             sizeof(line),
             "glyph U+%04X type=%d %dx%d adv=%d bytes=%02x %02x %02x %02x %02x %02x %02x %02x rows=%04x %04x %04x %04x",
             aCodepoint,
             aType,
             aWidth,
             aHeight,
             aAdvance,
             aData[0], aData[1], aData[2], aData[3],
             aData[4], aData[5], aData[6], aData[7],
             aRows[0], aRows[1], aRows[2], aRows[3]);
    EasyRpgRgssVmTrace(line);
}
}

CRgssGlyphCache* CRgssGlyphCache::NewL()
{
    CRgssGlyphCache* self = new (ELeave) CRgssGlyphCache;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CRgssGlyphCache::CRgssGlyphCache()
    : iStore(NULL),
      iFont(NULL),
      iGlyphBitmapType(EMonochromeGlyphBitmap)
{
    for (TInt index = 0; index <= KRgssGlyphCacheMask; ++index)
    {
        iEntries[index].iCodepoint = 0;
        iEntries[index].iWidth = 0;
        iEntries[index].iHeight = 0;
        iEntries[index].iAdvance = 0;
        iEntries[index].iValid = EFalse;
        Mem::FillZ(iEntries[index].iRows, sizeof(iEntries[index].iRows));
    }
}

void CRgssGlyphCache::ConstructL()
{
    iStore = CFbsTypefaceStore::NewL(NULL);
    iStore->SetDefaultBitmapType(EAntiAliasedGlyphBitmap);

    TFontSpec spec(KRgssGlyphTypeface, KRgssGlyphBaseHeight);
    spec.iFontStyle.SetBitmapType(EAntiAliasedGlyphBitmap);
    User::LeaveIfError(iStore->GetNearestFontInPixels(iFont, spec));
    // The typeface store may hand back a font that cannot honour the
    // requested anti-aliasing (bitmap fonts stay monochrome). The effective
    // format decides how GetCharacterData packs its glyph bytes, so it must
    // be read from the returned font instead of assumed.
    iGlyphBitmapType = iFont->FontSpecInTwips().iFontStyle.BitmapType();

    char line[96];
    snprintf(line, sizeof(line), "glyph font bitmap type=%d", iGlyphBitmapType);
    EasyRpgRgssVmTrace(line);
}

CRgssGlyphCache::~CRgssGlyphCache()
{
    if (iStore && iFont)
        iStore->ReleaseFont(iFont);
    iFont = NULL;
    delete iStore;
    iStore = NULL;
}

int CRgssGlyphCache::Resolve(void* aContext,
                             unsigned int aCodepoint,
                             const unsigned short** aRows,
                             int* aWidth,
                             int* aHeight,
                             int* aAdvance)
{
    if (!aContext || !aRows || !aWidth || !aHeight || !aAdvance)
        return 0;
    CRgssGlyphCache* self = static_cast<CRgssGlyphCache*>(aContext);
    const TUint16* rows = NULL;
    TInt width = 0;
    TInt height = 0;
    TInt advance = 0;
    if (self->ResolveGlyph(static_cast<TUint>(aCodepoint),
                           rows,
                           width,
                           height,
                           advance) != KErrNone)
        return 0;
    *aRows = rows;
    *aWidth = width;
    *aHeight = height;
    *aAdvance = advance;
    return 1;
}

TInt CRgssGlyphCache::ResolveGlyph(TUint aCodepoint,
                                   const TUint16*& aRows,
                                   TInt& aWidth,
                                   TInt& aHeight,
                                   TInt& aAdvance)
{
    const TUint hash = aCodepoint * 2654435761u;
    TEntry& entry = iEntries[(hash >> 25) & KRgssGlyphCacheMask];
    if (!entry.iValid || entry.iCodepoint != aCodepoint)
    {
        const TInt result = Rasterize(aCodepoint, entry);
        if (result != KErrNone)
            return result;
    }
    aRows = entry.iRows;
    aWidth = entry.iWidth;
    aHeight = entry.iHeight;
    aAdvance = entry.iAdvance;
    return KErrNone;
}

TInt CRgssGlyphCache::Rasterize(TUint aCodepoint, TEntry& aEntry)
{
    if (!iFont)
        return KErrNotReady;

    TOpenFontCharMetrics metrics;
    const TUint8* bitmap = NULL;
    TSize bitmapSize;
    const CFont::TCharacterDataAvailability availability =
        iFont->GetCharacterData(aCodepoint, metrics, bitmap, bitmapSize);
    if (availability != CFont::EAllCharacterData || !bitmap)
        return KErrNotFound;

    const TInt width = metrics.Width();
    const TInt height = metrics.Height();
    const TInt advance = metrics.HorizAdvance();
    if (width <= 0 || width > 16 || height <= 0 || height > 32 ||
        advance <= 0 || advance > 32 ||
        bitmapSize.iWidth < width || bitmapSize.iHeight < height)
        return KErrNotSupported;

    Mem::FillZ(aEntry.iRows, sizeof(aEntry.iRows));
    TInt onCount = 0;

    if (iGlyphBitmapType == EAntiAliasedGlyphBitmap ||
        iGlyphBitmapType == ESubPixelGlyphBitmap)
    {
        // Straight 8bpp coverage rows: one byte per pixel.
        const TInt stride = CFbsBitmap::ScanLineLength(bitmapSize.iWidth, EGray256);
        if (stride < width)
            return KErrCorrupt;
        for (TInt y = 0; y < height; ++y)
        {
            const TUint8* row = bitmap + y * stride;
            for (TInt x = 0; x < width; ++x)
            {
                if (row[x] >= KRgssGlyphCoverageThreshold)
                {
                    aEntry.iRows[y] = static_cast<TUint16>(
                        aEntry.iRows[y] | (1u << x));
                    ++onCount;
                }
            }
        }
    }
    else
    {
        // Monochrome glyph data is run-length encoded per line band, not
        // plain rows: a 1-bit repeat flag plus a 4-bit line count, then the
        // band's line pixels as a little-endian LSB-first bit stream. A
        // repeat band carries one line that stands for count lines; a
        // literal band carries count lines. Reading these bytes as 8bpp
        // coverage (the old assumption) turns run counts and packed bits
        // into near-solid blocks, which is exactly how every glyph came
        // out as a filled rectangle.
        TInt bitPos = 0;
        TInt line = 0;
        while (line < height)
        {
            if (bitPos + 5 > KRgssGlyphMaxMonoBits)
                return KErrCorrupt;
            const TUint repeat = (bitmap[bitPos >> 3] >> (bitPos & 7)) & 1u;
            ++bitPos;
            TInt count = 0;
            for (TInt bit = 0; bit < 4; ++bit, ++bitPos)
                count |= static_cast<TInt>((bitmap[bitPos >> 3] >> (bitPos & 7)) & 1u) << bit;
            if (count <= 0)
                return KErrCorrupt;

            const TInt storedLines = repeat == 0 ? 1 : count;
            for (TInt stored = 0; stored < storedLines; ++stored)
            {
                TUint16 rowBits = 0;
                TInt rowOn = 0;
                for (TInt x = 0; x < width; ++x, ++bitPos)
                {
                    if (((bitmap[bitPos >> 3] >> (bitPos & 7)) & 1u) != 0)
                    {
                        rowBits = static_cast<TUint16>(rowBits | (1u << x));
                        ++rowOn;
                    }
                }
                if (line < height)
                {
                    aEntry.iRows[line] = rowBits;
                    onCount += rowOn;
                }
                ++line;
                if (repeat == 0)
                {
                    // Repeat band: the decoded line also stands for the
                    // remaining count-1 lines.
                    for (TInt copy = 1; copy < count && line < height; ++copy, ++line)
                    {
                        aEntry.iRows[line] = rowBits;
                        onCount += rowOn;
                    }
                }
            }
        }
    }

    // A sane glyph is mostly empty; a misparsed stream floods the cell. In
    // that case fall back to the built-in font rather than show a block.
    if (onCount <= 0 || onCount * 100 > width * height * 85)
        return KErrNotFound;

    TraceFirstGlyphs(aCodepoint, iGlyphBitmapType, width, height, advance, bitmap, aEntry.iRows);

    aEntry.iCodepoint = aCodepoint;
    aEntry.iWidth = width;
    aEntry.iHeight = height;
    aEntry.iAdvance = advance;
    aEntry.iValid = ETrue;
    return KErrNone;
}
