#include <aknappui.h>
#include <aknlists.h>
#include <aknnotewrappers.h>
#include <coemain.h>
#include <e32keys.h>
#include <eikbtgpc.h>
#include <eikclbd.h>
#include <eikenv.h>
#include <eikscrlb.h>
#include <gdi.h>
#include <w32std.h>

#include "easyrpgsymbian/DisplaySurface.h"
#include "easyrpgsymbian/GameLocator.h"
#include "easyrpgsymbian/GameView.h"
#include "easyrpgsymbian/PlayerRuntime.h"
#include "easyrpgsymbian/SymbianAppUi.h"
#include "easyrpgsymbian/SymbianLogBridge.h"
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
      iDisplaySurface(NULL),
      iResources(NULL),
      iRuntime(NULL),
      iFrameTimer(NULL),
      iStartGameCallback(NULL),
      iForeground(ETrue)
{
}

void CGameView::ConstructL(const TRect& aRect)
{
    CreateWindowL();
    User::LeaveIfError(iFs.Connect());
    iFrameTimer = CFrameTimer::NewL(*this);
    iStartGameCallback = new (ELeave) CAsyncCallBack(
        TCallBack(&CGameView::StartGameCallback, this),
        CActive::EPriorityHigh);

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
    if (iStartGameCallback)
        iStartGameCallback->Cancel();
    delete iStartGameCallback;
    iStartGameCallback = NULL;

    if (iFrameTimer)
        iFrameTimer->Stop();

    delete iFrameTimer;
    iFrameTimer = NULL;

    delete iRuntime;
    iRuntime = NULL;

    delete iResources;
    iResources = NULL;

    delete iDisplaySurface;
    iDisplaySurface = NULL;

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

    CSymbianDisplaySurface* displaySurface =
        CSymbianDisplaySurface::NewL(TSize(KGameWidth, KGameHeight));
    CleanupStack::PushL(displaySurface);

    CZipVfs* resources = CZipVfs::OpenL(iFs,
                                        selectedArchive,
                                        KTargetArchivePassword);
    CleanupStack::PushL(resources);

    CPlayerRuntime* runtime = CPlayerRuntime::NewL(resources,
                                                   selectedArchive,
                                                   *displaySurface);
    CleanupStack::PushL(runtime);
    EasyRpgSymbianLogDebug("game runtime ready");

    if (iListBox->ScrollBarFrame())
    {
        iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
            CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOff);
    }

    iListBox->SetFocus(EFalse);
    iListBox->MakeVisible(EFalse);
    SetFocus(ETrue);

    CEasyRpgAppUi* appUi = static_cast<CEasyRpgAppUi*>(CEikonEnv::Static()->EikAppUi());
    appUi->EnterGameFullscreen();
    const TSize screenSize = CCoeEnv::Static()->ScreenDevice()->SizeInPixels();
    SetRect(TRect(TPoint(0, 0), screenSize));

    CleanupStack::Pop(runtime);
    CleanupStack::Pop(resources);
    CleanupStack::Pop(displaySurface);

    iDisplaySurface = displaySurface;
    iResources = resources;
    iRuntime = runtime;
    iGameRoot.Copy(selectedArchive);
    DrawNow();

    if (iForeground) {
        iFrameTimer->Start();
        EasyRpgSymbianLogDebug("frame timer started");
    }
    EasyRpgSymbianLogDebug("game start returned");
}

