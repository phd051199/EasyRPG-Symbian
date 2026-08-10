#ifndef EASYRPG_SYMBIAN_GAME_VIEW_H
#define EASYRPG_SYMBIAN_GAME_VIEW_H

#include <coecntrl.h>
#include <f32file.h>

#include "easyrpgsymbian/FrameTimer.h"

class CFrameBuffer;
class CPlayerRuntime;

class CGameView : public CCoeControl, public MFrameTimerObserver
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
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
    void FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame);

    TUint32 MapPlayerKey(const TKeyEvent& aKeyEvent) const;
    TPoint FrameOrigin() const;
    void DrawOverlay(CWindowGc& aGc) const;

private:
    RFs iFs;
    CFrameBuffer* iFrameBuffer;
    CPlayerRuntime* iRuntime;
    CFrameTimer* iFrameTimer;
    TBool iForeground;
    TFileName iGameRoot;
};

#endif
