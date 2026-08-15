#include "ScriptRuntime.h"
#include "ScriptVmBridge.h"
#include "ZipVfs.h"

namespace
{
const TInt KMaxScriptArchiveBytes = 16 * 1024 * 1024;
_LIT(KXpScripts, "Data/Scripts.rxdata");
_LIT(KVxScripts, "Data/Scripts.rvdata");
_LIT(KVxaScripts, "Data/Scripts.rvdata2");
}

CScriptRuntime* CScriptRuntime::NewL(RFs& aFs, const TDesC& aArchivePath)
{
    CScriptRuntime* self = new (ELeave) CScriptRuntime(aFs);
    CleanupStack::PushL(self);
    self->ConstructL(aArchivePath);
    CleanupStack::Pop(self);
    return self;
}

CScriptRuntime::CScriptRuntime(RFs& aFs)
    : iFs(aFs),
      iResources(NULL),
      iGeneration(0),
      iVmCreated(EFalse)
{
}

CScriptRuntime::~CScriptRuntime()
{
    if (iVmCreated)
    {
        EasyRpgScriptVmDestroy();
        iVmCreated = EFalse;
    }
    delete iResources;
    iResources = NULL;
}

void CScriptRuntime::ConstructL(const TDesC& aArchivePath)
{
    iResources = CZipVfs::OpenL(iFs, aArchivePath);
    SelectGenerationL();

    const TDesC* scriptPath = &KXpScripts;
    if (iGeneration == 2)
        scriptPath = &KVxScripts;
    else if (iGeneration == 3)
        scriptPath = &KVxaScripts;

    HBufC8* archive = iResources->ReadFileLC(*scriptPath, KMaxScriptArchiveBytes);
    char error[512];
    error[0] = 0;
    const TInt result = EasyRpgScriptVmCreate(
        archive->Ptr(), archive->Length(), iGeneration, error, sizeof(error));
    CleanupStack::PopAndDestroy(archive);
    if (result != EScriptVmOk)
    {
        CopyVmError(error);
        User::Leave(result);
    }
    iVmCreated = ETrue;
}

void CScriptRuntime::SelectGenerationL()
{
    const TBool xp = iResources->Exists(KXpScripts);
    const TBool vx = iResources->Exists(KVxScripts);
    const TBool vxa = iResources->Exists(KVxaScripts);
    const TInt count = static_cast<TInt>(xp) + static_cast<TInt>(vx) +
                       static_cast<TInt>(vxa);
    if (count != 1)
        User::Leave(count == 0 ? KErrNotSupported : KErrCorrupt);
    iGeneration = xp ? 1 : (vx ? 2 : 3);
}

void CScriptRuntime::ExecuteL()
{
    if (!iVmCreated)
        User::Leave(KErrNotReady);
    char error[512];
    error[0] = 0;
    const TInt result = EasyRpgScriptVmExecute(error, sizeof(error));
    if (result != EScriptVmOk)
    {
        CopyVmError(error);
        User::Leave(result);
    }
}

void CScriptRuntime::CopyVmError(const char* aError)
{
    iLastError.Zero();
    if (!aError)
        return;
    TInt length = 0;
    while (aError[length] && length < iLastError.MaxLength())
        ++length;
    iLastError.Copy(TPtrC8(
        reinterpret_cast<const TUint8*>(aError), length));
}

TInt CScriptRuntime::Generation() const
{
    return iGeneration;
}

const TDesC8& CScriptRuntime::LastError() const
{
    return iLastError;
}
