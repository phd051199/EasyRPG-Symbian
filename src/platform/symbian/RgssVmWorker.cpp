#include <fbs.h>

#include "Encoding.h"
#include "RgssGraphicsBridge.h"
#include "RgssImageLoader.h"
#include "RgssScriptArchiveBridge.h"
#include "RgssVmBridge.h"
#include "RgssVmWorker.h"
#include "ZipVfs.h"

namespace
{
_LIT(KRgssVmThreadName, "EasyRPG-RGSS-VM");

TInt VmResultToError(TInt aResult)
{
    switch (aResult)
    {
    case ERgssVmOk:
        return KErrNone;
    case ERgssVmErrorInvalidArgument:
        return KErrArgument;
    case ERgssVmErrorUnavailable:
        return KErrNotSupported;
    case ERgssVmErrorNotInitialized:
        return KErrNotReady;
    case ERgssVmErrorFile:
        return KErrNotFound;
    case ERgssVmErrorScript:
        return KErrCorrupt;
    default:
        return KErrGeneral;
    }
}
}

CRgssVmWorker* CRgssVmWorker::NewL(const TDesC& aGamePath, TInt aVersion)
{
    CRgssVmWorker* self = new (ELeave) CRgssVmWorker(aVersion);
    CleanupStack::PushL(self);
    self->ConstructL(aGamePath);
    CleanupStack::Pop(self);
    return self;
}

CRgssVmWorker::CRgssVmWorker(TInt aVersion)
    : iVersion(aVersion),
      iResources(NULL),
      iStartResult(KErrNotReady),
      iExitResult(KRequestPending),
      iFrameRate(0),
      iFrameCount(0),
      iScriptDataOffset(0),
      iScriptFileSize(0),
      iStopRequested(EFalse),
      iStartSignaled(EFalse),
      iVmInitialized(EFalse),
      iThreadCreated(EFalse)
{
    for (TInt index = 0; index < 4; ++index)
        iInputWords[index] = 0;
}

void CRgssVmWorker::ConstructL(const TDesC& aGamePath)
{
    if (aGamePath.Length() <= 0 || aGamePath.Length() > iGamePath.MaxLength())
        User::Leave(KErrArgument);
    if (iVersion < 1 || iVersion > 3)
        User::Leave(KErrArgument);

    iGamePath.Copy(aGamePath);
    User::LeaveIfError(iStarted.CreateLocal(0, EOwnerProcess));
    User::LeaveIfError(iFrameReady.CreateLocal(0, EOwnerProcess));
    User::LeaveIfError(iFrameResume.CreateLocal(0, EOwnerProcess));
}

CRgssVmWorker::~CRgssVmWorker()
{
    Stop();
    iFrameResume.Close();
    iFrameReady.Close();
    iStarted.Close();
}

void CRgssVmWorker::StartL()
{
    if (iThreadCreated)
        User::Leave(KErrAlreadyExists);

    iStartResult = KErrNotReady;
    iExitResult = KRequestPending;
    iFrameRate = 0;
    iFrameCount = 0;
    iScriptDataOffset = 0;
    iScriptFileSize = 0;
    iStopRequested = EFalse;
    iStartSignaled = EFalse;
    iVmInitialized = EFalse;

    User::LeaveIfError(iThread.Create(KRgssVmThreadName,
                                      &CRgssVmWorker::ThreadEntry,
                                      0x18000,
                                      &User::Allocator(),
                                      this,
                                      EOwnerProcess));
    iThreadCreated = ETrue;
    iThread.Resume();
    iStarted.Wait();

    const TInt result = iStartResult;
    if (result != KErrNone)
    {
        Stop();
        User::Leave(result);
    }
}

