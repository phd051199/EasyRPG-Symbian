#include "RgssDisplaySurface.h"

CRgssDisplaySurface* CRgssDisplaySurface::NewL(const TSize& aSize)
{
    CRgssDisplaySurface* self = new (ELeave) CRgssDisplaySurface;
    CleanupStack::PushL(self);
    self->ConstructL(aSize);
    CleanupStack::Pop(self);
    return self;
}

CRgssDisplaySurface::CRgssDisplaySurface()
    : iBitmap(NULL),
      iSize(0, 0),
      iLocked(EFalse),
      iDirtyCount(0)
{
}

void CRgssDisplaySurface::ConstructL(const TSize& aSize)
{
    if (aSize.iWidth <= 0 || aSize.iHeight <= 0)
        User::Leave(KErrArgument);
    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iBitmap->Create(aSize, EColor64K));
    iSize = aSize;
    ClearL();
}

CRgssDisplaySurface::~CRgssDisplaySurface()
{
    EndRender();
    delete iBitmap;
    iBitmap = NULL;
}

TUint16* CRgssDisplaySurface::BeginRenderL()
{
    if (!iBitmap || iLocked)
        User::Leave(KErrNotReady);
    iBitmap->LockHeap();
    iLocked = ETrue;
    TUint16* pixels = reinterpret_cast<TUint16*>(iBitmap->DataAddress());
    if (!pixels)
    {
        EndRender();
        User::Leave(KErrNoMemory);
    }
    return pixels;
}

void CRgssDisplaySurface::EndRender()
{
    if (!iBitmap || !iLocked)
        return;
    iBitmap->UnlockHeap();
    iLocked = EFalse;
}

TInt CRgssDisplaySurface::StrideBytes() const
{
    return iBitmap ? iBitmap->DataStride() : 0;
}

const CFbsBitmap& CRgssDisplaySurface::Bitmap() const
{
    return *iBitmap;
}

const TSize& CRgssDisplaySurface::Size() const
{
    return iSize;
}

void CRgssDisplaySurface::ClearL()
{
    TUint16* pixels = BeginRenderL();
    Mem::FillZ(pixels, StrideBytes() * iSize.iHeight);
    EndRender();
    MarkFullDirty();
}

TRect CRgssDisplaySurface::ClipRect(const TRect& aRect) const
{
    const TInt left = Max(0, aRect.iTl.iX);
    const TInt top = Max(0, aRect.iTl.iY);
    const TInt right = Min(iSize.iWidth, aRect.iBr.iX);
    const TInt bottom = Min(iSize.iHeight, aRect.iBr.iY);
    if (right <= left || bottom <= top)
        return TRect(0, 0, 0, 0);
    return TRect(left, top, right, bottom);
}

void CRgssDisplaySurface::MarkDirty(const TRect& aRect)
{
    const TRect clipped = ClipRect(aRect);
    if (clipped.Width() <= 0 || clipped.Height() <= 0)
        return;
    if (iDirtyCount >= KMaxDirtyRects)
    {
        MarkFullDirty();
        return;
    }
    iDirtyRects[iDirtyCount++] = clipped;
}

void CRgssDisplaySurface::MarkFullDirty()
{
    iDirtyRects[0] = TRect(TPoint(0, 0), iSize);
    iDirtyCount = 1;
}

TInt CRgssDisplaySurface::DirtyCount() const
{
    return iDirtyCount;
}

const TRect& CRgssDisplaySurface::DirtyRect(TInt aIndex) const
{
    return iDirtyRects[aIndex];
}

void CRgssDisplaySurface::ClearDirty()
{
    iDirtyCount = 0;
}
