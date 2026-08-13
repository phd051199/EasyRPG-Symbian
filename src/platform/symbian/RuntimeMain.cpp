#include <apacmdln.h>
#include <apgcli.h>
#include <e32base.h>
#include <fbs.h>

#include "RuntimeEngine.h"

namespace
{
_LIT(KEasyRpgExecutable, "EasyRPG.exe");

void DisconnectFbs(TAny*)
{
    RFbsSession::Disconnect();
}

void LaunchMainL()
{
    RApaLsSession session;
    User::LeaveIfError(session.Connect());
    CleanupClosePushL(session);
    CApaCommandLine* commandLine = CApaCommandLine::NewLC();
    commandLine->SetExecutableNameL(KEasyRpgExecutable);
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
    runtime->Run();
    CleanupStack::PopAndDestroy(runtime);

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
    TRAPD(error, RunRuntimeL());
    delete cleanup;
    return error;
}
