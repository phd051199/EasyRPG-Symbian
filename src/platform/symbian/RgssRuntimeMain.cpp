#include <apacmdln.h>
#include <apgcli.h>
#include <e32base.h>
#include <f32file.h>
#include <fbs.h>

#include "RgssEngine.h"

namespace
{
_LIT(KEasyRpgExecutable, "EasyRPG.exe");
TBool gRendezvousSignaled = EFalse;

void SignalRendezvous(TInt aResult)
{
    if (gRendezvousSignaled)
        return;
    RProcess::Rendezvous(aResult);
    gRendezvousSignaled = ETrue;
}

void DisconnectFbs(TAny*)
{
    RFbsSession::Disconnect();
}

void LaunchMainL()
{
    RApaLsSession session;
    User::LeaveIfError(session.Connect());
    CleanupClosePushL(session);
    const TFileName currentExecutable = RProcess().FileName();
    const TParsePtrC currentParse(currentExecutable);
    TFileName launcherPath(currentParse.DriveAndPath());
    launcherPath.Append(KEasyRpgExecutable);

    CApaCommandLine* commandLine = CApaCommandLine::NewLC();
    commandLine->SetExecutableNameL(launcherPath);
    commandLine->SetCommandL(EApaCommandRun);
    User::LeaveIfError(session.StartApp(*commandLine));
    CleanupStack::PopAndDestroy(commandLine);
    CleanupStack::PopAndDestroy(&session);
}

void RunRgssL()
{
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    User::LeaveIfError(RFbsSession::Connect());
    CleanupStack::PushL(TCleanupItem(DisconnectFbs, NULL));

    CRgssEngine* engine = CRgssEngine::NewL();
    CleanupStack::PushL(engine);
    engine->StartL();
    SignalRendezvous(KErrNone);
    engine->RunL();
    CleanupStack::PopAndDestroy(engine);

    CleanupStack::PopAndDestroy();
    TRAP_IGNORE(LaunchMainL());
    CActiveScheduler::Install(NULL);
    CleanupStack::PopAndDestroy(scheduler);
}
}

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup)
        return KErrNoMemory;
    TRAPD(error, RunRgssL());
    if (error != KErrNone)
        SignalRendezvous(error);
    delete cleanup;
    return error;
}
