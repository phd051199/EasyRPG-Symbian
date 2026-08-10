#ifndef EASYRPG_SYMBIAN_GAME_VIEW_H
#define EASYRPG_SYMBIAN_GAME_VIEW_H

#include <badesca.h>
#include <coecntrl.h>
#include <eiklbo.h>
#include <f32file.h>

#include "easyrpgsymbian/FrameTimer.h"

class CAknSingleStyleListBox;
class CFrameBuffer;
class CPlayerRuntime;
class CZipVfs;

class CGameView : public CCoeControl,
                  public MFrameTimerObserver,
                  public MEikListBoxObserver
{
public:
    static CGameView* NewL(const TRect& aRect);
    ~CGameView();

    void SetForeground(TBool aForeground);

private:
    CGameView();
    void ConstructL(const TRect& aRect);

    void Draw(const TRect& aRect) const;
    void SizeChanged();
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
    void DrawOverlay(CWindowGc& aGc) const;

private:
    RFs iFs;
    CAknSingleStyleListBox* iListBox;
    CDesCArrayFlat* iListItems;
    CDesCArrayFlat* iArchivePaths;
    CFrameBuffer* iFrameBuffer;
    CZipVfs* iResources;
    CPlayerRuntime* iRuntime;
    CFrameTimer* iFrameTimer;
    TBool iForeground;
    TFileName iGameRoot;
};

#endif
