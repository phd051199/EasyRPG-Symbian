#include <hal.h>
#include <hal_data.h>

#include "GohuFont.h"
#include "PerfOverlay.h"

namespace
{
const TInt KOverlayWidth = 104;
const TInt KOverlayHeight = 26;
const TInt KSampleIntervalUs = 1000000;
const TUint16 KTextColor = 0xffffu;
const TInt KGlyphWidth = 6;
const TInt KGlyphHeight = 11;
} // namespace

CPerfOverlay *CPerfOverlay::NewL()
{
    CPerfOverlay *self = new (ELeave) CPerfOverlay;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CPerfOverlay::CPerfOverlay()
    : iBitmap(NULL),
      iCounterPeriodUs(0),
      iUseNanoCounter(ETrue),
      iCpuAvailable(EFalse),
      iLastCounter(0),
      iLastCpuTime(0),
      iFramesSinceSample(0),
      iFps(0),
      iCpuPercent(0),
      iRamUsedTenthsMb(-1),
      iRamTotalTenthsMb(-1),
      iDirty(EFalse)
{
}

void CPerfOverlay::ConstructL()
{
    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iBitmap->Create(TSize(KOverlayWidth, KOverlayHeight), EColor64K));

    TInt period = 0;
    if (HAL::Get(HALData::ENanoTickPeriod, period) == KErrNone && period > 0)
    {
        iUseNanoCounter = ETrue;
        iCounterPeriodUs = period;
    }
    else if (HAL::Get(HALData::ESystemTickPeriod, period) == KErrNone && period > 0)
    {
        iUseNanoCounter = EFalse;
        iCounterPeriodUs = period;
    }
    else
    {
        User::Leave(KErrNotSupported);
    }

    if (iThread.Open(RThread().Id()) == KErrNone)
    {
        TTimeIntervalMicroSeconds cpuTime;
        if (iThread.GetCpuTime(cpuTime) == KErrNone)
        {
            iCpuAvailable = ETrue;
            iLastCpuTime = cpuTime.Int64();
        }
        else
        {
            iThread.Close();
        }
    }

    Reset();
}

CPerfOverlay::~CPerfOverlay()
{
    iThread.Close();
    delete iBitmap;
    iBitmap = NULL;
}

void CPerfOverlay::Reset()
{
    iLastCounter = ReadCounter();
    iFramesSinceSample = 0;
    iFps = 0;
    iCpuPercent = iCpuAvailable ? 0 : -1;

    if (iCpuAvailable)
    {
        TTimeIntervalMicroSeconds cpuTime;
        if (iThread.GetCpuTime(cpuTime) == KErrNone)
            iLastCpuTime = cpuTime.Int64();
        else
            iCpuPercent = -1;
    }

    ReadRam();
    iDirty = ETrue;
}

void CPerfOverlay::FrameRendered()
{
    ++iFramesSinceSample;
    const TUint32 currentCounter = ReadCounter();
    const TUint32 elapsedTicks = currentCounter - iLastCounter;
    const TInt64 elapsedUs = static_cast<TInt64>(elapsedTicks) * iCounterPeriodUs;
    if (elapsedUs < KSampleIntervalUs)
        return;

    const TInt64 framesScaled = static_cast<TInt64>(iFramesSinceSample) * 1000000;
    iFps = static_cast<TInt>((framesScaled + elapsedUs / 2) / elapsedUs);

    if (iCpuAvailable)
    {
        TTimeIntervalMicroSeconds cpuTime;
        if (iThread.GetCpuTime(cpuTime) == KErrNone)
        {
            const TInt64 currentCpuTime = cpuTime.Int64();
            TInt64 cpuDelta = currentCpuTime - iLastCpuTime;
            if (cpuDelta < 0)
                cpuDelta = 0;
            TInt cpuPercent = static_cast<TInt>((cpuDelta * 100 + elapsedUs / 2) / elapsedUs);
            if (cpuPercent < 0)
                cpuPercent = 0;
            if (cpuPercent > 100)
                cpuPercent = 100;
            iCpuPercent = cpuPercent;
            iLastCpuTime = currentCpuTime;
        }
        else
        {
            iCpuPercent = -1;
        }
    }

    ReadRam();
    iLastCounter = currentCounter;
    iFramesSinceSample = 0;
    iDirty = ETrue;
}

TBool CPerfOverlay::IsDirty() const
{
    return iDirty;
}

void CPerfOverlay::ClearDirty()
{
    iDirty = EFalse;
}