void CRgssVmWorker::Stop()
{
    if (!iThreadCreated)
        return;

    iStopRequested = ETrue;
    iFrameResume.Signal();

    TRequestStatus threadStatus;
    iThread.Logon(threadStatus);
    if (threadStatus == KRequestPending)
    {
        RTimer timer;
        if (timer.CreateLocal() == KErrNone)
        {
            TRequestStatus timerStatus;
            timer.After(timerStatus, TTimeIntervalMicroSeconds32(1000000));
            User::WaitForRequest(threadStatus, timerStatus);
            if (threadStatus == KRequestPending)
            {
                iThread.Kill(KErrCancel);
                User::WaitForRequest(threadStatus);
            }
            if (timerStatus == KRequestPending)
            {
                timer.Cancel();
                User::WaitForRequest(timerStatus);
            }
            timer.Close();
        }
        else
        {
            iThread.Kill(KErrCancel);
            User::WaitForRequest(threadStatus);
        }
    }

    iThread.Close();
    iThreadCreated = EFalse;
}

TBool CRgssVmWorker::TakeFrame(TInt& aFrameRate, TInt& aFrameCount)
{
    if (!iThreadCreated)
        return EFalse;
    if (iFrameReady.Wait(0) != KErrNone)
        return EFalse;

    aFrameRate = iFrameRate;
    aFrameCount = iFrameCount;
    return ETrue;
}

void CRgssVmWorker::ResumeFrame()
{
    if (iThreadCreated)
        iFrameResume.Signal();
}

void CRgssVmWorker::SetInputWord(TInt aWord, TUint32 aValue)
{
    if (aWord < 0 || aWord >= 4)
        return;
    iInputWords[aWord] = aValue;
}

TBool CRgssVmWorker::Running() const
{
    return iThreadCreated && iThread.ExitType() == EExitPending;
}

TInt CRgssVmWorker::ExitResult() const
{
    return iExitResult;
}

TInt CRgssVmWorker::FrameRate() const
{
    return iFrameRate;
}

TInt CRgssVmWorker::ThreadEntry(TAny* aContext)
{
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (!self)
        return KErrArgument;

    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup)
    {
        self->iExitResult = KErrNoMemory;
        self->SignalStarted(KErrNoMemory);
        return KErrNoMemory;
    }

    const TInt result = self->ThreadMain();
    delete cleanup;
    return result;
}

TInt CRgssVmWorker::ThreadMain()
{
    TRAPD(result, RunVmL());
    iExitResult = result;
    if (!iStartSignaled)
        SignalStarted(result);
    iResources = NULL;
    iFs.Close();
    return result;
}

