#include <f32file.h>

#include "easyrpgsymbian/PlayerRuntime.h"
#include "easyrpgsymbian/DisplaySurface.h"
#include "easyrpgsymbian/Encoding.h"
#include "easyrpgsymbian/LcfProbe.h"
#include "easyrpgsymbian/ModernCoreProbe.h"
#include "easyrpgsymbian/SymbianAudioBridge.h"
#include "easyrpgsymbian/SymbianDirentBridge.h"
#include "easyrpgsymbian/UpstreamLcfLinkAnchor.h"
#include "easyrpgsymbian/UpstreamPlayerBridge.h"
#include "easyrpgsymbian/ZipVfs.h"

namespace
{
const TInt KMaxAsyncFileBytes = 16 * 1024 * 1024;
_LIT(KProjectBasePath, "C:\\Data\\EasyRPG\\Cache\\Project\\");
_LIT(KSaveBasePath, "C:\\Data\\EasyRPG\\Saves\\");
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

TUint32 HashTextSecondary(const TDesC& aText, TUint32 aSeed)
{
    TUint32 hash = aSeed;
    for (TInt i = 0; i < aText.Length(); ++i)
        hash = ((hash << 5) + hash) ^ static_cast<TUint32>(aText[i]);
    return hash;
}

void NarrowAsciiPathL(const TDesC& aSource, TDes8& aDestination)
{
    aDestination.Zero();
    if (aSource.Length() + 1 > aDestination.MaxLength())
        User::Leave(KErrOverflow);
    for (TInt index = 0; index < aSource.Length(); ++index)
    {
        const TUint16 value = aSource[index];
        if (value > 0x7fu)
            User::Leave(KErrNotSupported);
        aDestination.Append(static_cast<TUint8>(value));
    }
    aDestination.Append(static_cast<TUint8>(0));
}

void Utf8PathL(const char* aSource, TDes& aDestination)
{
    if (!aSource)
        User::Leave(KErrArgument);
    TInt length = 0;
    while (aSource[length] != 0)
        ++length;
    TPtrC8 source(reinterpret_cast<const TUint8*>(aSource), length);
    TEncoding::Utf8ToUnicodeL(source, aDestination);
}

void ResolveResourcePathL(CZipVfs& aResources,
                          const char* aDirectory,
                          const char* aFile,
                          TDes& aArchivePath)
{
    TFileName directory;
    TFileName file;
    Utf8PathL(aDirectory, directory);
    Utf8PathL(aFile, file);

    TFileName base;
    if (directory.Length() > 0 && directory.Compare(_L(".")) != 0)
    {
        base.Copy(directory);
        if (base[base.Length() - 1] != '\\' && base[base.Length() - 1] != '/')
            base.Append('\\');
    }
    base.Append(file);

    if (aResources.Exists(base))
    {
        aArchivePath.Copy(base);
        return;
    }

    const TPtrC extensions[] =
    {
        _L(".xyz"), _L(".png"), _L(".bmp"),
        _L(".lmu"), _L(".lmt"), _L(".ldb"), _L(".lsd"),
        _L(".wav"), _L(".mid"), _L(".midi"), _L(".ogg"), _L(".mp3")
    };
    for (TUint index = 0; index < sizeof(extensions) / sizeof(extensions[0]); ++index)
    {
        TFileName candidate(base);
        candidate.Append(extensions[index]);
        if (aResources.Exists(candidate))
        {
            aArchivePath.Copy(candidate);
            return;
        }
    }

    User::Leave(KErrNotFound);
}

void CreateVirtualProjectTreeL(RFs& aFs, const TDesC& aRoot)
{
    CFileMan* fileMan = CFileMan::NewL(aFs);
    CleanupStack::PushL(fileMan);
    const TInt removeError = fileMan->RmDir(aRoot);
    if (removeError != KErrNone &&
        removeError != KErrNotFound &&
        removeError != KErrPathNotFound)
        User::Leave(removeError);
    CleanupStack::PopAndDestroy(fileMan);

    const TInt error = aFs.MkDirAll(aRoot);
    if (error != KErrNone && error != KErrAlreadyExists)
        User::Leave(error);
}
}

