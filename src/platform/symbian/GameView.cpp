#include <aknlists.h>
#include <aknnotewrappers.h>
#include <coemain.h>
#include <e32keys.h>
#include <eikclbd.h>
#include <eikscrlb.h>
#include <gdi.h>
#include <w32std.h>

#include "easyrpgsymbian/Framebuffer.h"
#include "easyrpgsymbian/GameLocator.h"
#include "easyrpgsymbian/GameView.h"
#include "easyrpgsymbian/PlayerRuntime.h"
#include "easyrpgsymbian/ZipVfs.h"

namespace
{
const TInt KGameWidth = 320;
const TInt KGameHeight = 240;
_LIT(KSingleListItemFormat, "\t%S");
_LIT(KNoGamesFound, "No games found");
_LIT8(KTargetArchivePassword, "nbhzvn");

void CopyListText(const TDesC& aSource, TDes& aTarget)
{
    aTarget.Copy(aSource.Left(aTarget.MaxLength()));
    for (TInt index = 0; index < aTarget.Length(); ++index)
    {
        if (aTarget[index] == '\t' || aTarget[index] == '\r' ||
            aTarget[index] == '\n')
        {
            aTarget[index] = ' ';
        }
    }
}
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
    : iListBox(NULL),
      iListItems(NULL),
      iArchivePaths(NULL),
      iFrameBuffer(NULL),
      iResources(NULL),
      iRuntime(NULL),
      iFrameTimer(NULL),
      iForeground(ETrue)
{
}

void CGameView::ConstructL(const TRect& aRect)
{
    CreateWindowL();
    User::LeaveIfError(iFs.Connect());

    iListBox = new (ELeave) CAknSingleStyleListBox;
    iListBox->SetContainerWindowL(*this);
    iListBox->ConstructL(this, EAknListBoxSelectionList);
    iListBox->SetListBoxObserver(this);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

    iListItems = new (ELeave) CDesCArrayFlat(16);
    iArchivePaths = new (ELeave) CDesCArrayFlat(16);
    iListBox->Model()->SetItemTextArray(iListItems);
    iListBox->Model()->SetOwnershipType(ELbmDoesNotOwnItemArray);

    BuildGameListL();

    SetRect(aRect);
    SetFocus(ETrue);
    ActivateL();
    iListBox->ActivateL();
    iListBox->SetFocus(ETrue);
}

CGameView::~CGameView()
{
    if (iFrameTimer)
        iFrameTimer->Stop();

    delete iFrameTimer;
    iFrameTimer = NULL;

    delete iRuntime;
    iRuntime = NULL;

    delete iResources;
    iResources = NULL;

    delete iFrameBuffer;
    iFrameBuffer = NULL;

    delete iListBox;
    iListBox = NULL;

    delete iListItems;
    iListItems = NULL;

    delete iArchivePaths;
    iArchivePaths = NULL;

    iFs.Close();
}

void CGameView::BuildGameListL()
{
    iListItems->Reset();
    iArchivePaths->Reset();

    CDesCArrayFlat* displayNames = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(displayNames);
    TGameLocator::ListGameArchivesL(iFs, *displayNames, *iArchivePaths);

    for (TInt index = 0; index < displayNames->Count(); ++index)
    {
        TBuf<KMaxFileName> title;
        CopyListText((*displayNames)[index], title);

        TBuf<KMaxFileName + 8> item;
        item.Format(KSingleListItemFormat, &title);
        iListItems->AppendL(item);
    }

    if (iListItems->Count() == 0)
    {
        TBuf<64> item;
        item.Format(KSingleListItemFormat, &KNoGamesFound);
        iListItems->AppendL(item);
    }

    CleanupStack::PopAndDestroy(displayNames);

    iListBox->HandleItemAdditionL();
    iListBox->SetCurrentItemIndex(0);
}

void CGameView::StartSelectedGameL()
{
    if (iRuntime || !iListBox || !iArchivePaths)
        return;

    const TInt index = iListBox->CurrentItemIndex();
    if (index < 0 || index >= iArchivePaths->Count())
        return;

    TFileName selectedArchive;
    selectedArchive.Copy((*iArchivePaths)[index]);

    // Nothing game-specific is allocated or mounted before this point. The
    // list screen is deliberately filesystem-enumeration only.
    CFrameBuffer* frameBuffer = CFrameBuffer::NewL(TSize(KGameWidth, KGameHeight));
    CleanupStack::PushL(frameBuffer);

    // The first compatibility target uses this AES password. Supplying it is
    // harmless for ordinary unencrypted ZIP archives.
    CZipVfs* resources = CZipVfs::OpenL(iFs,
                                        selectedArchive,
                                        KTargetArchivePassword);
    CleanupStack::PushL(resources);

    CPlayerRuntime* runtime = CPlayerRuntime::NewL(resources,
                                                   selectedArchive,
                                                   *frameBuffer);
    CleanupStack::PushL(runtime);

    CFrameTimer* frameTimer = CFrameTimer::NewL(*this);
    CleanupStack::PushL(frameTimer);

    CleanupStack::Pop(frameTimer);
    CleanupStack::Pop(runtime);
    CleanupStack::Pop(resources);
    CleanupStack::Pop(frameBuffer);

    iFrameBuffer = frameBuffer;
    iResources = resources;
    iRuntime = runtime;
    iFrameTimer = frameTimer;
    iGameRoot.Copy(selectedArchive);

    iListBox->SetFocus(EFalse);
    iListBox->MakeVisible(EFalse);

    if (iForeground)
        iFrameTimer->Start();
    DrawNow();
}

void CGameView::ShowLoadErrorL(TInt aError)
{
    TBuf<64> message;
    message.Format(_L("Cannot load game (%d)"), aError);
    CAknErrorNote* note = new (ELeave) CAknErrorNote;
    note->ExecuteLD(message);
}

void CGameView::SetForeground(TBool aForeground)
{
    iForeground = aForeground;

    if (!iRuntime || !iFrameTimer)
    {
        if (aForeground && iListBox)
        {
            SetFocus(ETrue);
            iListBox->SetFocus(ETrue);
            iListBox->DrawNow();
        }
        return;
    }

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
    // In launcher mode the child Avkon list box owns the entire client area.
    if (!iRuntime)
        return;

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
    if (iListBox)
        iListBox->SetRect(Rect());

    // The internal RPG Maker surface intentionally remains 320x240. The E72
    // is a native 320x240 target; other emulator screen sizes are centered.
}

void CGameView::FocusChanged(TDrawNow aDrawNow)
{
    CCoeControl::FocusChanged(aDrawNow);

    if (iRuntime)
    {
        if (!IsFocused())
            iRuntime->ReleaseAllKeys();
    }
    else if (iListBox)
    {
        iListBox->SetFocus(IsFocused());
    }
}

TInt CGameView::CountComponentControls() const
{
    return iListBox != NULL ? 1 : 0;
}

CCoeControl* CGameView::ComponentControl(TInt aIndex) const
{
    return aIndex == 0 ? iListBox : NULL;
}

TKeyResponse CGameView::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    if (!iRuntime)
    {
        if (aType != EEventKey)
            return EKeyWasNotConsumed;
        return iListBox != NULL
            ? iListBox->OfferKeyEventL(aKeyEvent, aType)
            : EKeyWasNotConsumed;
    }

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

void CGameView::HandleListBoxEventL(CEikListBox*, TListBoxEvent aEventType)
{
    if (iRuntime)
        return;

    if (aEventType == EEventEnterKeyPressed ||
        aEventType == EEventItemClicked)
    {
        TRAPD(error, StartSelectedGameL());
        if (error != KErrNone)
        {
            iGameRoot.Zero();
            ShowLoadErrorL(error);
        }
    }
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

    aGc.SetPenColor(TRgb(190, 205, 220));
    if (iRuntime->GameTitle().Length() > 0)
        aGc.DrawText(iRuntime->GameTitle(),
                     TPoint(origin.iX + 8,
                            baseline + font->HeightInPixels() + 2));
    else
        aGc.DrawText(iRuntime->GameRoot(),
                     TPoint(origin.iX + 8,
                            baseline + font->HeightInPixels() + 2));

    aGc.DiscardFont();
}
