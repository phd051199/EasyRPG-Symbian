#ifndef EASYRPG_RGSS_GLYPH_CACHE_H
#define EASYRPG_RGSS_GLYPH_CACHE_H

#include <e32base.h>
#include "RgssBitmapBridge.h"

class CFbsTypefaceStore;
class CFont;

class CRgssGlyphCache : public CBase
{
public:
    static CRgssGlyphCache* NewL();
    ~CRgssGlyphCache();

    static int Resolve(void* aContext,
                       unsigned int aCodepoint,
                       const unsigned short** aRows,
                       int* aWidth,
                       int* aHeight,
                       int* aAdvance);

private:
    struct TEntry
    {
        TUint iCodepoint;
        TUint16 iRows[32];
        TInt iWidth;
        TInt iHeight;
        TInt iAdvance;
        TBool iValid;
    };

private:
    CRgssGlyphCache();
    void ConstructL();
    TInt ResolveGlyph(TUint aCodepoint,
                      const TUint16*& aRows,
                      TInt& aWidth,
                      TInt& aHeight,
                      TInt& aAdvance);
    TInt Rasterize(TUint aCodepoint, TEntry& aEntry);

private:
    CFbsTypefaceStore* iStore;
    CFont* iFont;
    TEntry iEntries[128];
};

#endif
