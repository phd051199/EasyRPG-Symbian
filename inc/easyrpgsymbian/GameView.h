#ifndef EASYRPG_SYMBIAN_GAME_VIEW_H
#define EASYRPG_SYMBIAN_GAME_VIEW_H

#include <badesca.h>
#include <coecntrl.h>
#include <eiklbo.h>
#include <f32file.h>

#include "easyrpgsymbian/FrameTimer.h"

class CAknSingleStyleListBox;
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

    void BuildGameListL();
    void StartSelectedGameL();
    void ShowLoadErrorL(TInt aError);
    TUint32 MapPlayerKey(const TKeyEvent& aKeyEvent) const;
    TPoint FrameOrigin() const;

private:
    RFs iFs;
    CAknSingleStyleListBox* iListBox;
    CDesCArrayFlat* iListItems;
    CDesCArrayFlat* iArchivePaths;
    CSymbianDisplaySurface* iDisplaySurface;
    CZipVfs* iResources;
    CPlayerRuntime* iRuntime;
    CFrameTimer* iFrameTimer;
    TBool iForeground;
    TFileName iGameRoot;
};

#endif
