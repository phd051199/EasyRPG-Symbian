#include <f32file.h>
#include <hal.h>
#include <hal_data.h>

#include "PlayerRuntime.h"
#include "DisplaySurface.h"
#include "Encoding.h"
#include "AppConfig.h"
#include "AppPaths.h"
#include "AudioBridge.h"
#include "DirentBridge.h"
#include "UpstreamPlayerBridge.h"
#include "ZipVfs.h"

namespace
{
CZipVfs* gResourceResolver = NULL;

TUint32 HashText(const TDesC& aText, TUint32 aSeed)
{
    TUint32 hash = aSeed;
    for (TInt i = 0; i < aText.Length(); ++i)
    {
        const TUint16 value = aText[i];
        hash ^= static_cast<TUint8>(value & 0xffu);
        hash *= 16777619u;
        hash ^= static_cast<TUint8>((value >> 8) & 0xffu);
        hash *= 16777619u;
    }
    return hash;
}

void ResolveResourcePathL(CZipVfs& aResources,
                          const char* aDirectory,
                          const char* aFile,
                          TDes& aArchivePath)
{
    TFileName directory;
    TFileName file;
    TEncoding::Utf8CStringToUnicodeL(aDirectory, directory);
    TEncoding::Utf8CStringToUnicodeL(aFile, file);

    TFileName base;
    if (directory.Length() > 0 && directory.Compare(_L(".")) != 0)
    {
        TInt requiredLength = directory.Length() + file.Length();
        if (directory[directory.Length() - 1] != '\\' &&
            directory[directory.Length() - 1] != '/')
            ++requiredLength;
        if (requiredLength > base.MaxLength())
            User::Leave(KErrTooBig);
        base.Copy(directory);
        if (base[base.Length() - 1] != '\\' && base[base.Length() - 1] != '/')
            base.Append('\\');
    }
    else if (file.Length() > base.MaxLength())
    {
        User::Leave(KErrTooBig);
    }
    base.Append(file);

    if (aResources.Exists(base))
    {
        if (base.Length() > aArchivePath.MaxLength())
            User::Leave(KErrOverflow);
        aArchivePath.Copy(base);
        return;
    }

    const TPtrC extensions[] =
    {
        _L(".xyz"), _L(".png"), _L(".bmp"),
        _L(".lmu"), _L(".lmt"), _L(".ldb"), _L(".lsd"),
        _L(".wav"), _L(".mid"), _L(".midi"), _L(".ogg"), _L(".mp3"),
        _L(".avi"), _L(".mpg"), _L(".mpeg"), _L(".mp4"), _L(".3gp")
    };
    for (TUint index = 0; index < sizeof(extensions) / sizeof(extensions[0]); ++index)
    {
        if (base.Length() < extensions[index].Length())
            continue;
        if (base.Right(extensions[index].Length()).CompareF(extensions[index]) != 0)
            continue;
        User::Leave(KErrNotFound);
    }

    for (TUint index = 0; index < sizeof(extensions) / sizeof(extensions[0]); ++index)
    {
        if (base.Length() + extensions[index].Length() > base.MaxLength())
            continue;
        TFileName candidate(base);
        candidate.Append(extensions[index]);
        if (aResources.Exists(candidate))
        {
            if (candidate.Length() > aArchivePath.MaxLength())
                User::Leave(KErrOverflow);
            aArchivePath.Copy(candidate);
            return;
        }
    }

    User::Leave(KErrNotFound);
}

TBool ResolveUpstreamKey(TUint32 aKey, TInt& aWord, TUint32& aMask)
{
    switch (aKey)
    {
    case CPlayerRuntime::EKeyUp: aWord = 0; aMask = 1u << 13; break;
    case CPlayerRuntime::EKeyDown: aWord = 0; aMask = 1u << 15; break;
    case CPlayerRuntime::EKeyLeft: aWord = 0; aMask = 1u << 12; break;
    case CPlayerRuntime::EKeyRight: aWord = 0; aMask = 1u << 14; break;
    case CPlayerRuntime::EKeyDecision: aWord = 0; aMask = 1u << 4; break;
    case CPlayerRuntime::EKeyCancel: aWord = 0; aMask = 1u << 6; break;
    case CPlayerRuntime::EKeyShift: aWord = 0; aMask = 1u << 20; break;
    case CPlayerRuntime::EKeyN0: aWord = 0; aMask = 1u << 28; break;
    case CPlayerRuntime::EKeyN1: aWord = 0; aMask = 1u << 29; break;
    case CPlayerRuntime::EKeyN2: aWord = 0; aMask = 1u << 30; break;
    case CPlayerRuntime::EKeyN3: aWord = 0; aMask = 1u << 31; break;
    case CPlayerRuntime::EKeyN4: aWord = 1; aMask = 1u << 0; break;
    case CPlayerRuntime::EKeyN5: aWord = 1; aMask = 1u << 1; break;
    case CPlayerRuntime::EKeyN6: aWord = 1; aMask = 1u << 2; break;
    case CPlayerRuntime::EKeyN7: aWord = 1; aMask = 1u << 3; break;
    case CPlayerRuntime::EKeyN8: aWord = 1; aMask = 1u << 4; break;
    case CPlayerRuntime::EKeyN9: aWord = 1; aMask = 1u << 5; break;
    case CPlayerRuntime::EKeyMultiply: aWord = 2; aMask = 1u << 13; break;
    case CPlayerRuntime::EKeyPlus: aWord = 2; aMask = 1u << 14; break;
    case CPlayerRuntime::EKeyMinus: aWord = 2; aMask = 1u << 15; break;
    case CPlayerRuntime::EKeyPeriod: aWord = 2; aMask = 1u << 16; break;
    case CPlayerRuntime::EKeyDivide: aWord = 2; aMask = 1u << 17; break;
    default: return EFalse;
    }
    return ETrue;
}
}