void CPerfOverlay::Compose(const CFbsBitmap &aFrameBitmap,
                           TInt aX,
                           TInt aY)
{
    if (!iBitmap)
        return;

    const TSize frameSize = aFrameBitmap.SizeInPixels();
    if (aX < 0 || aY < 0 ||
        aX + KOverlayWidth > frameSize.iWidth ||
        aY + KOverlayHeight > frameSize.iHeight)
        return;

    aFrameBitmap.LockHeap();
    TUint8 *overlayPixels = reinterpret_cast<TUint8 *>(iBitmap->DataAddress());
    const TUint8 *framePixels = reinterpret_cast<const TUint8 *>(aFrameBitmap.DataAddress());
    const TInt overlayStride = iBitmap->DataStride();
    const TInt frameStride = aFrameBitmap.DataStride();
    if (overlayPixels && framePixels && overlayStride > 0 && frameStride > 0)
    {
        for (TInt y = 0; y < KOverlayHeight; ++y)
        {
            Mem::Copy(overlayPixels + y * overlayStride,
                      framePixels + (aY + y) * frameStride + aX * 2,
                      KOverlayWidth * 2);
        }
        DrawMetricsTo(overlayPixels, overlayStride, 0, 0);
    }
    aFrameBitmap.UnlockHeap();
}

const CFbsBitmap &CPerfOverlay::Bitmap() const
{
    return *iBitmap;
}

TUint32 CPerfOverlay::ReadCounter() const
{
    return iUseNanoCounter ? User::NTickCount() : User::TickCount();
}

void CPerfOverlay::ReadRam()
{
    TInt totalRam = 0;
    TInt freeRam = 0;
    if (HAL::Get(HALData::EMemoryRAM, totalRam) == KErrNone &&
        HAL::Get(HALData::EMemoryRAMFree, freeRam) == KErrNone &&
        totalRam > 0 && freeRam >= 0 && freeRam <= totalRam)
    {
        const TInt usedRam = totalRam - freeRam;
        iRamUsedTenthsMb = static_cast<TInt>(
            (static_cast<TInt64>(usedRam) * 10) / (1024 * 1024));
        iRamTotalTenthsMb = static_cast<TInt>(
            (static_cast<TInt64>(totalRam) * 10) / (1024 * 1024));
    }
    else
    {
        iRamUsedTenthsMb = -1;
        iRamTotalTenthsMb = -1;
    }
}

void CPerfOverlay::DrawMetricsTo(TUint8 *aPixels,
                                 TInt aStride,
                                 TInt aX,
                                 TInt aY)
{
    if (!aPixels || aStride <= 0)
        return;

    TBuf8<32> line;
    if (iCpuPercent >= 0)
        line.Format(_L8("FPS %d CPU %d%%"), iFps, iCpuPercent);
    else
        line.Format(_L8("FPS %d CPU --%%"), iFps);
    DrawText(aPixels, aStride, aX + 2, aY + 2, line);

    line.Zero();
    if (iRamUsedTenthsMb >= 0 && iRamTotalTenthsMb >= 0)
    {
        line.Format(_L8("RAM %d.%d/%d.%dM"),
                    iRamUsedTenthsMb / 10,
                    iRamUsedTenthsMb % 10,
                    iRamTotalTenthsMb / 10,
                    iRamTotalTenthsMb % 10);
    }
    else
    {
        line.Copy(_L8("RAM --.-/--.-M"));
    }
    DrawText(aPixels, aStride, aX + 2, aY + 15, line);
}

void CPerfOverlay::DrawText(TUint8 *aPixels,
                            TInt aStride,
                            TInt aX,
                            TInt aY,
                            const TDesC8 &aText)
{
    for (TInt index = 0; index < aText.Length(); ++index)
    {
        const TUint8 character = aText[index];
        if (character == ' ')
            continue;
        const TUint8 *rows = EasyRpgGohuGlyphRows(character);
        if (!rows)
            continue;
        for (TInt row = 0; row < KGlyphHeight; ++row)
        {
            const TUint8 bits = rows[row];
            TUint16 *destination = reinterpret_cast<TUint16 *>(
                                       aPixels + (aY + row) * aStride) +
                                   aX + index * KGlyphWidth;
            for (TInt column = 0; column < KGlyphWidth; ++column)
            {
                if ((bits & (1u << column)) != 0)
                    destination[column] = KTextColor;
            }
        }
    }
}
