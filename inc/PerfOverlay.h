#ifndef EASYRPG_PERF_OVERLAY_H
#define EASYRPG_PERF_OVERLAY_H

#include <e32base.h>
#include <fbs.h>

class CPerfOverlay : public CBase
{
public:
    static CPerfOverlay* NewL();
    ~CPerfOverlay();

    void Reset();
    void FrameRendered();
    TBool IsDirty() const;
    void ClearDirty();
    const CFbsBitmap& Bitmap() const;

private:
    CPerfOverlay();
    void ConstructL();
    TUint32 ReadCounter() const;
    void ReadRam();
    void RebuildBitmap();
    static TUint8 GlyphRow(TUint8 aCharacter, TInt aRow);
    static void DrawText(TUint8* aPixels,
                         TInt aStride,
                         TInt aX,
                         TInt aY,
                         const TDesC8& aText);

private:
    CFbsBitmap* iBitmap;
    RThread iThread;
    TInt iCounterPeriodUs;
    TBool iUseNanoCounter;
    TBool iCpuAvailable;
    TUint32 iLastCounter;
    TInt64 iLastCpuTime;
    TUint32 iFramesSinceSample;
    TInt iFps;
    TInt iCpuPercent;
    TInt iRamUsedTenthsMb;
    TInt iRamTotalTenthsMb;
    TBool iDirty;
};

#endif
