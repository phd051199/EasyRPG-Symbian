#include <e32std.h>

#include "DisplaySurface.h"
#include "UpstreamPlayerBridge.h"

namespace
{
// Region budgets, kept separate from PlayerConfig.h::KMaximumScreenUpdateRegions
// (upstream damage tracking cap): these bound what presentation can batch.
// ponytail: per-frame merging lives in core RenderDamage::OptimizeDirty; here
// we only cap accumulation — more than KMaximumPresentRegions pending rects
// means a heavy-change burst, which a full paint serves just as well.
const TInt KDirtyRegionCapacity = 16;
const TInt KMaximumPresentRegions = 4;

TBool RectsEqual(const TRect& aFirst, const TRect& aSecond)
{
    return aFirst.iTl.iX == aSecond.iTl.iX &&
        aFirst.iTl.iY == aSecond.iTl.iY &&
        aFirst.iBr.iX == aSecond.iBr.iX &&
        aFirst.iBr.iY == aSecond.iBr.iY;
}

}

CDisplaySurface* CDisplaySurface::NewL(const TSize& aSize)
{
    CDisplaySurface* self = new (ELeave) CDisplaySurface;
    CleanupStack::PushL(self);
    self->ConstructL(aSize);
    CleanupStack::Pop(self);
    return self;
}

CDisplaySurface::CDisplaySurface()
    : iBitmap(NULL),
      iBitmapLocked(EFalse),
      iSize(0, 0),
      iHasFrame(EFalse),
      iDirtyRegionCount(0),
      iPresentScrollX(0),
      iPresentScrollY(0)
{
}

void CDisplaySurface::ConstructL(const TSize& aSize)
{
    if (aSize.iWidth <= 0 || aSize.iHeight <= 0)
        User::Leave(KErrArgument);

    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iBitmap->Create(aSize, EColor64K));
    iSize = aSize;
    ClearL();
}

CDisplaySurface::~CDisplaySurface()
{
    EndRender();
    delete iBitmap;
    iBitmap = NULL;
}

const TSize& CDisplaySurface::Size() const
{
    return iSize;
}

const CFbsBitmap& CDisplaySurface::Bitmap() const
{
    return *iBitmap;
}

TBool CDisplaySurface::HasFrame() const
{
    return iHasFrame;
}

TBool CDisplaySurface::HasDirtyRegions() const
{
    return iDirtyRegionCount > 0;
}

TInt CDisplaySurface::DirtyRegionCount() const
{
    return iDirtyRegionCount;
}

const TRect& CDisplaySurface::DirtyRegion(TInt aIndex) const
{
    return iDirtyRegions[aIndex].iRect;
}

TBool CDisplaySurface::HasPresentScroll() const
{
    return iPresentScrollX != 0 || iPresentScrollY != 0;
}

void CDisplaySurface::PresentScroll(TInt& aShiftX, TInt& aShiftY) const
{
    aShiftX = iPresentScrollX;
    aShiftY = iPresentScrollY;
}

void CDisplaySurface::ClearDirtyRegions()
{
    iDirtyRegionCount = 0;
    iPresentScrollX = 0;
    iPresentScrollY = 0;
}

void CDisplaySurface::ClearL()
{
    TUint16* destination = BeginRenderL();
    Mem::FillZ(destination, RenderStrideBytes() * iSize.iHeight);
    EndRender();
    iHasFrame = EFalse;
    iDirtyRegionCount = 0;
    iPresentScrollX = 0;
    iPresentScrollY = 0;
}

TUint16* CDisplaySurface::BeginRenderL()
{
    if (!iBitmap || iBitmapLocked)
        User::Leave(KErrNotReady);
    iBitmap->LockHeap();
    iBitmapLocked = ETrue;
    TUint16* pixels = reinterpret_cast<TUint16*>(iBitmap->DataAddress());
    if (!pixels)
    {
        EndRender();
        User::Leave(KErrNoMemory);
    }
    return pixels;
}

void CDisplaySurface::EndRender()
{
    if (!iBitmap || !iBitmapLocked)
        return;
    iBitmap->UnlockHeap();
    iBitmapLocked = EFalse;
}

