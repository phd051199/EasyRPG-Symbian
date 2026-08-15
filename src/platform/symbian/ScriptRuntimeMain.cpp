#include <apacmdln.h>
#include <apgcli.h>
#include <e32base.h>
#include <f32file.h>

#include "ScriptRuntime.h"

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

void WriteLog(RFs& aFs, const TDesC8& aMessage)
{
    TRAP_IGNORE(
        aFs.MkDirAll(KLogDirectory);
        RFile file;
        if (file.Replace(aFs, KLogPath, EFileWrite | EFileShareAny) == KErrNone)
        {
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
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);

    TFileName archivePath;
    User::LeaveIfError(User::GetDesParameter(
        KGamePathParameterSlot, archivePath));

    CScriptRuntime* runtime = CScriptRuntime::NewL(fs, archivePath);
    CleanupStack::PushL(runtime);
    TBuf8<64> ready;
    ready.Format(_L8("Ruby 3 ready; script generation %d"), runtime->Generation());
    WriteLog(fs, ready);
    SignalRendezvous(KErrNone);

    TRAPD(executeError, runtime->ExecuteL());
    if (executeError != KErrNone)
    {
        if (runtime->LastError().Length() > 0)
            WriteLog(fs, runtime->LastError());
        User::Leave(executeError);
    }

    CleanupStack::PopAndDestroy(runtime);
    CleanupStack::PopAndDestroy(&fs);
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
