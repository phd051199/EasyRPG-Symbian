#include <aknappui.h>
#include <aknlists.h>
#include <aknnotewrappers.h>
#include <aknprogressdialog.h>
#include <basched.h>
#include <eikprogi.h>
#include <coemain.h>
#include <e32std.h>
#include <eikclbd.h>
#include <eikenv.h>
#include <eikscrlb.h>
#include <EasyRPG.rsg>

#include "GameLocator.h"
#include "AppConfig.h"
#include "GameLaunchWatcher.h"
#include "LauncherView.h"

namespace
{
_LIT(KDoubleListItemFormat, "\t%S\t%S");
_LIT(KNoGamesFound, "No games found");
_LIT(KEmptyListSubtitle, "");

void CopyGameListText(const TDesC& aSource, TDes& aTarget)
{
    aTarget.Copy(aSource.Left(aTarget.MaxLength()));
    for (TInt index = 0; index < aTarget.Length(); ++index)
    {
        if (aTarget[index] == '\t' || aTarget[index] == '\r' ||
            aTarget[index] == '\n')
            aTarget[index] = ' ';
    }
}

void FormatArchiveSize(TInt aBytes, TDes& aText)
{
    if (aBytes < 0)
        aBytes = 0;
    const TInt64 tenths =
        (static_cast<TInt64>(aBytes) * 10 + 524288) / 1048576;
    aText.Format(_L("%d.%d MB"),
                 static_cast<TInt>(tenths / 10),
                 static_cast<TInt>(tenths % 10));
}

}

CLauncherView* CLauncherView::NewL(const TRect& aRect)
{
    CLauncherView* self = new (ELeave) CLauncherView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop(self);
    return self;
}

CLauncherView::CLauncherView()
    : iListBox(NULL),
      iListItems(NULL),
      iArchivePaths(NULL),
      iLoadingDialog(NULL),
      iLoadingProgress(NULL),
      iLaunchWatcher(NULL),
      iLaunching(EFalse)
{
}