extern "C" int EasyRpgPlatformFreeRamBytes(void)
{
    TInt biggestBlock = 0;
    const TInt committedFree = User::Available(biggestBlock);
    const RHeap& heap = User::Heap();
    const TInt expandable = Max(0, heap.MaxLength() - heap.Size());
    TInt heapFree = committedFree;
    if (heapFree >= 0)
    {
        if (heapFree > KMaxTInt - expandable)
            heapFree = KMaxTInt;
        else
            heapFree += expandable;
    }
    TInt allocationHeadroom = Max(biggestBlock, expandable);
    if (heapFree >= 0)
        allocationHeadroom = Min(allocationHeadroom, heapFree);
    TInt systemFree = 0;
    const TInt systemError = HAL::Get(HALData::EMemoryRAMFree, systemFree);
    if (heapFree < 0)
        return systemError == KErrNone && systemFree >= 0 ? systemFree : -1;
    if (systemError != KErrNone || systemFree < 0)
        return allocationHeadroom;
    return Min(allocationHeadroom, systemFree);
}

extern "C" int EasyRpgPlatformPrepareAllocation(int aBytes)
{
    if (aBytes <= 0)
        return EasyRpgPlatformFreeRamBytes();
    TInt freeRam = EasyRpgPlatformFreeRamBytes();
    if (freeRam >= aBytes)
        return freeRam;

    const TInt pressure = freeRam >= 0 && freeRam > aBytes / 2 ? 1 : 2;
    EasyRpgUpstreamPlayerTrimMemory(pressure);
    User::Heap().Compress();
    return EasyRpgPlatformFreeRamBytes();
}

extern "C" int EasyRpgResolveResource(const char* aDirectory,
                                               const char* aFile,
                                               char* aPath,
                                               unsigned int aPathCapacity)
{
    if (!gResourceResolver || !aFile || !aPath || aPathCapacity == 0)
        return KErrNotReady;

    TInt result = KErrNone;
    TRAP(result,
    {
        TFileName archivePath;
        ResolveResourcePathL(*gResourceResolver, aDirectory, aFile, archivePath);

        TFileName virtualPath;
        EasyRpgBuildVirtualPathL(archivePath, virtualPath);
        TBuf8<KMaxFileName * 4 + 1> narrow;
        TEncoding::UnicodeToUtf8CStringL(virtualPath, narrow);
        const TInt length = narrow.Length() - 1;
        if (length < 0 || static_cast<unsigned int>(length + 1) > aPathCapacity)
            User::Leave(KErrOverflow);
        for (TInt i = 0; i < length; ++i)
            aPath[i] = static_cast<char>(narrow[i]);
        aPath[length] = 0;
    });
    return result;
}

CPlayerRuntime* CPlayerRuntime::NewL(CZipVfs* aResources,
                                     const TDesC& aGameSource,
                                     CDisplaySurface& aDisplaySurface)
{
    CPlayerRuntime* self = new (ELeave) CPlayerRuntime(aResources, aDisplaySurface);
    CleanupStack::PushL(self);
    self->ConstructL(aGameSource);
    CleanupStack::Pop(self);
    return self;
}

CPlayerRuntime::CPlayerRuntime(CZipVfs* aResources,
                               CDisplaySurface& aDisplaySurface)
    : iResources(aResources),
      iDisplaySurface(aDisplaySurface),
      iUpstreamKeys0(0),
      iUpstreamKeys1(0),
      iUpstreamKeys2(0),
      iTickMs(16),
      iTickRemainder(40),
      iPlayerRunning(EFalse)
{
}