void CGameView::StopGame()
{
    if (iFrameTimer)
        iFrameTimer->Stop();

    delete iRuntime;
    iRuntime = NULL;
    delete iResources;
    iResources = NULL;
    delete iDisplaySurface;
    iDisplaySurface = NULL;
    iGameRoot.Zero();

    CEasyRpgAppUi* appUi = static_cast<CEasyRpgAppUi*>(CEikonEnv::Static()->EikAppUi());
    appUi->ExitGameFullscreen();
    SetRect(appUi->ClientRect());

    if (iListBox)
    {
        if (iListBox->ScrollBarFrame())
        {
            TRAP_IGNORE(iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
                CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto));
        }
        iListBox->SetRect(Rect());
        iListBox->MakeVisible(ETrue);
        iListBox->SetFocus(ETrue);
        SetFocus(ETrue);
        iListBox->DrawNow();
    }
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
    EasyRpgSymbianLogDebug(aForeground ? "foreground on" : "foreground off");
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
        iRuntime->Suspend();
        iFrameTimer->Stop();
    }
    else
    {
        iRuntime->Resume();
        iFrameTimer->Start();
        DrawNow();
    }
}

void CGameView::UpdateLayout()
{
    CAknAppUi* appUi = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi());
    if (!iRuntime)
    {
        SetRect(appUi->ClientRect());
        return;
    }

    const TSize screenSize = CCoeEnv::Static()->ScreenDevice()->SizeInPixels();
    SetRect(TRect(TPoint(0, 0), screenSize));
}

void CGameView::Draw(const TRect&) const
{
    if (!iRuntime || !iDisplaySurface)
        return;

    CWindowGc& gc = SystemGc();
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.SetBrushColor(TRgb(0, 0, 0));
    gc.DrawRect(Rect());

    if (!iDisplaySurface->HasFrame())
        return;

    const TSize bitmapSize = iDisplaySurface->Size();
    const TSize viewSize = Rect().Size();
    TPoint destination(0, 0);
    if (viewSize.iWidth > bitmapSize.iWidth)
        destination.iX = (viewSize.iWidth - bitmapSize.iWidth) / 2;
    if (viewSize.iHeight > bitmapSize.iHeight)
        destination.iY = (viewSize.iHeight - bitmapSize.iHeight) / 2;
    gc.BitBlt(destination, &iDisplaySurface->Bitmap());
}

void CGameView::SizeChanged()
{
    if (iListBox && !iRuntime)
        iListBox->SetRect(Rect());
}

void CGameView::HandleResourceChange(TInt aType)
{
    CCoeControl::HandleResourceChange(aType);
    if (aType == KEikDynamicLayoutVariantSwitch)
    {
        UpdateLayout();
        if (iRuntime)
            DrawNow();
    }
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
    return iListBox != NULL && !iRuntime ? 1 : 0;
}

CCoeControl* CGameView::ComponentControl(TInt aIndex) const
{
    return aIndex == 0 && !iRuntime ? iListBox : NULL;
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
    if (iRuntime || !iStartGameCallback)
        return;

    if ((aEventType == EEventEnterKeyPressed ||
         aEventType == EEventItemClicked) &&
        !iStartGameCallback->IsActive())
    {
        EasyRpgSymbianLogDebug("game start deferred");
        iStartGameCallback->CallBack();
    }
}

TInt CGameView::StartGameCallback(TAny* aPtr)
{
    CGameView* self = static_cast<CGameView*>(aPtr);
    if (!self || self->iRuntime)
        return KErrNone;

    EasyRpgSymbianLogDebug("game start callback");
    TRAPD(error, self->StartSelectedGameL());
    if (error != KErrNone)
    {
        self->iGameRoot.Zero();
        TRAP_IGNORE(self->ShowLoadErrorL(error));
    }
    EasyRpgSymbianLogDebug("game start callback returned");
    return KErrNone;
}

void CGameView::FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame)
{
    if (!iForeground || !iRuntime)
        return;

    iRuntime->TickL(aLogicFrame, aRenderFrame);
    if (aRenderFrame && iDisplaySurface && iDisplaySurface->HasFrame())
        DrawDeferred();
    if (!iRuntime->IsRunning())
    {
        StopGame();
        return;
    }
}

void CGameView::FrameError(TInt aError)
{
    StopGame();
    TRAP_IGNORE(ShowLoadErrorL(aError));
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

