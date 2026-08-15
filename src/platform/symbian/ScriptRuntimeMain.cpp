#include <apacmdln.h>
#include <apgcli.h>
#include <e32base.h>
#include <f32file.h>
#include <fbs.h>

#include "RgssEngine.h"

namespace
{
const TInt KGamePathParameterSlot = 1;
_LIT(KEasyRpgExecutable, "EasyRPG.exe");
_LIT(KLogDirectory, "C:\\Data\\EasyRPG\\");
_LIT(KLogPath, "C:\\Data\\EasyRPG\\script-runtime.log");
TBool gRendezvousSignaled = EFalse;

void SignalRendezvous(TInt aResult)
{
    if (!gRendezvousSignaled)
    {
        RProcess::Rendezvous(aResult);
        gRendezvousSignaled = ETrue;
    }
}

void DisconnectFbs(TAny*)
{
    RFbsSession::Disconnect();
}

void WriteLog(RFs& aFs, const TDesC8& aMessage)
{
    TRAP_IGNORE(
        aFs.MkDirAll(KLogDirectory);
        RFile file;
        TInt error = file.Open(aFs, KLogPath, EFileWrite | EFileShareAny);
        if (error == KErrNotFound || error == KErrPathNotFound)
            error = file.Replace(aFs, KLogPath, EFileWrite | EFileShareAny);
        if (error == KErrNone)
        {
            TInt position = 0;
            file.Seek(ESeekEnd, position);
            file.Write(aMessage);
            file.Write(_L8("\r\n"));
            file.Close();
        }
    );
}

void LaunchMainL()
{
    RApaLsSession session;
    User::LeaveIfError(session.Connect());
    CleanupClosePushL(session);
    const TParsePtrC currentParse(RProcess().FileName());
    TFileName launcherPath(currentParse.DriveAndPath());
    launcherPath.Append(KEasyRpgExecutable);

    CApaCommandLine* commandLine = CApaCommandLine::NewLC();
    commandLine->SetExecutableNameL(launcherPath);
    commandLine->SetCommandL(EApaCommandRun);
    User::LeaveIfError(session.StartApp(*commandLine));
    CleanupStack::PopAndDestroy(commandLine);
    CleanupStack::PopAndDestroy(&session);
}

void RunScriptRuntimeL()
{
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    User::LeaveIfError(RFbsSession::Connect());
    CleanupStack::PushL(TCleanupItem(DisconnectFbs, NULL));

    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);

    TFileName archivePath;
    User::LeaveIfError(User::GetDesParameter(KGamePathParameterSlot, archivePath));

    CRgssEngine* engine = CRgssEngine::NewL(fs, archivePath);
    CleanupStack::PushL(engine);
    TBuf8<80> ready;
    ready.Format(_L8("RGSS host ready; generation %d"), engine->Generation());
    WriteLog(fs, ready);
    SignalRendezvous(KErrNone);

    TRAPD(executeError, engine->RunL());
    if (executeError != KErrNone)
    {
        if (engine->LastError().Length() > 0)
            WriteLog(fs, engine->LastError());
        User::Leave(executeError);
    }

    CleanupStack::PopAndDestroy(engine);
    CleanupStack::PopAndDestroy(&fs);
    CleanupStack::PopAndDestroy();
    CActiveScheduler::Install(NULL);
    CleanupStack::PopAndDestroy(scheduler);
    TRAP_IGNORE(LaunchMainL());
}
}

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup)
        return KErrNoMemory;
    TRAPD(error, RunScriptRuntimeL());
    if (error != KErrNone)
    {
        const TBool launcherAlreadyReleased = gRendezvousSignaled;
        SignalRendezvous(error);
        if (launcherAlreadyReleased)
            TRAP_IGNORE(LaunchMainL());
    }
    delete cleanup;
    return error;
}
