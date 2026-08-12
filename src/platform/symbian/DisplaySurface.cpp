#include <e32std.h>

#include "DisplaySurface.h"
#include "UpstreamPlayerBridge.h"

namespace
{
const TInt KDirtyRegionCapacity = 16;
const TInt KMaximumPresentRegions = 1;
const TInt KDirtyRegionFixedCostDivisor = 12;
const TInt KFullRedrawAreaPercent = 88;

TInt RectArea(const TRect& aRect)
{
    const TInt width = aRect.iBr.iX - aRect.iTl.iX;
    const TInt height = aRect.iBr.iY - aRect.iTl.iY;
    return width > 0 && height > 0 ? width * height : 0;
}

TRect UnionRects(const TRect& aFirst, const TRect& aSecond)
{
    return TRect(Min(aFirst.iTl.iX, aSecond.iTl.iX),
                 Min(aFirst.iTl.iY, aSecond.iTl.iY),
                 Max(aFirst.iBr.iX, aSecond.iBr.iX),
                 Max(aFirst.iBr.iY, aSecond.iBr.iY));
}

TBool RectsEqual(const TRect& aFirst, const TRect& aSecond)
{
    return aFirst.iTl.iX == aSecond.iTl.iX &&
        aFirst.iTl.iY == aSecond.iTl.iY &&
        aFirst.iBr.iX == aSecond.iBr.iX &&
        aFirst.iBr.iY == aSecond.iBr.iY;
}

TInt DirtyRegionArea(const TRect* aRegions, TInt aRegionCount)
{
    TInt area = 0;
    for (TInt index = 0; index < aRegionCount; ++index)
        area += RectArea(aRegions[index]);
    return area;
}

void OptimizeDirtyRegions(TRect* aRegions, TInt& aRegionCount, const TRect& aFrameRect)
{
    if (aRegionCount <= 0)
        return;

    if (aRegionCount > KMaximumPresentRegions)
    {
        TRect merged = aRegions[0];
        for (TInt index = 1; index < aRegionCount; ++index)
            merged = UnionRects(merged, aRegions[index]);
        aRegions[0] = merged;
        aRegionCount = 1;
    }

    TRect combined = aRegions[0];
    for (TInt index = 1; index < aRegionCount; ++index)
        combined = UnionRects(combined, aRegions[index]);

    const TInt frameArea = RectArea(aFrameRect);
    const TInt fixedRegionCost = frameArea / KDirtyRegionFixedCostDivisor;
    const TInt separateCost = DirtyRegionArea(aRegions, aRegionCount) +
        Max(0, aRegionCount - 1) * fixedRegionCost;
    if (RectArea(combined) <= separateCost)
    {
        aRegions[0] = combined;
        aRegionCount = 1;
    }

    if (DirtyRegionArea(aRegions, aRegionCount) * 100 > frameArea * KFullRedrawAreaPercent)
    {
        aRegions[0] = aFrameRect;
        aRegionCount = 1;
    }
}

void CopyRegion(const unsigned short* aSource,
                TInt aSourceStridePixels,
                TUint8* aDestination,
                TInt aDestinationStride,
                const TRect& aRegion)
{
    const TInt rowBytes = (aRegion.iBr.iX - aRegion.iTl.iX) * static_cast<TInt>(sizeof(unsigned short));
    const TInt height = aRegion.iBr.iY - aRegion.iTl.iY;
    if (aRegion.iTl.iX == 0 &&
        rowBytes == aSourceStridePixels * static_cast<TInt>(sizeof(unsigned short)) &&
        rowBytes == aDestinationStride)
    {
        Mem::Copy(aDestination + aRegion.iTl.iY * aDestinationStride,
                  aSource + aRegion.iTl.iY * aSourceStridePixels,
                  rowBytes * height);
        return;
    }

    for (TInt y = aRegion.iTl.iY; y < aRegion.iBr.iY; ++y)
    {
        Mem::Copy(aDestination + y * aDestinationStride +
                      aRegion.iTl.iX * static_cast<TInt>(sizeof(unsigned short)),
                  aSource + y * aSourceStridePixels + aRegion.iTl.iX,
                  rowBytes);
    }
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
      iSize(0, 0),
      iHasFrame(EFalse),
      iDirtyRegionCount(0)
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
    return iDirtyRegions[aIndex];
}

void CDisplaySurface::ClearDirtyRegions()
{
    iDirtyRegionCount = 0;
}

void CDisplaySurface::ClearL()
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
    iDirtyRegionCount = 0;
}