TInt CDisplaySurface::RenderStrideBytes() const
{
    return iBitmap ? iBitmap->DataStride() : 0;
}

void CDisplaySurface::PresentL()
{
    if (!iBitmap)
        User::Leave(KErrNotReady);

    TInt coreScrollX = 0;
    TInt coreScrollY = 0;
    const TBool coreHasScroll =
        EasyRpgUpstreamPlayerPresentScroll(&coreScrollX, &coreScrollY) != 0;
    const TInt coreDirtyCount = EasyRpgUpstreamPlayerDirtyRegionCount();
    if (iHasFrame && coreDirtyCount == 0 && !coreHasScroll)
    {
        EasyRpgUpstreamPlayerConsumeDirtyRegions();
        return;
    }

    const TBool hadFrame = iHasFrame;
    const TBool hadPendingScroll = HasPresentScroll();
    const TRect frameRect(TPoint(0, 0), iSize);
    TEasyRpgDirtyRegionSlot currentDirty[KDirtyRegionCapacity];
    TInt currentDirtyCount = 0;

    if (!hadFrame)
    {
        currentDirty[0].iRect = frameRect;
        currentDirtyCount = 1;
    }
    else if (coreDirtyCount >= 0 && coreDirtyCount <= KDirtyRegionCapacity)
    {
        TBool coreDirtyValid = ETrue;
        for (TInt index = 0; index < coreDirtyCount; ++index)
        {
            TInt x = 0;
            TInt y = 0;
            TInt width = 0;
            TInt height = 0;
            if (!EasyRpgUpstreamPlayerDirtyRegion(index, &x, &y, &width, &height))
            {
                coreDirtyValid = EFalse;
                break;
            }

            const TInt left = Max(0, x);
            const TInt top = Max(0, y);
            const TInt right = Min(iSize.iWidth, x + width);
            const TInt bottom = Min(iSize.iHeight, y + height);
            if (right > left && bottom > top)
                currentDirty[currentDirtyCount++].iRect = TRect(left, top, right, bottom);
        }

        if (!coreDirtyValid)
        {
            currentDirty[0].iRect = frameRect;
            currentDirtyCount = 1;
        }
    }
    else
    {
        currentDirty[0].iRect = frameRect;
        currentDirtyCount = 1;
    }

    if (coreDirtyCount >= 0)
        EasyRpgUpstreamPlayerConsumeDirtyRegions();

    const TBool currentFull =
        currentDirtyCount == 1 && RectsEqual(currentDirty[0].iRect, frameRect);
    if (coreHasScroll)
    {
        if (!hadFrame || currentFull || iDirtyRegionCount > 0 || hadPendingScroll)
        {
            currentDirty[0].iRect = frameRect;
            currentDirtyCount = 1;
            iPresentScrollX = 0;
            iPresentScrollY = 0;
        }
        else
        {
            iPresentScrollX = coreScrollX;
            iPresentScrollY = coreScrollY;
        }
    }
    else if (hadPendingScroll && currentDirtyCount > 0)
    {
        currentDirty[0].iRect = frameRect;
        currentDirtyCount = 1;
        iPresentScrollX = 0;
        iPresentScrollY = 0;
    }

    iHasFrame = ETrue;

    if (currentDirtyCount <= 0)
        return;

    if (currentDirtyCount == 1 && RectsEqual(currentDirty[0].iRect, frameRect))
    {
        iDirtyRegions[0].iRect = frameRect;
        iDirtyRegionCount = 1;
        iPresentScrollX = 0;
        iPresentScrollY = 0;
        return;
    }

    if (iDirtyRegionCount == 1 && RectsEqual(iDirtyRegions[0].iRect, frameRect))
    {
        iPresentScrollX = 0;
        iPresentScrollY = 0;
        return;
    }

    for (TInt index = 0; index < currentDirtyCount; ++index)
    {
        if (iDirtyRegionCount >= KMaximumPresentRegions)
        {
            iDirtyRegions[0].iRect = frameRect;
            iDirtyRegionCount = 1;
            iPresentScrollX = 0;
            iPresentScrollY = 0;
            return;
        }
        iDirtyRegions[iDirtyRegionCount++].iRect = currentDirty[index].iRect;
    }
}
