#ifndef EASYRPG_DISPLAY_SURFACE_H
#define EASYRPG_DISPLAY_SURFACE_H

#include <e32base.h>
#include <fbs.h>

class CUpstreamPlayer;

class CSymbianDisplaySurface : public CBase
{
public:
    static CSymbianDisplaySurface* NewL(const TSize& aSize);
    ~CSymbianDisplaySurface();

    void ClearL();
    void PresentL();
    const TSize& Size() const;
    const CFbsBitmap& Bitmap() const;
    TBool HasFrame() const;
    TBool HasDirtyRegions() const;
    TInt DirtyRegionCount() const;
    const TRect& DirtyRegion(TInt aIndex) const;
    void ClearDirtyRegions();

private:
    CSymbianDisplaySurface();
    void ConstructL(const TSize& aSize);

private:
    CFbsBitmap* iBitmap;
    TSize iSize;
    TBool iHasFrame;
    enum { KMaxDirtyRegions = 16 };
    TRect iDirtyRegions[KMaxDirtyRegions];
    TInt iDirtyRegionCount;
};

#endif
