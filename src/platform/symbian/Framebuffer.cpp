#include "easyrpgsymbian/Framebuffer.h"

CFrameBuffer* CFrameBuffer::NewL(const TSize& aSize)
{
    CFrameBuffer* self = new (ELeave) CFrameBuffer;
    CleanupStack::PushL(self);
    self->ConstructL(aSize);
    CleanupStack::Pop(self);
    return self;
}

CFrameBuffer::CFrameBuffer()
    : iBitmap(NULL),
      iSize(0, 0),
      iLocked(EFalse)
{
}

void CFrameBuffer::ConstructL(const TSize& aSize)
{
    if (aSize.iWidth <= 0 || aSize.iHeight <= 0)
        User::Leave(KErrArgument);

    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iBitmap->Create(aSize, EColor64K));
    iSize = aSize;
}

CFrameBuffer::~CFrameBuffer()
{
    if (iLocked && iBitmap)
    {
        iBitmap->UnlockHeap();
        iLocked = EFalse;
    }

    delete iBitmap;
    iBitmap = NULL;
}

TUint16* CFrameBuffer::LockPixelsL(TInt& aStridePixels)
{
    if (!iBitmap)
        User::Leave(KErrNotReady);
    if (iLocked)
        User::Leave(KErrInUse);

    iBitmap->LockHeap();
    iLocked = ETrue;

    TUint32* address = iBitmap->DataAddress();
    if (!address)
    {
        iBitmap->UnlockHeap();
        iLocked = EFalse;
        User::Leave(KErrNoMemory);
    }

    aStridePixels = iBitmap->DataStride() / static_cast<TInt>(sizeof(TUint16));
    return reinterpret_cast<TUint16*>(address);
}

void CFrameBuffer::UnlockPixels()
{
    if (!iBitmap || !iLocked)
        return;

    iBitmap->UnlockHeap();
    iLocked = EFalse;
}

CFbsBitmap& CFrameBuffer::Bitmap()
{
    return *iBitmap;
}

const CFbsBitmap& CFrameBuffer::Bitmap() const
{
    return *iBitmap;
}

const TSize& CFrameBuffer::Size() const
{
    return iSize;
}