void CDisplaySurface::PresentL()
{
    if (!iBitmap)
        User::Leave(KErrNotReady);

    const unsigned short* source = EasyRpgUpstreamPlayerPixels();
    const TInt sourceWidth = EasyRpgUpstreamPlayerWidth();
    const TInt sourceHeight = EasyRpgUpstreamPlayerHeight();
    const TInt sourceStridePixels = EasyRpgUpstreamPlayerPitchPixels();
    if (!source || sourceWidth != iSize.iWidth || sourceHeight != iSize.iHeight ||
        sourceStridePixels < iSize.iWidth)
        User::Leave(KErrNotReady);

    const TInt coreDirtyCount = EasyRpgUpstreamPlayerDirtyRegionCount();
    if (iHasFrame && coreDirtyCount == 0)
    {
        EasyRpgUpstreamPlayerConsumeDirtyRegions();
        return;
    }

    iBitmap->LockHeap();
    TUint8* destination = reinterpret_cast<TUint8*>(iBitmap->DataAddress());
    if (!destination)
    {
        iBitmap->UnlockHeap();
        User::Leave(KErrNoMemory);
    }

    const TInt destinationStride = iBitmap->DataStride();
    const TRect frameRect(TPoint(0, 0), iSize);
    TRect currentDirty[KDirtyRegionCapacity];
    TInt currentDirtyCount = 0;

    if (!iHasFrame)
    {
        currentDirty[0] = frameRect;
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
                currentDirty[currentDirtyCount++] = TRect(left, top, right, bottom);
        }

        if (coreDirtyValid)
        {
            OptimizeDirtyRegions(currentDirty, currentDirtyCount, frameRect);
        }
        else
        {
            currentDirty[0] = frameRect;
            currentDirtyCount = 1;
        }
    }
    else if (coreDirtyCount > KDirtyRegionCapacity)
    {
        currentDirty[0] = frameRect;
        currentDirtyCount = 1;
    }
    else
    {
        currentDirty[0] = frameRect;
        currentDirtyCount = 1;
    }

    for (TInt index = 0; index < currentDirtyCount; ++index)
        CopyRegion(source, sourceStridePixels, destination, destinationStride, currentDirty[index]);

    if (coreDirtyCount >= 0)
        EasyRpgUpstreamPlayerConsumeDirtyRegions();

    iBitmap->UnlockHeap();
    iHasFrame = ETrue;

    if (currentDirtyCount <= 0)
        return;

    if (currentDirtyCount == 1 && RectsEqual(currentDirty[0], frameRect))
    {
        iDirtyRegions[0] = frameRect;
        iDirtyRegionCount = 1;
        return;
    }

    if (iDirtyRegionCount == 1 && RectsEqual(iDirtyRegions[0], frameRect))
        return;

    for (TInt index = 0; index < currentDirtyCount; ++index)
    {
        if (iDirtyRegionCount >= KDirtyRegionCapacity)
        {
            iDirtyRegions[0] = frameRect;
            iDirtyRegionCount = 1;
            return;
        }
        iDirtyRegions[iDirtyRegionCount++] = currentDirty[index];
    }
    OptimizeDirtyRegions(iDirtyRegions, iDirtyRegionCount, frameRect);
}
