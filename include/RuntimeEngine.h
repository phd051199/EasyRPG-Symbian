#ifndef EASYRPG_RUNTIME_ENGINE_H
#define EASYRPG_RUNTIME_ENGINE_H

#include <e32base.h>
#include <f32file.h>
#include <w32std.h>

#include "FrameTimer.h"
#include "GameMenu.h"

class CRawDirectScreenAccess;
class CDisplaySurface;
class CFbsBitGc;
class CPerfOverlay;
class CPlayerRuntime;
class CWindowEventPump;
class CWsScreenDevice;
class CZipVfs;

class CRuntimeEngine : public CBase,
                       public MFrameTimerObserver,
                       public MGameMenuHost
{
  public:
    static CRuntimeEngine *NewL();
    ~CRuntimeEngine();
    void Run();
    TBool ReturnToLauncher() const;
    void HandleWindowEvent(const TWsEvent &aEvent);

  private:
    CRuntimeEngine();
    void ConstructL();
    void StartDirectScreenL();
    void StopDirectScreen();
    void DestroyDirectScreen();
    void PresentDirectL(TBool aForceFull);
    void RaisePriority();
    void RestorePriority();
    void RequestExit(TBool aReturnToLauncher);
    void HandleKeyEvent(TInt aType, const TKeyEvent &aKey);
    void SetPlayerKeyMask(TUint32 aMask, TBool aPressed);
    void ClearPlayerInput();
    void MaintainMemory(TUint32 aLogicFrame);
    TUint32 GameMenuFeatures() const;
    void GameMenuSetSoundEnabledL(TBool aEnabled);
    void GameMenuSetVolumeL(TInt aVolume);
    void GameMenuSetFpsEnabledL(TBool aEnabled);
    void GameMenuPauseL(TBool aPaused);
    void GameMenuExitL();
    void GameMenuRequestRedraw();
    void FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame);
    void FrameError(TInt aError);

  private:
    RFs iFs;
    RWsSession iWs;
    RWindowGroup *iWindowGroup;
    RWindow *iWindow;
    CWsScreenDevice *iScreenDevice;
    CDisplaySurface *iDisplaySurface;
    CZipVfs *iResources;
    CPlayerRuntime *iRuntime;
    CFrameTimer *iFrameTimer;
    CWindowEventPump *iEventPump;
    CRawDirectScreenAccess *iDirectScreen;
    CPerfOverlay *iPerfOverlay;
    CGameMenu *iGameMenu;
    TFileName iGamePath;
    TBool iDirectScreenActive;
    TBool iNeedsFullRedraw;
    TBool iForeground;
    TBool iPaused;
    TBool iReturnToLauncher;
    TBool iRightSoftHeld;
    TUint32 iHeldPlayerKeys;
    TUint32 iPendingPlayerPulses;
    TUint32 iDirectScreenRetryFrame;
    TBool iGameplayPriorityRaised;
    TThreadPriority iPreviousThreadPriority;
    TInt iFrameError;
    TInt iMemoryPressureLevel;
};

#endif
