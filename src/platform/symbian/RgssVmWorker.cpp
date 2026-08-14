#include <e32hal.h>
#include <fbs.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifdef EASYRPG_RUBY300
#include <pthread.h>
#endif

#include "Encoding.h"
#include "RgssGlyphCache.h"
#include "RgssGraphicsBridge.h"
#include "RgssImageLoader.h"
#include "RgssScriptArchiveBridge.h"
#include "RgssVmBridge.h"
#include "RgssVmWorker.h"
#include "ZipVfs.h"

namespace
{
_LIT(KRgssVmThreadName, "EasyRPG-RGSS-VM");
const char* const KAudioExtensions[] = { ".ogg", ".mp3", ".wav", ".mid", ".midi", ".wma" };
const TInt KAudioExtensionCount = sizeof(KAudioExtensions) / sizeof(KAudioExtensions[0]);
_LIT(KSaveBasePath, "C:\\Data\\EasyRPG\\Saves\\");

TUint32 HashText(const TDesC& aText, TUint32 aSeed)
{
    TUint32 hash = aSeed;
    for (TInt index = 0; index < aText.Length(); ++index)
    {
        const TUint16 value = aText[index];
        hash ^= static_cast<TUint8>(value & 0xffu);
        hash *= 16777619u;
        hash ^= static_cast<TUint8>((value >> 8) & 0xffu);
        hash *= 16777619u;
    }
    return hash;
}

void PrepareSaveDirectoryL(RFs& aFs, const TDesC& aGamePath)
{
    const TUint32 gameHash = HashText(aGamePath, 2166136261u);
    TBuf<16> gameDirectory;
    gameDirectory.Format(_L("%08x\\"), gameHash);

    TFileName saveRoot;
    saveRoot.Copy(KSaveBasePath);
    saveRoot.Append(gameDirectory);
    const TInt directoryError = aFs.MkDirAll(saveRoot);
    if (directoryError != KErrNone && directoryError != KErrAlreadyExists)
        User::Leave(directoryError);

    TBuf8<KMaxFileName + 1> path;
    for (TInt index = 0; index < saveRoot.Length(); ++index)
    {
        const TUint16 value = saveRoot[index];
        if (value > 0x7fu)
            User::Leave(KErrNotSupported);
        path.Append(static_cast<TUint8>(value == '\\' ? '/' : value));
    }
    path.Append(0);
    if (chdir(reinterpret_cast<const char*>(path.Ptr())) != 0)
        User::Leave(KErrPathNotFound);
}

void* ScriptAllocate(void*, unsigned int aBytes)
{
    if (aBytes > 0x7fffffffu)
        return NULL;
    return User::Alloc(static_cast<TInt>(aBytes));
}

void ScriptRelease(void*, void* aMemory)
{
    User::Free(aMemory);
}

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
      iGlyphCache(NULL),
      iStartResult(KErrNotReady),
      iExitResult(KRequestPending),
      iFrameRate(0),
      iFrameCount(0),
      iScriptDataOffset(0),
      iScriptFileSize(0),
      iScriptEntries(NULL),
      iScriptCount(0),
      iAudioCommandCount(0),
      iAudioCommandRead(0),
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
    iScriptEntries = NULL;
    iScriptCount = 0;
    iAudioCommandCount = 0;
    iAudioCommandRead = 0;
    iStopRequested = EFalse;
    iStartSignaled = EFalse;
    iVmInitialized = EFalse;

#ifdef EASYRPG_RUBY300
    const TInt stackSize = 0x40000;
    const TInt heapMax = 0x02800000;
#else
    const TInt stackSize = 0x18000;
    const TInt heapMax = 0x01800000;
#endif
    User::LeaveIfError(iThread.Create(KRgssVmThreadName,
                                      &CRgssVmWorker::ThreadEntry,
                                      stackSize,
                                      0x00080000,
                                      heapMax,
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

TBool CRgssVmWorker::TakeAudioCommand(TRgssAudioCommand& aCommand)
{
    if (iAudioCommandRead >= iAudioCommandCount)
    {
        iAudioCommandRead = 0;
        iAudioCommandCount = 0;
        return EFalse;
    }
    aCommand = iAudioCommands[iAudioCommandRead++];
    if (iAudioCommandRead >= iAudioCommandCount)
    {
        iAudioCommandRead = 0;
        iAudioCommandCount = 0;
    }
    return ETrue;
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

#ifdef EASYRPG_RUBY300
    pthread_attr_t attributes;
    if (pthread_attr_init(&attributes) != 0)
    {
        self->SignalStarted(KErrGeneral);
        return KErrGeneral;
    }
    const int stackResult = pthread_attr_setstacksize(&attributes, 0x40000);
    pthread_t thread = 0;
    const int createResult = stackResult == 0
        ? pthread_create(&thread, &attributes, &CRgssVmWorker::PthreadEntry, self)
        : stackResult;
    pthread_attr_destroy(&attributes);
    if (createResult != 0)
    {
        self->SignalStarted(KErrGeneral);
        return KErrGeneral;
    }
    pthread_join(thread, NULL);
    return self->iExitResult;
#else
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
#endif
}

#ifdef EASYRPG_RUBY300
TAny* CRgssVmWorker::PthreadEntry(TAny* aContext)
{
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (!self)
        return NULL;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup)
    {
        self->iExitResult = KErrNoMemory;
        self->SignalStarted(KErrNoMemory);
        return NULL;
    }
    self->ThreadMain();
    delete cleanup;
    return NULL;
}
#endif

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
    iGlyphCache = CRgssGlyphCache::NewL();
    EasyRpgRgssBitmapSetGlyphProvider(iGlyphCache, &CRgssGlyphCache::Resolve);
    CleanupStack::PushL(TCleanupItem(&CRgssVmWorker::DestroyGlyphCache, this));

    User::LeaveIfError(iFs.Connect());
    CleanupClosePushL(iFs);
    PrepareSaveDirectoryL(iFs, iGamePath);
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
    if (scriptCount <= 0)
        User::Leave(KErrCorrupt);
    iScriptEntries = static_cast<EasyRpgRgssScriptEntry*>(
        User::Alloc(scriptCount * static_cast<TInt>(sizeof(EasyRpgRgssScriptEntry))));
    if (!iScriptEntries)
        User::Leave(KErrNoMemory);
    iScriptCount = scriptCount;
    CleanupStack::PushL(TCleanupItem(&CRgssVmWorker::FreeScriptEntries, this));
    TInt indexedCount = 0;
    const TInt indexResult = EasyRpgRgssScriptArchiveInspect(&archiveReader,
                                                              iScriptEntries,
                                                              iScriptCount,
                                                              &indexedCount);
    if (indexResult != ERgssScriptArchiveOk || indexedCount != iScriptCount)
        User::Leave(indexResult == ERgssScriptArchiveErrorNoMemory
            ? KErrNoMemory : KErrCorrupt);

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
    host.text_decode = &CRgssVmWorker::VmTextDecode;
    host.frame_sync = &CRgssVmWorker::VmFrameSync;
    host.script_count = &CRgssVmWorker::VmScriptCount;
    host.script_info = &CRgssVmWorker::VmScriptInfo;
    host.script_name = &CRgssVmWorker::VmScriptName;
    host.script_inflate = &CRgssVmWorker::VmScriptInflate;
    host.release = &CRgssVmWorker::VmRelease;
    host.dirty = &CRgssVmWorker::VmDirty;
    host.audio = &CRgssVmWorker::VmAudio;

    const TInt initResult = EasyRpgRgssVmInitialize(iVersion, &host);
    if (initResult != ERgssVmOk)
    {
        const TInt error = VmResultToError(initResult);
        SignalStarted(error);
        iFrameResume.Wait();
        User::Leave(error);
    }
    iVmInitialized = ETrue;
    CleanupStack::PushL(TCleanupItem(&CRgssVmWorker::ShutdownVm, this));
    SignalStarted(KErrNone);

    const TInt loadResult = EasyRpgRgssVmLoadScripts(scriptsPath);
    if (loadResult != ERgssVmOk && !iStopRequested)
    {
        const TInt error = VmResultToError(loadResult);
        User::Leave(error);
    }

    CleanupStack::PopAndDestroy();
    iVmInitialized = EFalse;
    CleanupStack::PopAndDestroy();
    CleanupStack::PopAndDestroy();
    CleanupStack::PopAndDestroy(iResources);
    iResources = NULL;
    CleanupStack::PopAndDestroy(&iFs);
    CleanupStack::PopAndDestroy();
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

void CRgssVmWorker::DestroyGlyphCache(TAny* aContext)
{
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    EasyRpgRgssBitmapSetGlyphProvider(NULL, NULL);
    if (!self)
        return;
    delete self->iGlyphCache;
    self->iGlyphCache = NULL;
}

void CRgssVmWorker::FreeScriptEntries(TAny* aContext)
{
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (!self)
        return;
    User::Free(self->iScriptEntries);
    self->iScriptEntries = NULL;
    self->iScriptCount = 0;
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
    if (error == KErrNone)
        return KErrNone;
    TRAP(error, TEncoding::ShiftJisToUnicodeL(source, aTarget));
    return error;
}

int CRgssVmWorker::VmFileSize(void* aContext, const char* aRelativePath)
{
    if (!aContext)
        return ERgssVmErrorInvalidArgument;
#ifdef EASYRPG_RUBY300
    if (aRelativePath && strncmp(aRelativePath, "Data/", 5) == 0)
    {
        const char* name = strrchr(aRelativePath, '/');
        name = name ? name + 1 : aRelativePath;
        TBuf<64> threadName;
        TBuf<16> shortName;
        while (*name && *name != '.' && shortName.Length() < shortName.MaxLength())
            shortName.Append(static_cast<TUint16>(*name++));
        TInt allocated = 0;
        User::Heap().AllocSize(allocated);
        TInt largest = 0;
        const TInt free = User::Heap().Available(largest);
        threadName.Format(_L("D%S-S%d-A%d-F%d"), &shortName,
                          User::Heap().Size() / 1024,
                          allocated / 1024,
                          free / 1024);
        User::RenameThread(threadName);
    }
#endif
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
#ifdef EASYRPG_RUBY300
    if (aRelativePath && strcmp(aRelativePath, "Data/Map001.rxdata") == 0)
    {
        TBuf<32> threadName;
        threadName.Format(_L("RGSS-M1-O%d-N%d"), aOffset, aBytes);
        User::RenameThread(threadName);
    }
#endif
    return static_cast<CRgssVmWorker*>(aContext)->ReadFile(aRelativePath,
                                                           aOffset,
                                                           aBuffer,
                                                           aBytes);
}

unsigned int CRgssVmWorker::VmTicks(void*)
{
    static TUint32 tickPeriod = 0;
    if (tickPeriod == 0)
    {
        TTimeIntervalMicroSeconds32 period;
        if (UserHal::TickPeriod(period) != KErrNone || period.Int() <= 0)
            return 0;
        tickPeriod = static_cast<TUint32>(period.Int());
    }
    return User::NTickCount() * tickPeriod;
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
#ifdef EASYRPG_RUBY300
    const char* name = strrchr(aRelativePath, '/');
    name = name ? name + 1 : aRelativePath;
    TBuf<64> threadName;
    TBuf<20> shortName;
    while (*name && *name != '.' && shortName.Length() < shortName.MaxLength())
        shortName.Append(static_cast<TUint16>(*name++));
    TInt allocated = 0;
    User::Heap().AllocSize(allocated);
    TInt largest = 0;
    const TInt free = User::Heap().Available(largest);
    threadName.Format(_L("B%S-S%d-A%d-F%d"), &shortName,
                      User::Heap().Size() / 1024,
                      allocated / 1024,
                      free / 1024);
    User::RenameThread(threadName);
#endif

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

int CRgssVmWorker::VmTextDecode(void*,
                                const unsigned char* aText,
                                int aTextLength,
                                unsigned int* aCodepoints,
                                int aCapacity)
{
    if (!aText || aTextLength < 0 || !aCodepoints || aCapacity < aTextLength)
        return ERgssVmErrorInvalidArgument;
    if (aTextLength == 0)
        return 0;

    TInt count = 0;
    TRAPD(error,
        HBufC* unicode = HBufC::NewLC(aTextLength);
        TPtr output = unicode->Des();
        TPtrC8 source(aText, aTextLength);
        TEncoding::ShiftJisToUnicodeL(source, output);
        TEncoding::NormalizeJapaneseNfcInPlace(output);
        if (output.Length() > aCapacity)
            User::Leave(KErrOverflow);
        for (TInt index = 0; index < output.Length(); ++index)
            aCodepoints[index] = static_cast<TUint16>(output[index]);
        count = output.Length();
        CleanupStack::PopAndDestroy(unicode);
    );
    if (error != KErrNone)
        return error == KErrNoMemory ? ERgssVmErrorRuntime : ERgssVmErrorScript;
    return count;
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

int CRgssVmWorker::VmScriptCount(void* aContext)
{
    if (!aContext)
        return ERgssVmErrorInvalidArgument;
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    return self->iScriptEntries ? self->iScriptCount : ERgssVmErrorNotInitialized;
}

int CRgssVmWorker::VmScriptInfo(void* aContext,
                                int aIndex,
                                int* aId,
                                int* aNameLength)
{
    if (!aContext || !aId || !aNameLength)
        return ERgssVmErrorInvalidArgument;
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (!self->iScriptEntries)
        return ERgssVmErrorNotInitialized;
    if (aIndex < 0 || aIndex >= self->iScriptCount)
        return ERgssVmErrorInvalidArgument;
    const EasyRpgRgssScriptEntry& entry = self->iScriptEntries[aIndex];
    *aId = entry.id;
    *aNameLength = entry.name_length;
    return ERgssVmOk;
}

int CRgssVmWorker::VmScriptName(void* aContext,
                                int aIndex,
                                void* aBuffer,
                                int aCapacity)
{
    if (!aContext || aIndex < 0 || aCapacity < 0 ||
        (aCapacity > 0 && !aBuffer))
        return ERgssVmErrorInvalidArgument;
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (!self->iScriptEntries || !self->iResources)
        return ERgssVmErrorNotInitialized;
    if (aIndex >= self->iScriptCount)
        return ERgssVmErrorInvalidArgument;
    const EasyRpgRgssScriptEntry& entry = self->iScriptEntries[aIndex];
    if (aCapacity < entry.name_length)
        return ERgssVmErrorInvalidArgument;
    if (entry.name_length == 0)
        return 0;

    TPtr8 buffer(static_cast<TUint8*>(aBuffer), 0, entry.name_length);
    TRAPD(error, self->iResources->ReadStoredFileL(self->iScriptDataOffset,
                                                   self->iScriptFileSize,
                                                   entry.name_offset,
                                                   buffer));
    return error == KErrNone ? buffer.Length() : ERgssVmErrorFile;
}

int CRgssVmWorker::VmScriptInflate(void* aContext,
                                   int aIndex,
                                   unsigned char** aSource,
                                   int* aSourceLength)
{
    if (!aContext || !aSource || !aSourceLength)
        return ERgssVmErrorInvalidArgument;
    CRgssVmWorker* self = static_cast<CRgssVmWorker*>(aContext);
    if (!self->iScriptEntries || !self->iResources)
        return ERgssVmErrorNotInitialized;
    if (aIndex < 0 || aIndex >= self->iScriptCount)
        return ERgssVmErrorInvalidArgument;
#ifdef EASYRPG_RUBY300
    TBuf<24> threadName;
    threadName.Format(_L("RGSS-S%d"), aIndex);
    User::RenameThread(threadName);
#endif

    EasyRpgRgssScriptArchiveReader reader;
    reader.context = self;
    reader.size = self->iScriptFileSize;
    reader.read = &CRgssVmWorker::ScriptArchiveRead;
    const TInt result = EasyRpgRgssScriptArchiveInflate(&reader,
                                                         &self->iScriptEntries[aIndex],
                                                         ScriptAllocate,
                                                         ScriptRelease,
                                                         NULL,
                                                         4 * 1024 * 1024,
                                                         aSource,
                                                         aSourceLength);
    if (result == ERgssScriptArchiveOk)
        return ERgssVmOk;
    if (result == ERgssScriptArchiveErrorNoMemory ||
        result == ERgssScriptArchiveErrorCapacity)
        return ERgssVmErrorRuntime;
    return ERgssVmErrorScript;
}

void CRgssVmWorker::VmRelease(void*, void* aMemory)
{
    User::Free(aMemory);
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

int CRgssVmWorker::VmAudio(void* aContext,
                           int aCommand,
                           const char* aRelativePath,
                           int aVolume,
                           int aPitch,
                           int aDuration)
{
    if (!aContext)
        return ERgssVmErrorInvalidArgument;
    return static_cast<CRgssVmWorker*>(aContext)->QueueAudioCommand(aCommand,
                                                                    aRelativePath,
                                                                    aVolume,
                                                                    aPitch,
                                                                    aDuration);
}

TInt CRgssVmWorker::QueueAudioCommand(TInt aCommand,
                                      const char* aRelativePath,
                                      TInt aVolume,
                                      TInt aPitch,
                                      TInt aDuration)
{
    if (aCommand < ERgssAudioBgmPlay || aCommand > ERgssAudioSeStop)
        return ERgssVmErrorInvalidArgument;
    const TBool play = aCommand == ERgssAudioBgmPlay ||
                       aCommand == ERgssAudioBgsPlay ||
                       aCommand == ERgssAudioMePlay ||
                       aCommand == ERgssAudioSePlay;
    if (play && (aVolume < 0 || aVolume > 100 || aPitch < 50 || aPitch > 200))
        return ERgssVmErrorInvalidArgument;
    if ((aCommand == ERgssAudioBgmFade ||
         aCommand == ERgssAudioBgsFade ||
         aCommand == ERgssAudioMeFade) && aDuration < 0)
        return ERgssVmErrorInvalidArgument;
    if (iAudioCommandCount >= 32)
        return ERgssVmErrorRuntime;

    TRgssAudioCommand& command = iAudioCommands[iAudioCommandCount];
    command.iCommand = aCommand;
    command.iVolume = aVolume;
    command.iPitch = aPitch;
    command.iDuration = aDuration;
    command.iPath.Zero();
    if (play)
    {
        const TInt pathError = ResolveAudioPath(aRelativePath, command.iPath);
        if (pathError != KErrNone)
            return pathError == KErrNotFound ? ERgssVmErrorFile : ERgssVmErrorInvalidArgument;
    }
    ++iAudioCommandCount;
    return ERgssVmOk;
}

TInt CRgssVmWorker::ResolveAudioPath(const char* aRelativePath, TDes8& aResolved) const
{
    if (!aRelativePath || !aRelativePath[0])
        return KErrArgument;
    const TInt length = User::StringLength(reinterpret_cast<const TUint8*>(aRelativePath));
    if (length <= 0 || length > aResolved.MaxLength())
        return KErrArgument;

    TFileName path;
    TInt error = CopyVmPath(aRelativePath, path);
    if (error != KErrNone)
        return error;
    if (iResources && iResources->Exists(path))
    {
        aResolved.Copy(TPtrC8(reinterpret_cast<const TUint8*>(aRelativePath), length));
        return KErrNone;
    }

    for (TInt index = 0; index < KAudioExtensionCount; ++index)
    {
        const char* extension = KAudioExtensions[index];
        const TInt extensionLength = User::StringLength(reinterpret_cast<const TUint8*>(extension));
        if (length + extensionLength > aResolved.MaxLength())
            continue;
        TFileName candidate;
        candidate.Copy(path);
        if (candidate.Length() + extensionLength > candidate.MaxLength())
            continue;
        for (TInt offset = 0; offset < extensionLength; ++offset)
            candidate.Append(static_cast<TUint16>(extension[offset]));
        if (!iResources || !iResources->Exists(candidate))
            continue;
        aResolved.Copy(TPtrC8(reinterpret_cast<const TUint8*>(aRelativePath), length));
        aResolved.Append(TPtrC8(reinterpret_cast<const TUint8*>(extension), extensionLength));
        return KErrNone;
    }
    return KErrNotFound;
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