void CRgssVmWorker::RunVmL()
{
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    User::LeaveIfError(RFbsSession::Connect());
    CleanupStack::PushL(TCleanupItem(&CRgssVmWorker::DisconnectFbs, NULL));

    User::LeaveIfError(iFs.Connect());
    CleanupClosePushL(iFs);
    iResources = CZipVfs::OpenL(iFs, iGamePath);
    CleanupStack::PushL(iResources);

    const char* scriptsPath = "Data/Scripts.rxdata";
    if (iVersion == 2)
        scriptsPath = "Data/Scripts.rvdata";
    else if (iVersion == 3)
        scriptsPath = "Data/Scripts.rvdata2";

    TFileName scriptPath;
    User::LeaveIfError(CopyVmPath(scriptsPath, scriptPath));
    iResources->OpenStoredFileL(scriptPath, iScriptDataOffset, iScriptFileSize);
    EasyRpgRgssScriptArchiveReader archiveReader;
    archiveReader.context = this;
    archiveReader.size = iScriptFileSize;
    archiveReader.read = &CRgssVmWorker::ScriptArchiveRead;
    TInt scriptCount = 0;
    const TInt archiveResult = EasyRpgRgssScriptArchiveValidate(&archiveReader, &scriptCount);
    if (archiveResult != ERgssScriptArchiveOk)
        User::Leave(archiveResult == ERgssScriptArchiveErrorUnsupported
            ? KErrNotSupported : KErrCorrupt);

    EasyRpgRgssGraphicsHost graphicsHost;
    graphicsHost.context = this;
    graphicsHost.frame_sync = &CRgssVmWorker::VmFrameSync;
    graphicsHost.default_frame_rate = iVersion == 1 ? 40 : 60;
    User::LeaveIfError(EasyRpgRgssGraphicsInitialize(&graphicsHost) == ERgssGraphicsOk
        ? KErrNone : KErrGeneral);
    CleanupStack::PushL(TCleanupItem(&CRgssVmWorker::ShutdownGraphics, this));

    EasyRpgRgssVmHost host;
    host.context = this;
    host.file_size = &CRgssVmWorker::VmFileSize;
    host.file_read = &CRgssVmWorker::VmFileRead;
    host.ticks = &CRgssVmWorker::VmTicks;
    host.input = &CRgssVmWorker::VmInput;
    host.bitmap_load = &CRgssVmWorker::VmBitmapLoad;
    host.frame_sync = &CRgssVmWorker::VmFrameSync;
    host.dirty = &CRgssVmWorker::VmDirty;

    const TInt initResult = EasyRpgRgssVmInitialize(iVersion, &host);
    if (initResult != ERgssVmOk)
    {
        const TInt error = VmResultToError(initResult);
        SignalStarted(error);
        User::Leave(error);
    }
    iVmInitialized = ETrue;
    CleanupStack::PushL(TCleanupItem(&CRgssVmWorker::ShutdownVm, this));
    SignalStarted(KErrNone);

    const TInt loadResult = EasyRpgRgssVmLoadScripts(scriptsPath);
    if (loadResult != ERgssVmOk && !iStopRequested)
        User::Leave(VmResultToError(loadResult));

    CleanupStack::PopAndDestroy();
    iVmInitialized = EFalse;
    CleanupStack::PopAndDestroy();
    CleanupStack::PopAndDestroy(iResources);
    iResources = NULL;
    CleanupStack::PopAndDestroy(&iFs);
    CleanupStack::PopAndDestroy();
    CActiveScheduler::Install(NULL);
    CleanupStack::PopAndDestroy(scheduler);
}

void CRgssVmWorker::SignalStarted(TInt aResult)
{
    if (iStartSignaled)
        return;
    iStartResult = aResult;
    iStartSignaled = ETrue;
    iStarted.Signal();
}

void CRgssVmWorker::DisconnectFbs(TAny*)
{
    RFbsSession::Disconnect();
}

void CRgssVmWorker::ShutdownGraphics(TAny*)
{
    EasyRpgRgssGraphicsShutdown();
}

void CRgssVmWorker::ShutdownVm(TAny* aContext)
{
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (!self || !self->iVmInitialized)
        return;
    EasyRpgRgssVmShutdown();
    self->iVmInitialized = EFalse;
}

TInt CRgssVmWorker::CopyVmPath(const char* aSource, TDes& aTarget)
{
    if (!aSource)
        return KErrArgument;

    const TInt length = User::StringLength(reinterpret_cast<const TUint8*>(aSource));
    if (length <= 0 || length > KMaxFileName * 3)
        return KErrArgument;

    TPtrC8 source(reinterpret_cast<const TUint8*>(aSource), length);
    TRAPD(error, TEncoding::Utf8ToUnicodeL(source, aTarget));
    return error;
}

int CRgssVmWorker::VmFileSize(void* aContext, const char* aRelativePath)
{
    if (!aContext)
        return ERgssVmErrorInvalidArgument;
    return static_cast<CRgssVmWorker*>(aContext)->FileSize(aRelativePath);
}

int CRgssVmWorker::VmFileRead(void* aContext,
                              const char* aRelativePath,
                              int aOffset,
                              void* aBuffer,
                              int aBytes)
{
    if (!aContext)
        return ERgssVmErrorInvalidArgument;
    return static_cast<CRgssVmWorker*>(aContext)->ReadFile(aRelativePath,
                                                           aOffset,
                                                           aBuffer,
                                                           aBytes);
}

unsigned int CRgssVmWorker::VmTicks(void*)
{
    return User::NTickCount();
}

unsigned int CRgssVmWorker::VmInput(void* aContext, int aWord)
{
    if (!aContext || aWord < 0 || aWord >= 4)
        return 0;
    return static_cast<CRgssVmWorker*>(aContext)->iInputWords[aWord];
}

