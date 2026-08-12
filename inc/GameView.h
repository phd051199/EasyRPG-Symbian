#ifndef EASYRPG_GAME_VIEW_H
#define EASYRPG_GAME_VIEW_H

#include <badesca.h>
#include <coecntrl.h>
#include <eiklbo.h>
#include <f32file.h>

#include "FrameTimer.h"

class CAknDoubleStyleListBox;
class CAsyncCallBack;
class CPerfOverlay;
class CPlayerRuntime;
class CDisplaySurface;
class CZipVfs;

class CGameView : public CCoeControl,
                  public MFrameTimerObserver,
                  public MEikListBoxObserver
{
public:
    static CGameView* NewL(const TRect& aRect);
    ~CGameView();

    void SetForeground(TBool aForeground);
    void UpdateLayout();
    TBool IsGameRunning() const;
    void HandleLeftSoftkeyL();
    void HandleRightSoftkeyL();
    void PlaySelectedGameL();
    TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

private:
    CGameView();
    void ConstructL(const TRect& aRect);

    void Draw(const TRect& aRect) const;
    void SizeChanged();
    void HandleResourceChange(TInt aType);
    void FocusChanged(TDrawNow aDrawNow);
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
    void HandleListBoxEventL(CEikListBox* aListBox,
                             TListBoxEvent aEventType);
    void FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame);
    void FrameError(TInt aError);
    static TInt StartGameCallback(TAny* aPtr);

    void BuildGameListL();
    void StartSelectedGameL();
    void StopGame();
    void ShowLoadErrorL(TInt aError);
    void OpenGameMenu();
    void CloseGameMenu();
    void MoveGameMenuSelection(TInt aDelta);
    void ActivateGameMenuItemL();
    void ToggleSound();
    void ToggleFrameSkip();
    void RestartGameL();
    void ToggleFpsOverlayL();
    void AdjustMasterVolume(TInt aDelta);
    void DrawGameMenuOverlay(CWindowGc& aGc) const;
    void DrawKeyMapOverlay(CWindowGc& aGc) const;
    void DrawVolumeOverlay(CWindowGc& aGc, const TPoint& aDestination) const;
    TUint32 MapPlayerScanKey(const TKeyEvent& aKeyEvent) const;
    TUint32 MapPlayerCharacterKey(TInt aCode) const;
    TUint32 FindDynamicPlayerScanKey(TInt aScanCode) const;
    void RememberDynamicPlayerScanKey(TInt aScanCode, TUint32 aPlayerKey);
    void ForgetDynamicPlayerScanKey(TInt aScanCode);

private:
    RFs iFs;
    CAknDoubleStyleListBox* iListBox;
    CDesCArrayFlat* iListItems;
    CDesCArrayFlat* iArchivePaths;
    CDisplaySurface* iDisplaySurface;
    CPerfOverlay* iPerfOverlay;
    CZipVfs* iResources;
    CPlayerRuntime* iRuntime;
    CFrameTimer* iFrameTimer;
    CAsyncCallBack* iStartGameCallback;
    TBool iForeground;
    TBool iKeyMapVisible;
    TBool iGameMenuActive;
    TInt iGameMenuSelection;
    TBool iLeftSoftkeyHeld;
    TBool iRightSoftkeyHeld;
    TBool iPendingCancelPulse;
    TUint32 iHeldPlayerKeys;
    TUint32 iPendingPlayerKeyPulses;
    TInt iDynamicPlayerScanCodes[16];
    TUint32 iDynamicPlayerScanKeys[16];
    TInt iDynamicPlayerScanCount;
    TBool iVolumeUpHeld;
    TBool iVolumeDownHeld;
    mutable TBool iFrameDrawPending;
    mutable TBool iVolumeOverlayErasePending;
    TInt iMasterVolume;
    TInt iAudibleVolume;
    TInt iVolumeOverlayTicks;
    TFileName iGameRoot;
};

#endif