void CPlayerRuntime::PrepareProjectRootL(const TDesC& aGameSource,
                                         TDes8& aProjectPath,
                                         TDes8& aSavePath)
{
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);

    const TUint32 gameHash = HashText(aGameSource, 2166136261u);
    TBuf<16> gameDirectory;
    gameDirectory.Format(_L("%08x\\"), gameHash);

    TFileName projectRoot;
    EasyRpgPaths::BuildL(projectRoot, EasyRpgApp::KVirtualProjectDirectorySuffix);
    projectRoot.Append(gameDirectory);
    EasyRpgSetVirtualProject(iResources, projectRoot);

    TFileName saveRoot;
    EasyRpgPaths::BuildL(saveRoot, EasyRpgApp::KSaveDirectorySuffix);
    saveRoot.Append(gameDirectory);
    const TInt saveError = fs.MkDirAll(saveRoot);
    if (saveError != KErrNone && saveError != KErrAlreadyExists)
        User::Leave(saveError);

    TEncoding::UnicodeToUtf8CStringL(projectRoot, aProjectPath);
    TEncoding::UnicodeToUtf8CStringL(saveRoot, aSavePath);
    CleanupStack::PopAndDestroy(&fs);
}

void CPlayerRuntime::ConstructL(const TDesC& aGameSource)
{
    if (!iResources)
        User::Leave(KErrNotFound);

    gResourceResolver = iResources;

    TBuf8<KMaxFileName + 1> projectPath;
    TBuf8<KMaxFileName + 1> savePath;
    PrepareProjectRootL(aGameSource, projectPath, savePath);

    TUint16* framebuffer = iDisplaySurface.BeginRenderL();
    const TInt bindError = EasyRpgUpstreamPlayerBindFramebuffer(
        framebuffer,
        iDisplaySurface.Size().iWidth,
        iDisplaySurface.Size().iHeight,
        iDisplaySurface.RenderStrideBytes());
    if (bindError != 0)
    {
        iDisplaySurface.EndRender();
        EasyRpgClearVirtualProject();
        User::Leave(KErrNotSupported);
    }

    const TInt initError = EasyRpgUpstreamPlayerInit(
        reinterpret_cast<const char*>(projectPath.Ptr()),
        reinterpret_cast<const char*>(savePath.Ptr()),
        iDisplaySurface.Size().iWidth,
        iDisplaySurface.Size().iHeight);
    iDisplaySurface.EndRender();
    if (initError != 0)
    {
        EasyRpgClearVirtualProject();
        User::Leave(KErrNotSupported);
    }

    iPlayerRunning = ETrue;
}

CPlayerRuntime::~CPlayerRuntime()
{
    if (iPlayerRunning)
        EasyRpgUpstreamPlayerShutdown();
    iPlayerRunning = EFalse;
    EasyRpgAudioShutdown();
    EasyRpgClearVirtualProject();
    if (gResourceResolver == iResources)
        gResourceResolver = NULL;
}

void CPlayerRuntime::SetKeyState(TUint32 aKey, TBool aPressed)
{
    TInt word = 0;
    TUint32 mask = 0;
    if (!ResolveUpstreamKey(aKey, word, mask))
        return;
    TUint32* state = word == 0 ? &iUpstreamKeys0 : word == 1 ? &iUpstreamKeys1 : &iUpstreamKeys2;
    if (aPressed)
        *state |= mask;
    else
        *state &= ~mask;
}

void CPlayerRuntime::ReleaseAllKeys()
{
    iUpstreamKeys0 = 0;
    iUpstreamKeys1 = 0;
    iUpstreamKeys2 = 0;
}

void CPlayerRuntime::Suspend()
{
    ReleaseAllKeys();
    if (iPlayerRunning)
        EasyRpgUpstreamPlayerPause();
}

void CPlayerRuntime::Resume()
{
    if (iPlayerRunning)
        EasyRpgUpstreamPlayerResume();
}

void CPlayerRuntime::TickL(TUint32 aLogicFrame, TBool aRenderFrame)
{
    if (!iPlayerRunning)
        return;
    static_cast<void>(aLogicFrame);

    if (aRenderFrame)
    {
        TUint16* framebuffer = iDisplaySurface.BeginRenderL();
        const TInt bindError = EasyRpgUpstreamPlayerBindFramebuffer(
            framebuffer,
            iDisplaySurface.Size().iWidth,
            iDisplaySurface.Size().iHeight,
            iDisplaySurface.RenderStrideBytes());
        if (bindError != 0)
        {
            iDisplaySurface.EndRender();
            User::Leave(KErrNotSupported);
        }
    }

    const TInt tickResult = EasyRpgUpstreamPlayerTick(
        iTickMs,
        iUpstreamKeys0,
        iUpstreamKeys1,
        iUpstreamKeys2,
        0,
        aRenderFrame ? 1 : 0);
    if (aRenderFrame)
        iDisplaySurface.EndRender();
    iTickMs += 16;
    iTickRemainder += 40;
    if (iTickRemainder >= 60)
    {
        ++iTickMs;
        iTickRemainder -= 60;
    }
    if (tickResult < 0)
        User::Leave(KErrGeneral);
    if (tickResult > 0)
    {
        iPlayerRunning = EFalse;
        return;
    }

    if (aRenderFrame)
        iDisplaySurface.PresentL();
}

TBool CPlayerRuntime::IsRunning() const
{
    return iPlayerRunning;
}
