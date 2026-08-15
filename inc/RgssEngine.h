#ifndef EASYRPG_RGSS_ENGINE_H
#define EASYRPG_RGSS_ENGINE_H

#include <e32base.h>
#include <f32file.h>
#include <w32std.h>

#include "FrameTimer.h"

class CRgssDirectScreenAccess;
class CRgssDisplaySurface;
class CRgssRuntime;
class CRgssWindowEventPump;

class CRgssEngine : public CBase,
                    public MFrameTimerObserver
{
public:
    static CRgssEngine* NewL();
    ~CRgssEngine();

    void StartL();
    void RunL();
    void HandleWindowEvent(const TWsEvent& aEvent);
    void FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame);
    void FrameError(TInt aError);

private:
    CRgssEngine();
    void ConstructL();
    void StartDirectScreenL();
    void StopDirectScreen();
    void PresentL(TBool aForceFull);
    void DrawBootGaugeL();
    void HandleKeyEvent(TInt aType, const TKeyEvent& aKey);
    TUint32 MapScanKey(const TKeyEvent& aKey) const;
    TUint32 MapCharacterKey(TInt aCode) const;
    TUint32 FindDynamicScanKey(TInt aScanCode) const;
    void RememberDynamicScanKey(TInt aScanCode, TUint32 aMask);
    void ForgetDynamicScanKey(TInt aScanCode);
    void SetInput(TUint32 aMask, TBool aPressed);
    void ClearInput();

private:
    RFs iFs;
    RWsSession iWs;
    CWsScreenDevice* iScreenDevice;
    RWindowGroup* iWindowGroup;
    RWindow* iWindow;
    CRgssRuntime* iRuntime;
    CFrameTimer* iFrameTimer;
    CRgssWindowEventPump* iEventPump;
    CRgssDirectScreenAccess* iDirectScreen;
    TFileName iGamePath;
    TUint32 iInputWord;
    TUint32 iPendingInputPulses;
    TInt iDynamicScanCodes[16];
    TUint32 iDynamicScanMasks[16];
    TInt iDynamicScanCount;
    TInt iLogicAccumulator;
    TInt iPresentAccumulator;
    TInt iFrameError;
    TBool iForeground;
    TBool iDirectScreenActive;
    TBool iPresentedFrame;
    TInt iBootGaugePhase;
    TInt iBootGaugePercent;
    TUint32 iBootGaugeFrame;
    TBool iBootGaugeActive;
};

#endif
