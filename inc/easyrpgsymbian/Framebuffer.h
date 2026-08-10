#ifndef EASYRPG_SYMBIAN_FRAMEBUFFER_H
#define EASYRPG_SYMBIAN_FRAMEBUFFER_H

#include <e32base.h>
#include <fbs.h>

class CFrameBuffer : public CBase
{
public:
    static CFrameBuffer* NewL(const TSize& aSize);
    ~CFrameBuffer();

    TUint16* LockPixelsL(TInt& aStridePixels);
    void UnlockPixels();
    CFbsBitmap& Bitmap();
    const CFbsBitmap& Bitmap() const;
    const TSize& Size() const;

private:
    CFrameBuffer();
    void ConstructL(const TSize& aSize);

private:
    CFbsBitmap* iBitmap;
    TSize iSize;
    TBool iLocked;
};

#endif
