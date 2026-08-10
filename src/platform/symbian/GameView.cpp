#include <coemain.h>
#include <e32keys.h>
#include <gdi.h>
#include <w32std.h>

#include "easyrpgsymbian/Framebuffer.h"
#include "easyrpgsymbian/GameLocator.h"
#include "easyrpgsymbian/GameView.h"
#include "easyrpgsymbian/PlayerRuntime.h"

namespace
{
const TInt KGameWidth = 320;
const TInt KGameHeight = 240;
}

CGameView* CGameView::NewL(const TRect& aRect)
{
    CGameView* self = new (ELeave) CGameView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop(self);
    return self;
}

CGameView::CGameView()
    : iFrameBuffer(NULL),
      iRuntime(NULL),
      iFrameTimer(NULL),
      iForeground(ETrue)
{
}

void CGameView::ConstructL(const TRect& aRect)
{
    CreateWindowL();
    SetRect(aRect);
    SetFocus(ETrue);

    User::LeaveIfError(iFs.Connect());
    iFrameBuffer = CFrameBuffer::NewL(TSize(KGameWidth, KGameHeight));

    if (TGameLocator::FindNobihazaRoot(iFs, iGameRoot) != KErrNone)
        iGameRoot.Zero();

    iRuntime = CPlayerRuntime::NewL(iFs, iGameRoot, *iFrameBuffer);
    iFrameTimer = CFrameTimer::NewL(*this);

    ActivateL();
    iFrameTimer->Start();
}

CGameView::~CGameView()
{
    if (iFrameTimer)
        iFrameTimer->Stop();

    delete iFrameTimer;
    iFrameTimer = NULL;

    delete iRuntime;
    iRuntime = NULL;

    delete iFrameBuffer;
    iFrameBuffer = NULL;

    iFs.Close();
}

void CGameView::SetForeground(TBool aForeground)
{
    iForeground = aForeground;

    if (!iRuntime || !iFrameTimer)
        return;

    if (!aForeground)
    {
        iRuntime->ReleaseAllKeys();
        iFrameTimer->Stop();
    }
    else
    {
        iFrameTimer->Start();
        DrawNow();
    }
}

void CGameView::Draw(const TRect& /*aRect*/) const
{
    CWindowGc& gc = SystemGc();
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.SetBrushColor(TRgb(0, 0, 0));
    gc.DrawRect(Rect());

    if (iFrameBuffer)
        gc.BitBlt(FrameOrigin(), &iFrameBuffer->Bitmap());

    DrawOverlay(gc);
}

void CGameView::SizeChanged()
{
    // The internal RPG Maker surface intentionally remains 320x240. The E72
    // is a native 320x240 target; other emulator screen sizes are centered.
}

void CGameView::FocusChanged(TDrawNow aDrawNow)
{
    CCoeControl::FocusChanged(aDrawNow);
    if (!IsFocused() && iRuntime)
        iRuntime->ReleaseAllKeys();
}

TKeyResponse CGameView::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    if (!iRuntime)
        return EKeyWasNotConsumed;

    const TUint32 playerKey = MapPlayerKey(aKeyEvent);
    if (playerKey == 0)
        return EKeyWasNotConsumed;

    if (aType == EEventKeyDown)
    {
        iRuntime->SetKeyState(playerKey, ETrue);
        return EKeyWasConsumed;
    }

    if (aType == EEventKeyUp)
    {
        iRuntime->SetKeyState(playerKey, EFalse);
        return EKeyWasConsumed;
    }

    return EKeyWasNotConsumed;
}

void CGameView::FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame)
{
    if (!iForeground || !iRuntime)
        return;

    iRuntime->TickL(aLogicFrame, aRenderFrame);
    if (aRenderFrame)
        DrawNow();
}

TUint32 CGameView::MapPlayerKey(const TKeyEvent& aKeyEvent) const
{
    switch (aKeyEvent.iScanCode)
    {
    case EStdKeyUpArrow:
        return CPlayerRuntime::EKeyUp;
    case EStdKeyDownArrow:
        return CPlayerRuntime::EKeyDown;
    case EStdKeyLeftArrow:
        return CPlayerRuntime::EKeyLeft;
    case EStdKeyRightArrow:
        return CPlayerRuntime::EKeyRight;
    case EStdKeyEnter:
    case EStdKeyDevice3:
        return CPlayerRuntime::EKeyDecision;
    case EStdKeyEscape:
    case EStdKeyBackspace:
        return CPlayerRuntime::EKeyCancel;
    case EStdKeyLeftShift:
    case EStdKeyRightShift:
        return CPlayerRuntime::EKeyShift;
    default:
        break;
    }

    switch (aKeyEvent.iCode)
    {
    case EKeyUpArrow:
        return CPlayerRuntime::EKeyUp;
    case EKeyDownArrow:
        return CPlayerRuntime::EKeyDown;
    case EKeyLeftArrow:
        return CPlayerRuntime::EKeyLeft;
    case EKeyRightArrow:
        return CPlayerRuntime::EKeyRight;
    case EKeyEnter:
    case EKeyDevice3:
    case 'z':
    case 'Z':
    case ' ':
        return CPlayerRuntime::EKeyDecision;
    case EKeyEscape:
    case EKeyBackspace:
    case 'x':
    case 'X':
        return CPlayerRuntime::EKeyCancel;
    case 'a':
    case 'A':
        return CPlayerRuntime::EKeyShift;
    case '1':
        return CPlayerRuntime::EKeyDebug;
    default:
        break;
    }

    return 0;
}

TPoint CGameView::FrameOrigin() const
{
    const TRect rect = Rect();
    return TPoint(rect.iTl.iX + (rect.Width() - KGameWidth) / 2,
                  rect.iTl.iY + (rect.Height() - KGameHeight) / 2);
}

void CGameView::DrawOverlay(CWindowGc& aGc) const
{
    if (!iRuntime || !iCoeEnv)
        return;

    const CFont* font = iCoeEnv->NormalFont();
    if (!font)
        return;

    aGc.UseFont(font);
    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(235, 235, 235));
    aGc.SetBrushStyle(CGraphicsContext::ENullBrush);

    const TPoint origin = FrameOrigin();
    const TInt baseline = origin.iY + font->HeightInPixels() + 12;
    aGc.DrawText(iRuntime->StatusText(), TPoint(origin.iX + 8, baseline));

    if (iRuntime->GameRoot().Length() > 0)
    {
        aGc.SetPenColor(TRgb(190, 205, 220));
        if (iRuntime->GameTitle().Length() > 0)
            aGc.DrawText(iRuntime->GameTitle(), TPoint(origin.iX + 8, baseline + font->HeightInPixels() + 2));
        else
            aGc.DrawText(iRuntime->GameRoot(), TPoint(origin.iX + 8, baseline + font->HeightInPixels() + 2));
    }
    else
    {
        _LIT(KCopyHint, "Copy game to E:\\EasyRPG\\Games\\NobihazaVH\\");
        aGc.SetPenColor(TRgb(220, 175, 105));
        aGc.DrawText(KCopyHint, TPoint(origin.iX + 8, baseline + font->HeightInPixels() + 2));
    }

    aGc.DiscardFont();
}
