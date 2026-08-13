#ifndef EASYRPG_RUNTIME_ENGINE_H
#define EASYRPG_RUNTIME_ENGINE_H

#include <e32base.h>
#include <f32file.h>
#include <w32std.h>

#include "FrameTimer.h"

class CRawDirectScreenAccess;
class CDisplaySurface;
class CFbsBitGc;
class CPerfOverlay;
class CPlayerRuntime;
class CWindowEventPump;
class CWsScreenDevice;
class CZipVfs;

class CRuntimeEngine : public CBase,
                       public MFrameTimerObserver
{
public:
    static CRuntimeEngine* NewL();
    ~CRuntimeEngine();
    void Run();
    void HandleWindowEvent(const TWsEvent& aEvent);

private:
    CRuntimeEngine();
    void ConstructL();
    void StartDirectScreenL();
    void StopDirectScreen();
    void DestroyDirectScreen();
    void PresentDirectL(TBool aForceFull);
    void RaisePriority();
    void RestorePriority();
    void HandleKeyEvent(TInt aType, const TKeyEvent& aKey);
    TUint32 MapScanKey(const TKeyEvent& aKey) const;
    TUint32 MapCharacterKey(TInt aCode) const;
    TUint32 FindDynamicScanKey(TInt aScanCode) const;
    void RememberDynamicScanKey(TInt aScanCode, TUint32 aPlayerKey);
    void ForgetDynamicScanKey(TInt aScanCode);
    void SetPlayerKeyMask(TUint32 aMask, TBool aPressed);
    void OpenMenu();
    void CloseMenu();
    void MoveMenuSelection(TInt aDelta);
    void ActivateMenuItemL();
    void ToggleSound();
    void AdjustVolume(TInt aDelta);
    void ToggleFrameSkip();
    void TogglePerfOverlay();
    void DrawMenuOverlayL(CFbsBitGc& aGc, CFbsScreenDevice& aScreen);
    void DrawKeyMapOverlayL(CFbsBitGc& aGc, CFbsScreenDevice& aScreen);
    void FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame);
    void FrameError(TInt aError);

private:
    RFs iFs;
    RWsSession iWs;
    RWindowGroup* iWindowGroup;
    RWindow* iWindow;
    CWsScreenDevice* iScreenDevice;
    CDisplaySurface* iDisplaySurface;
    CZipVfs* iResources;
    CPlayerRuntime* iRuntime;
    CFrameTimer* iFrameTimer;
    CWindowEventPump* iEventPump;
    CRawDirectScreenAccess* iDirectScreen;
    CPerfOverlay* iPerfOverlay;
    TFileName iGamePath;
    TBool iDirectScreenActive;
    TBool iNeedsFullRedraw;
    TBool iForeground;
    TBool iPaused;
    TBool iKeyMapVisible;
    TInt iMenuSelection;
    TBool iLeftSoftHeld;
    TBool iRightSoftHeld;
    TUint32 iHeldPlayerKeys;
    TUint32 iPendingPlayerPulses;
    TInt iDynamicPlayerScanCodes[16];
    TUint32 iDynamicPlayerScanKeys[16];
    TInt iDynamicPlayerScanCount;
    TInt iMasterVolume;
    TInt iAudibleVolume;
    TInt iVolumeOverlayTicks;
    TBool iVolumeOverlayErasePending;
    TBool iGameplayPriorityRaised;
    TThreadPriority iPreviousThreadPriority;
    TInt iFrameError;
};

#endif