extern "C" int EasyRpgSymbianResolveResource(const char* aDirectory,
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

        TUint32 primary = HashText(gResourceResolver->ArchivePath(), 2166136261u);
        primary = HashText(archivePath, primary);
        TUint32 secondary = HashTextSecondary(gResourceResolver->ArchivePath(), 5381u);
        secondary = HashTextSecondary(archivePath, secondary);

        TFileName cacheName;
        cacheName.Format(_L("res_%08x_%08x"), primary, secondary);
        const TInt dot = archivePath.LocateReverse('.');
        if (dot != KErrNotFound && archivePath.Length() - dot <= 8)
            cacheName.Append(archivePath.Mid(dot));

        TFileName materialized;
        gResourceResolver->MaterializeFileL(archivePath,
                                            cacheName,
                                            materialized,
                                            KMaxAsyncFileBytes);
        TBuf8<KMaxFileName + 1> narrow;
        NarrowAsciiPathL(materialized, narrow);
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
                                     CSymbianDisplaySurface& aDisplaySurface)
{
    CPlayerRuntime* self = new (ELeave) CPlayerRuntime(aResources, aDisplaySurface);
    CleanupStack::PushL(self);
    self->ConstructL(aGameSource);
    CleanupStack::Pop(self);
    return self;
}

CPlayerRuntime::CPlayerRuntime(CZipVfs* aResources,
                               CSymbianDisplaySurface& aDisplaySurface)
    : iResources(aResources),
      iDisplaySurface(aDisplaySurface),
      iPressedKeys(0),
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
    projectRoot.Copy(KProjectBasePath);
    projectRoot.Append(gameDirectory);
    CreateVirtualProjectTreeL(fs, projectRoot);
    EasyRpgSetVirtualProject(iResources, projectRoot);

    TFileName saveRoot;
    saveRoot.Copy(KSaveBasePath);
    saveRoot.Append(gameDirectory);
    const TInt saveError = fs.MkDirAll(saveRoot);
    if (saveError != KErrNone && saveError != KErrAlreadyExists)
        User::Leave(saveError);

    NarrowAsciiPathL(projectRoot, aProjectPath);
    NarrowAsciiPathL(saveRoot, aSavePath);
    CleanupStack::PopAndDestroy(&fs);
}

void CPlayerRuntime::ConstructL(const TDesC& aGameSource)
{
    if (!iResources)
        User::Leave(KErrNotFound);

    gResourceResolver = iResources;

    TLcfProjectSummary projectSummary;
    TBuf<96> gameTitle;
    User::LeaveIfError(TLcfProbe::ProbeProject(*iResources, gameTitle, projectSummary));
    if (EasyRpgModernCoreProbe(5) != 33 || EasyRpgUpstreamLmuLinkAnchor() != 0)
        User::Leave(KErrNotSupported);

    TBuf8<KMaxFileName + 1> projectPath;
    TBuf8<KMaxFileName + 1> savePath;
    PrepareProjectRootL(aGameSource, projectPath, savePath);

    const TInt initError = EasyRpgUpstreamPlayerInit(
        reinterpret_cast<const char*>(projectPath.Ptr()),
        reinterpret_cast<const char*>(savePath.Ptr()),
        iDisplaySurface.Size().iWidth,
        iDisplaySurface.Size().iHeight);
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
    EasyRpgSymbianAudioShutdown();
    EasyRpgClearVirtualProject();
    if (gResourceResolver == iResources)
        gResourceResolver = NULL;
}

void CPlayerRuntime::SetKeyState(TUint32 aKey, TBool aPressed)
{
    if (aPressed)
        iPressedKeys |= aKey;
    else
        iPressedKeys &= ~aKey;
}

void CPlayerRuntime::ReleaseAllKeys()
{
    iPressedKeys = 0;
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

    TUint32 upstreamKeys0 = 0;
    TUint32 upstreamKeys1 = 0;
    TUint32 upstreamKeys2 = 0;
    if ((iPressedKeys & EKeyUp) != 0) upstreamKeys0 |= 1u << 13;
    if ((iPressedKeys & EKeyDown) != 0) upstreamKeys0 |= 1u << 15;
    if ((iPressedKeys & EKeyLeft) != 0) upstreamKeys0 |= 1u << 12;
    if ((iPressedKeys & EKeyRight) != 0) upstreamKeys0 |= 1u << 14;
    if ((iPressedKeys & EKeyDecision) != 0) upstreamKeys0 |= 1u << 4;
    if ((iPressedKeys & EKeyCancel) != 0) upstreamKeys0 |= 1u << 6;
    if ((iPressedKeys & EKeyShift) != 0) upstreamKeys0 |= 1u << 20;
    if ((iPressedKeys & EKeyDebug) != 0) upstreamKeys2 |= 1u << 26;

    const TInt tickResult = EasyRpgUpstreamPlayerTick(
        static_cast<unsigned int>((aLogicFrame * 1000u) / 60u),
        upstreamKeys0,
        upstreamKeys1,
        upstreamKeys2,
        0,
        aRenderFrame ? 1 : 0);
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
