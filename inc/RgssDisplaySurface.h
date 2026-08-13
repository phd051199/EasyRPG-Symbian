#ifndef EASYRPG_RGSS_DISPLAY_SURFACE_H
#define EASYRPG_RGSS_DISPLAY_SURFACE_H

#include <e32base.h>
#include <fbs.h>

class CRgssDisplaySurface : public CBase
{
public:
    static CRgssDisplaySurface* NewL(const TSize& aSize);
    ~CRgssDisplaySurface();

    TUint16* BeginRenderL();
    void EndRender();
    TInt StrideBytes() const;
    const CFbsBitmap& Bitmap() const;
    const TSize& Size() const;
    void ClearL();
    void MarkDirty(const TRect& aRect);
    void MarkFullDirty();
    TInt DirtyCount() const;
    const TRect& DirtyRect(TInt aIndex) const;
    void ClearDirty();

private:
    CRgssDisplaySurface();
    void ConstructL(const TSize& aSize);
    TRect ClipRect(const TRect& aRect) const;

private:
    enum { KMaxDirtyRects = 16 };
    CFbsBitmap* iBitmap;
    TSize iSize;
    TBool iLocked;
    TRect iDirtyRects[KMaxDirtyRects];
    TInt iDirtyCount;
};

#endif
