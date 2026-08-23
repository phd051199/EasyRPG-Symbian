#include <apacmdln.h>
#include <apgcli.h>
#include <e32base.h>
#include <f32file.h>
#include <fbs.h>

#include "Encoding.h"
#include "RuntimeEngine.h"
#include "AppConfig.h"

namespace
{
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
    if (launcherPath.Length() + EasyRpgApp::KLauncherExecutable().Length() > launcherPath.MaxLength())
        User::Leave(KErrTooBig);
    launcherPath.Append(EasyRpgApp::KLauncherExecutable);

    CApaCommandLine* commandLine = CApaCommandLine::NewLC();
    commandLine->SetExecutableNameL(launcherPath);
    commandLine->SetCommandL(EApaCommandRun);
    User::LeaveIfError(session.StartApp(*commandLine));
    CleanupStack::PopAndDestroy(commandLine);
    CleanupStack::PopAndDestroy(&session);
}

void RunRuntimeL()
{
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    User::LeaveIfError(RFbsSession::Connect());
    CleanupStack::PushL(TCleanupItem(DisconnectFbs, NULL));

    CRuntimeEngine* runtime = CRuntimeEngine::NewL();
    CleanupStack::PushL(runtime);
    RProcess::Rendezvous(KErrNone);
    runtime->Run();
    const TBool returnToLauncher = runtime->ReturnToLauncher();
    CleanupStack::PopAndDestroy(runtime);

    CleanupStack::PopAndDestroy();
    if (returnToLauncher)
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
    TRAPD(error, RunRuntimeL());
    TEncoding::Shutdown();
    if (error != KErrNone)
        TRAP_IGNORE(LaunchMainL());
    delete cleanup;
    return error;
}
