#include <e32std.h>

#include "easyrpgsymbian/DisplaySurface.h"
#include "easyrpgsymbian/UpstreamPlayerBridge.h"

namespace
{
const TInt KDirtyTileSize = 16;
const TInt KDirtyRegionCapacity = 16;
const TInt KMaximumPresentRegions = 4;
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

void RemoveDirtyRegion(TRect* aRegions, TInt& aRegionCount, TInt aIndex)
{
    for (TInt index = aIndex + 1; index < aRegionCount; ++index)
        aRegions[index - 1] = aRegions[index];
    --aRegionCount;
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

    while (aRegionCount > KMaximumPresentRegions)
    {
        TInt selectedFirst = 0;
        TInt selectedSecond = 1;
        TInt selectedIncrease = 0x7fffffff;
        for (TInt first = 0; first < aRegionCount - 1; ++first)
        {
            for (TInt second = first + 1; second < aRegionCount; ++second)
            {
                const TRect merged = UnionRects(aRegions[first], aRegions[second]);
                const TInt increase = RectArea(merged) -
                    RectArea(aRegions[first]) - RectArea(aRegions[second]);
                if (increase < selectedIncrease)
                {
                    selectedFirst = first;
                    selectedSecond = second;
                    selectedIncrease = increase;
                }
            }
        }
        aRegions[selectedFirst] = UnionRects(aRegions[selectedFirst], aRegions[selectedSecond]);
        RemoveDirtyRegion(aRegions, aRegionCount, selectedSecond);
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

TBool TileChanged(const unsigned short* aSource,
                  TInt aSourceStridePixels,
                  const TUint8* aDestination,
                  TInt aDestinationStride,
                  TInt aX,
                  TInt aY,
                  TInt aWidth,
                  TInt aHeight)
{
    for (TInt row = 0; row < aHeight; ++row)
    {
        const unsigned short* source = aSource + (aY + row) * aSourceStridePixels + aX;
        const TUint16* destination = reinterpret_cast<const TUint16*>(
            aDestination + (aY + row) * aDestinationStride) + aX;
        for (TInt column = 0; column < aWidth; ++column)
        {
            if (source[column] != destination[column])
                return ETrue;
        }
    }
    return EFalse;
}

void AddDirtyRun(TRect* aRegions,
                 TInt& aRegionCount,
                 TInt aX1,
                 TInt aY1,
                 TInt aX2,
                 TInt aY2)
{
    for (TInt index = 0; index < aRegionCount; ++index)
    {
        TRect& region = aRegions[index];
        if (region.iTl.iX == aX1 && region.iBr.iX == aX2 && region.iBr.iY == aY1)
        {
            region.iBr.iY = aY2;
            return;
        }
    }

    if (aRegionCount < KDirtyRegionCapacity)
        aRegions[aRegionCount++] = TRect(aX1, aY1, aX2, aY2);
}

void DetectDirtyRegions(const unsigned short* aSource,
                        TInt aSourceStridePixels,
                        const TUint8* aDestination,
                        TInt aDestinationStride,
                        const TSize& aSize,
                        TRect* aRegions,
                        TInt& aRegionCount)
{
    const TRect frameRect(TPoint(0, 0), aSize);
    aRegionCount = 0;

    for (TInt y = 0; y < aSize.iHeight; y += KDirtyTileSize)
    {
        const TInt tileHeight = Min(KDirtyTileSize, aSize.iHeight - y);
        TInt runStart = -1;
        for (TInt x = 0; x < aSize.iWidth; x += KDirtyTileSize)
        {
            const TInt tileWidth = Min(KDirtyTileSize, aSize.iWidth - x);
            const TBool changed = TileChanged(aSource,
                                              aSourceStridePixels,
                                              aDestination,
                                              aDestinationStride,
                                              x,
                                              y,
                                              tileWidth,
                                              tileHeight);
            if (changed && runStart < 0)
                runStart = x;

            const TBool runEnds = runStart >= 0 && (!changed || x + tileWidth >= aSize.iWidth);
            if (runEnds)
            {
                const TInt runEnd = changed ? x + tileWidth : x;
                AddDirtyRun(aRegions, aRegionCount, runStart, y, runEnd, y + tileHeight);
                if (aRegionCount >= KDirtyRegionCapacity)
                {
                    aRegions[0] = frameRect;
                    aRegionCount = 1;
                    return;
                }
                runStart = -1;
            }
        }
    }

    OptimizeDirtyRegions(aRegions, aRegionCount, frameRect);
}

void CopyRegion(const unsigned short* aSource,
                TInt aSourceStridePixels,
                TUint8* aDestination,
                TInt aDestinationStride,
                const TRect& aRegion)
{
    const TInt rowBytes = (aRegion.iBr.iX - aRegion.iTl.iX) * static_cast<TInt>(sizeof(unsigned short));
    for (TInt y = aRegion.iTl.iY; y < aRegion.iBr.iY; ++y)
    {
        Mem::Copy(aDestination + y * aDestinationStride +
                      aRegion.iTl.iX * static_cast<TInt>(sizeof(unsigned short)),
                  aSource + y * aSourceStridePixels + aRegion.iTl.iX,
                  rowBytes);
    }
}
}

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
      iHasFrame(EFalse),
      iDirtyRegionCount(0)
{
}

void CSymbianDisplaySurface::ConstructL(const TSize& aSize)
{
    if (aSize.iWidth <= 0 || aSize.iHeight <= 0)
        User::Leave(KErrArgument);

    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iBitmap->Create(aSize, EColor64K));
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

TBool CSymbianDisplaySurface::HasDirtyRegions() const
{
    return iDirtyRegionCount > 0;
}

TInt CSymbianDisplaySurface::DirtyRegionCount() const
{
    return iDirtyRegionCount;
}

const TRect& CSymbianDisplaySurface::DirtyRegion(TInt aIndex) const
{
    return iDirtyRegions[aIndex];
}

void CSymbianDisplaySurface::ClearDirtyRegions()
{
    iDirtyRegionCount = 0;
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
    iDirtyRegionCount = 0;
}

void CSymbianDisplaySurface::PresentL()
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
    const TInt coreDirtyCount = EasyRpgUpstreamPlayerDirtyRegionCount();

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
            DetectDirtyRegions(source,
                               sourceStridePixels,
                               destination,
                               destinationStride,
                               iSize,
                               currentDirty,
                               currentDirtyCount);
        }
    }
    else if (coreDirtyCount > KDirtyRegionCapacity)
    {
        currentDirty[0] = frameRect;
        currentDirtyCount = 1;
    }
    else
    {
        DetectDirtyRegions(source,
                           sourceStridePixels,
                           destination,
                           destinationStride,
                           iSize,
                           currentDirty,
                           currentDirtyCount);
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
