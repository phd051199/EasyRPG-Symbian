#include <e32std.h>

#include "easyrpgsymbian/DisplaySurface.h"
#include "easyrpgsymbian/UpstreamPlayerBridge.h"

CSymbianDisplaySurface* CSymbianDisplaySurface::NewL(const TSize& aSize)
{
    CSymbianDisplaySurface* self = new (ELeave) CSymbianDisplaySurface;
    CleanupStack::PushL(self);
    self->ConstructL(aSize);
    CleanupStack::Pop(self);
    return self;
}

CSymbianDisplaySurface::CSymbianDisplaySurface()
    : iBitmap(NULL),
      iSize(0, 0),
      iHasFrame(EFalse)
{
}

void CSymbianDisplaySurface::ConstructL(const TSize& aSize)
{
    if (aSize.iWidth <= 0 || aSize.iHeight <= 0)
        User::Leave(KErrArgument);

    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iBitmap->Create(aSize, EColor16MA));
    iSize = aSize;
    ClearL();
}

CSymbianDisplaySurface::~CSymbianDisplaySurface()
{
    delete iBitmap;
    iBitmap = NULL;
}

const TSize& CSymbianDisplaySurface::Size() const
{
    return iSize;
}

const CFbsBitmap& CSymbianDisplaySurface::Bitmap() const
{
    return *iBitmap;
}

TBool CSymbianDisplaySurface::HasFrame() const
{
    return iHasFrame;
}

void CSymbianDisplaySurface::ClearL()
{
    if (!iBitmap)
        User::Leave(KErrNotReady);

    iBitmap->LockHeap();
    TUint8* destination = reinterpret_cast<TUint8*>(iBitmap->DataAddress());
    if (!destination)
    {
        iBitmap->UnlockHeap();
        User::Leave(KErrNoMemory);
    }

    Mem::FillZ(destination, iBitmap->DataStride() * iSize.iHeight);
    iBitmap->UnlockHeap();
    iHasFrame = EFalse;
}

void CSymbianDisplaySurface::PresentL()
{
    if (!iBitmap)
        User::Leave(KErrNotReady);

    const unsigned int* source = EasyRpgUpstreamPlayerPixels();
    const TInt sourceWidth = EasyRpgUpstreamPlayerWidth();
    const TInt sourceHeight = EasyRpgUpstreamPlayerHeight();
    const TInt sourceStridePixels = EasyRpgUpstreamPlayerPitchPixels();
    if (!source || sourceWidth != iSize.iWidth || sourceHeight != iSize.iHeight ||
        sourceStridePixels < iSize.iWidth)
        User::Leave(KErrNotReady);

    iBitmap->LockHeap();
    TUint8* destination = reinterpret_cast<TUint8*>(iBitmap->DataAddress());
    if (!destination)
    {
        iBitmap->UnlockHeap();
        User::Leave(KErrNoMemory);
    }

    const TInt destinationStride = iBitmap->DataStride();
    const TInt sourceStride = sourceStridePixels * static_cast<TInt>(sizeof(TUint32));
    const TInt rowBytes = iSize.iWidth * static_cast<TInt>(sizeof(TUint32));
    const TUint8* sourceBytes = reinterpret_cast<const TUint8*>(source);
    for (TInt y = 0; y < iSize.iHeight; ++y)
    {
        Mem::Copy(destination + y * destinationStride,
                  sourceBytes + y * sourceStride,
                  rowBytes);
    }

    iBitmap->UnlockHeap();
    iHasFrame = ETrue;
}
