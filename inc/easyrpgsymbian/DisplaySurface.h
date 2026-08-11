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
    void ClearL();
    void PresentL();

private:
    CSymbianDisplaySurface();
    void ConstructL(const TSize& aSize);

private:
    CFbsBitmap* iBitmap;
    TSize iSize;
    TBool iHasFrame;
};

#endif