void CLauncherView::ConstructL(const TRect& aRect)
{
    CreateWindowL();
    User::LeaveIfError(iFs.Connect());
    iLaunchWatcher = CGameLaunchWatcher::NewL(*this);

    iListBox = new (ELeave) CAknDoubleStyleListBox;
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

CLauncherView::~CLauncherView()
{
    delete iLaunchWatcher;
    iLaunchWatcher = NULL;
    DismissLoadingDialog();
    delete iListBox;
    iListBox = NULL;
    delete iListItems;
    iListItems = NULL;
    delete iArchivePaths;
    iArchivePaths = NULL;
    iFs.Close();
}

void CLauncherView::BuildGameListL()
{
    iListItems->Reset();
    iArchivePaths->Reset();

    CDesCArrayFlat* displayNames = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(displayNames);
    CDesCArrayFlat* sourcePaths = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(sourcePaths);
    RArray<TInt> archiveSizes;
    CleanupClosePushL(archiveSizes);

    TGameLocator::ListGameArchivesL(iFs,
                                    *displayNames,
                                    *sourcePaths,
                                    archiveSizes);
    for (TInt index = 0; index < displayNames->Count(); ++index)
    {
        TBuf<KMaxFileName> title;
        CopyGameListText((*displayNames)[index], title);
        TBuf<32> subtitle;
        FormatArchiveSize(archiveSizes[index], subtitle);
        TBuf<KMaxFileName + 80> item;
        item.Format(KDoubleListItemFormat, &title, &subtitle);
        iListItems->AppendL(item);
        iArchivePaths->AppendL((*sourcePaths)[index]);
    }

    if (iListItems->Count() == 0)
    {
        TBuf<64> item;
        item.Format(KDoubleListItemFormat, &KNoGamesFound, &KEmptyListSubtitle);
        iListItems->AppendL(item);
    }

    CleanupStack::PopAndDestroy(&archiveSizes);
    CleanupStack::PopAndDestroy(sourcePaths);
    CleanupStack::PopAndDestroy(displayNames);
    iListBox->HandleItemAdditionL();
    if (iArchivePaths->Count() > 0)
        iListBox->SetCurrentItemIndex(0);
}

void CLauncherView::ShowLoadingDialogL()
{
    if (iLoadingDialog)
        return;
    iLoadingDialog = new (ELeave) CAknProgressDialog(
        reinterpret_cast<CEikDialog**>(&iLoadingDialog), ETrue);
    iLoadingDialog->PrepareLC(R_EASYRPG_LOADING_WAIT_NOTE);
    iLoadingProgress = iLoadingDialog->GetProgressInfoL();
    iLoadingProgress->SetFinalValue(100);
    iLoadingDialog->RunLD();
    iLoadingProgress->SetAndDraw(10);
}

void CLauncherView::UpdateLoadingProgress(TInt aValue)
{
    if (!iLoadingProgress)
        return;
    if (aValue < 0)
        aValue = 0;
    else if (aValue > 100)
        aValue = 100;
    iLoadingProgress->SetAndDraw(aValue);
}

void CLauncherView::DismissLoadingDialog()
{
    if (!iLoadingDialog)
        return;
    iLoadingProgress = NULL;
    TRAP_IGNORE(iLoadingDialog->ProcessFinishedL());
    iLoadingDialog = NULL;
}

void CLauncherView::LaunchArchiveL(const TDesC& aArchivePath)
{
    const TFileName currentExecutable = RProcess().FileName();
    const TParsePtrC currentParse(currentExecutable);
    TFileName executablePath(currentParse.DriveAndPath());
    if (executablePath.Length() + EasyRpgApp::KPlayerExecutable().Length() > executablePath.MaxLength())
        User::Leave(KErrTooBig);
    executablePath.Append(EasyRpgApp::KPlayerExecutable);

    if (!iLaunchWatcher)
        User::Leave(KErrNotReady);

    iLaunchWatcher->StartL(executablePath, aArchivePath);
}

void CLauncherView::PlaySelectedGameL()
{
    if (iLaunching || !iListBox || !iArchivePaths)
        return;
    const TInt index = iListBox->CurrentItemIndex();
    if (index < 0 || index >= iArchivePaths->Count())
        return;

    iLaunching = ETrue;
    TRAPD(error,
        ShowLoadingDialogL();
        LaunchArchiveL((*iArchivePaths)[index]);
    );
    if (error != KErrNone)
    {
        iLaunching = EFalse;
        DismissLoadingDialog();
        TBuf<48> message;
        message.Format(_L("Start failed (%d)"), error);
        CAknErrorNote* note = new (ELeave) CAknErrorNote;
        note->ExecuteLD(message);
    }
}

void CLauncherView::GameLaunchCompleted(TInt aError)
{
    iLaunching = EFalse;
    UpdateLoadingProgress(100);
    DismissLoadingDialog();
    if (aError == KLeaveExit)
        aError = KErrNone;
    if (aError != KErrNone)
    {
        const TGameLaunchStage stage = iLaunchWatcher ? iLaunchWatcher->FailureStage() : ELaunchStageNone;
        const TDesC& exitCategory = iLaunchWatcher ? iLaunchWatcher->ExitCategory() : KNullDesC;
        const TInt exitReason = iLaunchWatcher ? iLaunchWatcher->ExitReason() : 0;
        TRAP_IGNORE(
            TBuf<96> message;
            const TDesC& stageName = GameLaunchStageName(stage);
            message.Format(_L("%S failed (%d) [%S %d]"), &stageName, aError, &exitCategory, exitReason);
            CAknErrorNote* note = new (ELeave) CAknErrorNote;
            note->ExecuteLD(message);
        );
        return;
    }

    CAknAppUi* appUi = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi());
    appUi->PrepareToExit();
    if (CActiveScheduler::Current())
        CActiveScheduler::Stop();
}

void CLauncherView::UpdateLayout()
{
    CAknAppUi* appUi = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi());
    SetRect(appUi->ClientRect());
}

void CLauncherView::SizeChanged()
{
    if (iListBox)
        iListBox->SetRect(Rect());
}

TInt CLauncherView::CountComponentControls() const
{
    return iListBox ? 1 : 0;
}

CCoeControl* CLauncherView::ComponentControl(TInt aIndex) const
{
    return aIndex == 0 ? iListBox : NULL;
}

TKeyResponse CLauncherView::HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    return OfferKeyEventL(aKeyEvent, aType);
}

TKeyResponse CLauncherView::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
    if (!iListBox)
        return EKeyWasNotConsumed;
    if (aType == EEventKey &&
        (aKeyEvent.iScanCode == EStdKeyEnter ||
         aKeyEvent.iScanCode == EStdKeyDevice3 ||
         aKeyEvent.iCode == EKeyEnter ||
         aKeyEvent.iCode == EKeyDevice3))
    {
        PlaySelectedGameL();
        return EKeyWasConsumed;
    }
    return iListBox->OfferKeyEventL(aKeyEvent, aType);
}

void CLauncherView::HandleListBoxEventL(CEikListBox*, TListBoxEvent aEventType)
{
    if (aEventType == EEventEnterKeyPressed || aEventType == EEventItemClicked)
        PlaySelectedGameL();
}
