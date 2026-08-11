#ifndef EASYRPG_SYMBIAN_DISPLAY_SURFACE_H
#define EASYRPG_SYMBIAN_DISPLAY_SURFACE_H

#include <e32base.h>
#include <fbs.h>

class CSymbianDisplaySurface : public CBase
{
public:
    static CSymbianDisplaySurface* NewL(const TSize& aSize);
    ~CSymbianDisplaySurface();

    const TSize& Size() const;
    const CFbsBitmap& Bitmap() const;
    TBool HasFrame() const;
    TBool HasDirtyRegions() const;
    TInt DirtyRegionCount() const;
    const TRect& DirtyRegion(TInt aIndex) const;
    void ClearDirtyRegions();
    void ClearL();
    void PresentL();

private:
    CSymbianDisplaySurface();
    void ConstructL(const TSize& aSize);

private:
    enum
    {
        KDirtyRegionCapacity = 16
    };

    CFbsBitmap* iBitmap;
    TSize iSize;
    TBool iHasFrame;
    TRect iDirtyRegions[KDirtyRegionCapacity];
    TInt iDirtyRegionCount;
};

#endif
