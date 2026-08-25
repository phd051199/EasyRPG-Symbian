#include "GameLaunchWatcher.h"
#include "LauncherView.h"

namespace
{
_LIT(KStageLaunch, "launch");
_LIT(KStageCreate, "create");
_LIT(KStageRendezvous, "rendezvous");
_LIT(KStageRuntime, "runtime");
}

const TDesC& GameLaunchStageName(TGameLaunchStage aStage)
{
    switch (aStage)
    {
    case ELaunchStageCreate: return KStageCreate;
    case ELaunchStageRendezvous: return KStageRendezvous;
    case ELaunchStageRuntimeStartup: return KStageRuntime;
    default: return KStageLaunch;
    }
}

CGameLaunchWatcher* CGameLaunchWatcher::NewL(CLauncherView& aOwner)
{
    CGameLaunchWatcher* self = new (ELeave) CGameLaunchWatcher(aOwner);
    CActiveScheduler::Add(self);
    return self;
}

CGameLaunchWatcher::CGameLaunchWatcher(CLauncherView& aOwner)
    : CActive(EPriorityIdle),
      iOwner(aOwner),
      iState(EIdle),
      iCreated(EFalse),
      iResumed(EFalse),
      iFailureStage(ELaunchStageNone),
      iExitReason(0)
{
}

CGameLaunchWatcher::~CGameLaunchWatcher()
{
    Cancel();
    if (iCreated)
        iProcess.Close();
}

void CGameLaunchWatcher::StartL(const TDesC& aExecutablePath,
                                const TDesC& aArchivePath)
{
    if (IsActive() || iState != EIdle || iCreated)
        User::Leave(KErrInUse);
    if (aExecutablePath.Length() > iExecutablePath.MaxLength() ||
        aArchivePath.Length() > iArchivePath.MaxLength())
        User::Leave(KErrTooBig);

    iExecutablePath.Copy(aExecutablePath);
    iArchivePath.Copy(aArchivePath);
    iFailureStage = ELaunchStageNone;
    iExitCategory.Zero();
    iExitReason = 0;
    iState = EDeferredStart;
    iStatus = KRequestPending;
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
}

TGameLaunchStage CGameLaunchWatcher::FailureStage() const
{
    return iFailureStage;
}

const TDesC& CGameLaunchWatcher::ExitCategory() const
{
    return iExitCategory;
}

TInt CGameLaunchWatcher::ExitReason() const
{
    return iExitReason;
}

void CGameLaunchWatcher::StartProcessL()
{
    iOwner.UpdateLoadingProgress(35);
    iFailureStage = ELaunchStageCreate;
    User::LeaveIfError(iProcess.Create(iExecutablePath, iArchivePath));
    iCreated = ETrue;
    iResumed = EFalse;

    iFailureStage = ELaunchStageRendezvous;
    iProcess.Rendezvous(iStatus);
    if (iStatus != KRequestPending)
    {
        TInt error = iStatus.Int();
        if (error == KErrNone)
            error = KErrGeneral;
        iProcess.Kill(error);
        iProcess.Close();
        iCreated = EFalse;
        iResumed = EFalse;
        User::Leave(error);
    }

    iState = EWaitingForRuntime;
    iFailureStage = ELaunchStageRuntimeStartup;
    iOwner.UpdateLoadingProgress(70);
    SetActive();
    iProcess.Resume();
    iResumed = ETrue;
}

void CGameLaunchWatcher::RunL()
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
    {
        iExitCategory.Copy(iProcess.ExitCategory());
        iExitReason = iProcess.ExitReason();
        iProcess.Close();
    }
    iCreated = EFalse;
    iResumed = EFalse;
    iState = EIdle;
    iOwner.GameLaunchCompleted(error);
}

TInt CGameLaunchWatcher::RunError(TInt aError)
{
    if (iCreated)
    {
        if (iResumed)
        {
            iProcess.RendezvousCancel(iStatus);
            User::WaitForRequest(iStatus);
        }
        else
        {
            iProcess.Kill(aError);
        }
        iProcess.Close();
        iCreated = EFalse;
        iResumed = EFalse;
    }
    iState = EIdle;
    iOwner.GameLaunchCompleted(aError);
    return KErrNone;
}

void CGameLaunchWatcher::DoCancel()
{
    if (iCreated)
    {
        if (iResumed)
        {
            iProcess.RendezvousCancel(iStatus);
            User::WaitForRequest(iStatus);
        }
        else
        {
            iProcess.Kill(KErrCancel);
        }
        iProcess.Close();
        iCreated = EFalse;
        iResumed = EFalse;
    }
    iState = EIdle;
}
