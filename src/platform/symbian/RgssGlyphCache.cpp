#include <fbs.h>
#include <gdi.h>
#include <openfont.h>

#include "RgssGlyphCache.h"

namespace
{
_LIT(KRgssGlyphTypeface, "Series 60 Sans");
const TInt KRgssGlyphBaseHeight = 12;
const TInt KRgssGlyphCacheMask = 127;
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
      iFont(NULL)
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

    const TInt stride = CFbsBitmap::ScanLineLength(bitmapSize.iWidth, EGray256);
    if (stride < width)
        return KErrCorrupt;

    Mem::FillZ(aEntry.iRows, sizeof(aEntry.iRows));
    for (TInt y = 0; y < height; ++y)
    {
        TUint16 row = 0;
        const TUint8* source = bitmap + y * stride;
        for (TInt x = 0; x < width; ++x)
        {
            if (source[x] >= 64)
                row = static_cast<TUint16>(row | (static_cast<TUint16>(1u) << x));
        }
        aEntry.iRows[y] = row;
    }

    aEntry.iCodepoint = aCodepoint;
    aEntry.iWidth = width;
    aEntry.iHeight = height;
    aEntry.iAdvance = advance;
    aEntry.iValid = ETrue;
    return KErrNone;
}
