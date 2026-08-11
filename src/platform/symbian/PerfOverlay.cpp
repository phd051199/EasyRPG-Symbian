#include <hal.h>
#include <hal_data.h>

#include "PerfOverlay.h"

namespace
{
const TInt KOverlayWidth = 104;
const TInt KOverlayHeight = 19;
const TInt KSampleIntervalUs = 1000000;
const TUint16 KTextColor = 0xffffu;

const TUint8 KGlyph0[7] = {14, 17, 19, 21, 25, 17, 14};
const TUint8 KGlyph1[7] = {4, 12, 4, 4, 4, 4, 14};
const TUint8 KGlyph2[7] = {14, 17, 1, 2, 4, 8, 31};
const TUint8 KGlyph3[7] = {30, 1, 1, 14, 1, 1, 30};
const TUint8 KGlyph4[7] = {2, 6, 10, 18, 31, 2, 2};
const TUint8 KGlyph5[7] = {31, 16, 16, 30, 1, 1, 30};
const TUint8 KGlyph6[7] = {14, 16, 16, 30, 17, 17, 14};
const TUint8 KGlyph7[7] = {31, 1, 2, 4, 8, 8, 8};
const TUint8 KGlyph8[7] = {14, 17, 17, 14, 17, 17, 14};
const TUint8 KGlyph9[7] = {14, 17, 17, 15, 1, 1, 14};
const TUint8 KGlyphA[7] = {14, 17, 17, 31, 17, 17, 17};
const TUint8 KGlyphC[7] = {14, 17, 16, 16, 16, 17, 14};
const TUint8 KGlyphF[7] = {31, 16, 16, 30, 16, 16, 16};
const TUint8 KGlyphM[7] = {17, 27, 21, 21, 17, 17, 17};
const TUint8 KGlyphP[7] = {30, 17, 17, 30, 16, 16, 16};
const TUint8 KGlyphR[7] = {30, 17, 17, 30, 20, 18, 17};
const TUint8 KGlyphS[7] = {15, 16, 16, 14, 1, 1, 30};
const TUint8 KGlyphU[7] = {17, 17, 17, 17, 17, 17, 14};
const TUint8 KGlyphPercent[7] = {25, 25, 2, 4, 8, 19, 19};
const TUint8 KGlyphDot[7] = {0, 0, 0, 0, 0, 6, 6};
const TUint8 KGlyphDash[7] = {0, 0, 0, 14, 0, 0, 0};
const TUint8 KGlyphSlash[7] = {1, 1, 2, 4, 8, 16, 16};

const TUint8* GlyphData(TUint8 aCharacter)
{
    switch (aCharacter)
    {
    case '0': return KGlyph0;
    case '1': return KGlyph1;
    case '2': return KGlyph2;
    case '3': return KGlyph3;
    case '4': return KGlyph4;
    case '5': return KGlyph5;
    case '6': return KGlyph6;
    case '7': return KGlyph7;
    case '8': return KGlyph8;
    case '9': return KGlyph9;
    case 'A': return KGlyphA;
    case 'C': return KGlyphC;
    case 'F': return KGlyphF;
    case 'M': return KGlyphM;
    case 'P': return KGlyphP;
    case 'R': return KGlyphR;
    case 'S': return KGlyphS;
    case 'U': return KGlyphU;
    case '%': return KGlyphPercent;
    case '.': return KGlyphDot;
    case '-': return KGlyphDash;
    case '/': return KGlyphSlash;
    default: return NULL;
    }
}
}

CPerfOverlay* CPerfOverlay::NewL()
{
    CPerfOverlay* self = new (ELeave) CPerfOverlay;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CPerfOverlay::CPerfOverlay()
    : iBitmap(NULL),
      iMaskBitmap(NULL),
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
    iMaskBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iMaskBitmap->Create(TSize(KOverlayWidth, KOverlayHeight), EGray2));

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
    delete iMaskBitmap;
    iMaskBitmap = NULL;
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
    RebuildBitmap();
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
    RebuildBitmap();
}

