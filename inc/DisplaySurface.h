#ifndef EASYRPG_DISPLAY_SURFACE_H
#define EASYRPG_DISPLAY_SURFACE_H

#include <e32base.h>
#include <fbs.h>

class CUpstreamPlayer;

struct TEasyRpgDirtyRegionSlot
{
    TEasyRpgDirtyRegionSlot() : iRect(0, 0, 0, 0) {}
    TRect iRect;
};

class CDisplaySurface : public CBase
{
public:
    static CDisplaySurface* NewL(const TSize& aSize);
    ~CDisplaySurface();

    void ClearL();
    TUint16* BeginRenderL();
    void EndRender();
    TInt RenderStrideBytes() const;
    void PresentL();
    const TSize& Size() const;
    const CFbsBitmap& Bitmap() const;
    TBool HasFrame() const;
    TBool HasDirtyRegions() const;
    TInt DirtyRegionCount() const;
    const TRect& DirtyRegion(TInt aIndex) const;
    void ClearDirtyRegions();

private:
    CDisplaySurface();
    void ConstructL(const TSize& aSize);

private:
    CFbsBitmap* iBitmap;
    TBool iBitmapLocked;
    TSize iSize;
    TBool iHasFrame;
    enum { KMaxDirtyRegions = 16 };
    TEasyRpgDirtyRegionSlot iDirtyRegions[KMaxDirtyRegions];
    TInt iDirtyRegionCount;
};

#endif
