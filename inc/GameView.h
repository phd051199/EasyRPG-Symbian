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
class CSymbianDisplaySurface;
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
    TUint32 MapPlayerKey(const TKeyEvent& aKeyEvent) const;

private:
    RFs iFs;
    CAknDoubleStyleListBox* iListBox;
    CDesCArrayFlat* iListItems;
    CDesCArrayFlat* iArchivePaths;
    CSymbianDisplaySurface* iDisplaySurface;
    CPerfOverlay* iPerfOverlay;
    CZipVfs* iResources;
    CPlayerRuntime* iRuntime;
    CFrameTimer* iFrameTimer;
    CAsyncCallBack* iStartGameCallback;
    TBool iForeground;
    mutable TBool iFrameDrawPending;
    TFileName iGameRoot;
};

#endif
