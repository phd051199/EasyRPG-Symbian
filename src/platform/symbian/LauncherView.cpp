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
#include "GameMetadata.h"
#include "LauncherView.h"

namespace
{
_LIT(KDoubleListItemFormat, "\t%S\t%S");
_LIT(KNoGamesFound, "No games found");
_LIT(KEmptyListSubtitle, "");

void CopyListText(const TDesC& aSource, TDes& aTarget)
{
    aTarget.Copy(aSource.Left(aTarget.MaxLength()));
    for (TInt index = 0; index < aTarget.Length(); ++index)
    {
        if (aTarget[index] == '\t' || aTarget[index] == '\r' ||
            aTarget[index] == '\n')
            aTarget[index] = ' ';
    }
}

}

class CGameLaunchWatcher : public CActive
{
public:
    static CGameLaunchWatcher* NewL(CLauncherView& aOwner)
    {
        CGameLaunchWatcher* self = new (ELeave) CGameLaunchWatcher(aOwner);
        CActiveScheduler::Add(self);
        return self;
    }

    ~CGameLaunchWatcher()
    {
        Cancel();
        if (iCreated)
            iProcess.Close();
    }

    void StartL(const TDesC& aExecutablePath, const TDesC& aArchivePath)
    {
        if (IsActive() || iState != EIdle || iCreated)
            User::Leave(KErrInUse);
        if (aExecutablePath.Length() > iExecutablePath.MaxLength() ||
            aArchivePath.Length() > iArchivePath.MaxLength())
            User::Leave(KErrTooBig);

        iExecutablePath.Copy(aExecutablePath);
        iArchivePath.Copy(aArchivePath);
        iState = EDeferredStart;
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status, KErrNone);
    }

private:
    enum TState
    {
        EIdle,
        EDeferredStart,
        EWaitingForRuntime
    };

    CGameLaunchWatcher(CLauncherView& aOwner)
        : CActive(EPriorityIdle),
          iOwner(aOwner),
          iState(EIdle),
          iCreated(EFalse)
    {
    }

    void StartProcessL()
    {
        const TInt KGamePathParameterSlot = 1;
        iOwner.UpdateLoadingProgress(35);
        User::LeaveIfError(iProcess.Create(iExecutablePath, KNullDesC));
        iCreated = ETrue;

        const TInt parameterError = iProcess.SetParameter(KGamePathParameterSlot, iArchivePath);
        if (parameterError != KErrNone)
        {
            iProcess.Kill(parameterError);
            iProcess.Close();
            iCreated = EFalse;
            User::Leave(parameterError);
        }

        iProcess.Rendezvous(iStatus);
        if (iStatus != KRequestPending)
        {
            TInt error = iStatus.Int();
            if (error == KErrNone)
                error = KErrGeneral;
            iProcess.Kill(error);
            iProcess.Close();
            iCreated = EFalse;
            User::Leave(error);
        }

        iState = EWaitingForRuntime;
        iOwner.UpdateLoadingProgress(70);
        SetActive();
        iProcess.Resume();
    }

    void RunL()
    {
        if (iState == EDeferredStart)
        {
            iState = EIdle;
            TRAPD(error, StartProcessL());
            if (error != KErrNone)
                iOwner.GameLaunchCompleted(error);
            return;
        }

        const TInt error = iStatus.Int();
        if (iCreated)
            iProcess.Close();
        iCreated = EFalse;
        iState = EIdle;
        iOwner.GameLaunchCompleted(error);
    }

    TInt RunError(TInt aError)
    {
        if (iCreated)
        {
            iProcess.Kill(aError);
            iProcess.Close();
            iCreated = EFalse;
        }
        iState = EIdle;
        iOwner.GameLaunchCompleted(aError);
        return KErrNone;
    }

    void DoCancel()
    {
        if (iState == EWaitingForRuntime && iCreated)
        {
            iProcess.Kill(KErrCancel);
            User::WaitForRequest(iStatus);
            iProcess.Close();
            iCreated = EFalse;
        }
        iState = EIdle;
    }

private:
    CLauncherView& iOwner;
    RProcess iProcess;
    TFileName iExecutablePath;
    TFileName iArchivePath;
    TState iState;
    TBool iCreated;
};

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
    iEngineVersions.Close();
    iFs.Close();
}

void CLauncherView::BuildGameListL()
{
    iListItems->Reset();
    iArchivePaths->Reset();
    iEngineVersions.Reset();

    CDesCArrayFlat* displayNames = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(displayNames);
    TGameLocator::ListGameArchivesL(iFs, *displayNames, *iArchivePaths);

    for (TInt index = 0; index < displayNames->Count(); ++index)
    {
        TBuf<KMaxFileName> title;
        CopyListText((*displayNames)[index], title);

        TInt version = EGameEngineUnknown;
        TRAP_IGNORE(ReadGameMetadataL(iFs,
                                      (*iArchivePaths)[index],
                                      title,
                                      version));
        User::LeaveIfError(iEngineVersions.Append(version));

        TBuf<64> subtitle;
        FormatGameSubtitle(iFs, (*iArchivePaths)[index], version, subtitle);

        TBuf<KMaxFileName + 80> item;
        item.Format(KDoubleListItemFormat, &title, &subtitle);
        iListItems->AppendL(item);
    }

    if (iListItems->Count() == 0)
    {
        TBuf<64> item;
        item.Format(KDoubleListItemFormat, &KNoGamesFound, &KEmptyListSubtitle);
        iListItems->AppendL(item);
    }

    CleanupStack::PopAndDestroy(displayNames);
    iListBox->HandleItemAdditionL();
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

void CLauncherView::LaunchArchiveL(const TDesC& aArchivePath, TInt aVersion)
{
    _LIT(KEasyRpgRuntime, "EasyRPGGame.exe");
    _LIT(KEasyRpgScriptRuntime, "EasyRPGScript.exe");
    const TDesC* executable = NULL;
    if (aVersion == EGameEngineRpg2000 ||
        aVersion == EGameEngineRpg2003)
        executable = &KEasyRpgRuntime;
    else if (aVersion == EGameEngineRpgXp ||
             aVersion == EGameEngineRpgVx ||
             aVersion == EGameEngineRpgVxAce)
        executable = &KEasyRpgScriptRuntime;
    else
        User::Leave(KErrNotSupported);

    const TFileName currentExecutable = RProcess().FileName();
    const TParsePtrC currentParse(currentExecutable);
    TFileName executablePath(currentParse.DriveAndPath());
    executablePath.Append(*executable);

    if (!iLaunchWatcher)
        User::Leave(KErrNotReady);
    iLaunchWatcher->StartL(executablePath, aArchivePath);
}

void CLauncherView::PlaySelectedGameL()
{
    if (iLaunching || !iListBox || !iArchivePaths)
        return;
    const TInt index = iListBox->CurrentItemIndex();
    if (index < 0 || index >= iArchivePaths->Count() ||
        index >= iEngineVersions.Count())
        return;
    iLaunching = ETrue;
    TRAPD(error,
        ShowLoadingDialogL();
        LaunchArchiveL((*iArchivePaths)[index], iEngineVersions[index]);
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
        TRAP_IGNORE(
            TBuf<48> message;
            message.Format(_L("Start failed (%d)"), aError);
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
