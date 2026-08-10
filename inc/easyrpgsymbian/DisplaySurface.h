#ifndef EASYRPG_SYMBIAN_DISPLAY_SURFACE_H
#define EASYRPG_SYMBIAN_DISPLAY_SURFACE_H

#include <e32base.h>
#include <fbs.h>

class CFrameBuffer;

class CSymbianDisplaySurface : public CBase
{
public:
    static CSymbianDisplaySurface* NewL(const TSize& aSize);
    ~CSymbianDisplaySurface();

    TUint32* Pixels();
    const TUint32* Pixels() const;
    TInt StridePixels() const;
    const TSize& Size() const;
    void Clear(TUint32 aColor);
    void PresentL();
    const CFbsBitmap& Bitmap() const;

private:
    CSymbianDisplaySurface();
    void ConstructL(const TSize& aSize);

private:
    HBufC8* iPixels;
    CFrameBuffer* iNativeBuffer;
    TSize iSize;
    TInt iStridePixels;
};

#endif