int CRgssVmWorker::VmBitmapLoad(void* aContext,
                                const char* aRelativePath,
                                int* aBitmapHandle)
{
    if (!aContext || !aRelativePath || !aBitmapHandle)
        return ERgssVmErrorInvalidArgument;

    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (!self->iResources)
        return ERgssVmErrorNotInitialized;

    TInt handle = 0;
    TRAPD(error, EasyRpgRgssLoadBitmapL(self->iFs,
                                        *self->iResources,
                                        aRelativePath,
                                        handle));
    if (error != KErrNone)
        return error == KErrNoMemory ? ERgssVmErrorRuntime : ERgssVmErrorFile;

    *aBitmapHandle = handle;
    return ERgssVmOk;
}

int CRgssVmWorker::ScriptArchiveRead(void* aContext,
                                     int aOffset,
                                     void* aBuffer,
                                     int aBytes)
{
    if (!aContext || aOffset < 0 || !aBuffer || aBytes < 0)
        return -1;
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (!self->iResources || aOffset > self->iScriptFileSize ||
        aBytes > self->iScriptFileSize - aOffset)
        return -1;
    if (aBytes == 0)
        return 0;

    TPtr8 buffer(static_cast<TUint8*>(aBuffer), 0, aBytes);
    TRAPD(error, self->iResources->ReadStoredFileL(self->iScriptDataOffset,
                                                   self->iScriptFileSize,
                                                   aOffset,
                                                   buffer));
    return error == KErrNone ? buffer.Length() : -1;
}

int CRgssVmWorker::VmFrameSync(void* aContext,
                               int aFrameRate,
                               int aFrameCount)
{
    if (!aContext || aFrameRate <= 0 || aFrameRate > 1000 || aFrameCount < 0)
        return ERgssVmErrorInvalidArgument;

    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (self->iStopRequested)
        return ERgssVmErrorRuntime;

    self->iFrameRate = aFrameRate;
    self->iFrameCount = aFrameCount;
    self->iFrameReady.Signal();
    self->iFrameResume.Wait();

    return self->iStopRequested ? ERgssVmErrorRuntime : ERgssVmOk;
}

void CRgssVmWorker::VmDirty(void*, int, int, int, int)
{
}

TInt CRgssVmWorker::FileSize(const char* aRelativePath) const
{
    if (!iResources)
        return ERgssVmErrorNotInitialized;

    TFileName path;
    if (CopyVmPath(aRelativePath, path) != KErrNone)
        return ERgssVmErrorInvalidArgument;

    TInt dataOffset = 0;
    TInt fileSize = 0;
    TRAPD(error, iResources->OpenStoredFileL(path, dataOffset, fileSize));
    if (error != KErrNone)
        return ERgssVmErrorFile;
    return fileSize;
}

TInt CRgssVmWorker::ReadFile(const char* aRelativePath,
                             TInt aOffset,
                             TAny* aBuffer,
                             TInt aBytes) const
{
    if (!iResources || aOffset < 0 || !aBuffer || aBytes < 0)
        return ERgssVmErrorInvalidArgument;

    TFileName path;
    if (CopyVmPath(aRelativePath, path) != KErrNone)
        return ERgssVmErrorInvalidArgument;

    TInt dataOffset = 0;
    TInt fileSize = 0;
    TRAPD(openError, iResources->OpenStoredFileL(path, dataOffset, fileSize));
    if (openError != KErrNone)
        return ERgssVmErrorFile;
    if (aOffset >= fileSize || aBytes == 0)
        return 0;

    const TInt count = Min(aBytes, fileSize - aOffset);
    TPtr8 buffer(static_cast<TUint8*>(aBuffer), 0, count);
    TRAPD(readError, iResources->ReadStoredFileL(dataOffset,
                                                 fileSize,
                                                 aOffset,
                                                 buffer));
    if (readError != KErrNone)
        return ERgssVmErrorFile;
    return buffer.Length();
}