TBool CPerfOverlay::IsDirty() const
{
    return iDirty;
}

void CPerfOverlay::ClearDirty()
{
    iDirty = EFalse;
}

const CFbsBitmap& CPerfOverlay::Bitmap() const
{
    return *iBitmap;
}

const CFbsBitmap& CPerfOverlay::MaskBitmap() const
{
    return *iMaskBitmap;
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

void CPerfOverlay::RebuildBitmap()
{
    if (!iBitmap || !iMaskBitmap)
        return;

    iBitmap->LockHeap();
    TUint8* pixels = reinterpret_cast<TUint8*>(iBitmap->DataAddress());
    if (!pixels)
    {
        iBitmap->UnlockHeap();
        return;
    }

    const TInt stride = iBitmap->DataStride();
    for (TInt y = 0; y < KOverlayHeight; ++y)
        Mem::FillZ(pixels + y * stride, KOverlayWidth * static_cast<TInt>(sizeof(TUint16)));

    iMaskBitmap->LockHeap();
    TUint8* maskPixels = reinterpret_cast<TUint8*>(iMaskBitmap->DataAddress());
    if (!maskPixels)
    {
        iMaskBitmap->UnlockHeap();
        iBitmap->UnlockHeap();
        return;
    }
    const TInt maskStride = iMaskBitmap->DataStride();
    for (TInt y = 0; y < KOverlayHeight; ++y)
        Mem::FillZ(maskPixels + y * maskStride, maskStride);
    iMaskBitmap->UnlockHeap();

    TBuf8<32> line;
    if (iCpuPercent >= 0)
        line.Format(_L8("FPS %d CPU %d%%"), iFps, iCpuPercent);
    else
        line.Format(_L8("FPS %d CPU --%%"), iFps);
    DrawText(pixels, stride, 2, 2, line);
    DrawMaskText(*iMaskBitmap, 2, 2, line);

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
    DrawText(pixels, stride, 2, 10, line);
    DrawMaskText(*iMaskBitmap, 2, 10, line);

    iBitmap->UnlockHeap();
    iDirty = ETrue;
}

TUint8 CPerfOverlay::GlyphRow(TUint8 aCharacter, TInt aRow)
{
    if (aRow < 0 || aRow >= 7)
        return 0;
    const TUint8* glyph = GlyphData(aCharacter);
    return glyph ? glyph[aRow] : 0;
}

void CPerfOverlay::DrawText(TUint8* aPixels,
                            TInt aStride,
                            TInt aX,
                            TInt aY,
                            const TDesC8& aText)
{
    for (TInt index = 0; index < aText.Length(); ++index)
    {
        const TUint8 character = aText[index];
        if (character != ' ')
        {
            for (TInt row = 0; row < 7; ++row)
            {
                const TUint8 bits = GlyphRow(character, row);
                TUint16* destination = reinterpret_cast<TUint16*>(
                    aPixels + (aY + row) * aStride) + aX + index * 6;
                for (TInt column = 0; column < 5; ++column)
                {
                    if ((bits & (1u << (4 - column))) != 0)
                        destination[column] = KTextColor;
                }
            }
        }
    }
}

void CPerfOverlay::DrawMaskText(CFbsBitmap& aBitmap,
                                TInt aX,
                                TInt aY,
                                const TDesC8& aText)
{
    TBitmapUtil bitmapUtil(&aBitmap);
    bitmapUtil.Begin(TPoint(0, 0));
    for (TInt index = 0; index < aText.Length(); ++index)
    {
        const TUint8 character = aText[index];
        if (character != ' ')
        {
            for (TInt row = 0; row < 7; ++row)
            {
                const TUint8 bits = GlyphRow(character, row);
                for (TInt column = 0; column < 5; ++column)
                {
                    if ((bits & (1u << (4 - column))) != 0)
                    {
                        bitmapUtil.SetPos(TPoint(aX + index * 6 + column, aY + row));
                        bitmapUtil.SetPixel(1);
                    }
                }
            }
        }
    }
    bitmapUtil.End();
}
