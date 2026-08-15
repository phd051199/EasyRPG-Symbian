#include <e32hal.h>
#include <fbs.h>

#include "AudioBridge.h"
#include "DirentBridge.h"
#include "Encoding.h"
#include "RgssEngine.h"
#include "rgss/RgssRun.h"
#include "ZipVfs.h"
#include "rgss/core/RgssDownscale.h"

#include <stdlib.h>
#include <string.h>

namespace
{
_LIT(KVirtualProjectBasePath, "C:\\Data\\EasyRPG\\Vfs\\");
_LIT(KSaveBasePath, "C:\\Data\\EasyRPG\\Saves\\");
_LIT(KLogDirectory, "C:\\Data\\EasyRPG\\");
_LIT(KLogPath, "C:\\Data\\EasyRPG\\script-runtime.log");
_LIT(KXpScripts, "Data/Scripts.rxdata");
_LIT(KVxScripts, "Data/Scripts.rvdata");
_LIT(KVxaScripts, "Data/Scripts.rvdata2");

const TInt KOutputWidth = 320;
const TInt KOutputHeight = 240;
const TInt KMaxHostReadBytes = 48 * 1024 * 1024;
const TInt KMaxSaveBytes = 16 * 1024 * 1024;

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

void Utf8PathL(const char* aSource, TDes& aDestination)
{
    if (!aSource || !aSource[0])
        User::Leave(KErrArgument);
    TInt length = 0;
    while (aSource[length])
        ++length;
    const TPtrC8 source(reinterpret_cast<const TUint8*>(aSource), length);
    TEncoding::Utf8ToUnicodeL(source, aDestination);
    for (TInt i = 0; i < aDestination.Length(); ++i)
        if (aDestination[i] == '/')
            aDestination[i] = '\\';
}

TBool SafeRelativePath(const TDesC& aPath)
{
    if (aPath.Length() <= 0 || aPath[0] == '\\' || aPath[0] == '/')
        return EFalse;
    if (aPath.Locate(':') >= 0)
        return EFalse;
    for (TInt i = 0; i + 1 < aPath.Length(); ++i)
    {
        if (aPath[i] != '.' || aPath[i + 1] != '.')
            continue;
        const TBool leftBoundary = i == 0 || aPath[i - 1] == '\\' || aPath[i - 1] == '/';
        const TBool rightBoundary = i + 2 == aPath.Length() ||
            aPath[i + 2] == '\\' || aPath[i + 2] == '/';
        if (leftBoundary && rightBoundary)
            return EFalse;
    }
    return ETrue;
}

TUint32 ButtonBit(TInt aCode)
{
    return aCode >= 0 && aCode < 32 ? (1u << aCode) : 0;
}

TInt ClampVolume(TInt aVolume)
{
    if (aVolume < 0) return 0;
    if (aVolume > 100) return 100;
    return aVolume;
}

TInt ClampPitch(TInt aPitch)
{
    if (aPitch < 50) return 50;
    if (aPitch > 150) return 150;
    return aPitch;
}
}

CRgssEngine* CRgssEngine::NewL(RFs& aFs, const TDesC& aArchivePath)
{
    CRgssEngine* self = new (ELeave) CRgssEngine(aFs);
    CleanupStack::PushL(self);
    self->ConstructL(aArchivePath);
    CleanupStack::Pop(self);
    return self;
}

CRgssEngine::CRgssEngine(RFs& aFs)
    : iFs(aFs), iResources(NULL), iGeneration(0), iScreenDevice(NULL),
      iWindowGroup(NULL), iWindow(NULL), iWindowGc(NULL),
      iBitmap(NULL), iWindowReady(EFalse), iNativeWidth(0),
      iNativeHeight(0), iHeldButtons(0), iPulseButtons(0),
      iRightSoftHeld(EFalse)
{
}

void CRgssEngine::ConstructL(const TDesC& aArchivePath)
{
    if (aArchivePath.Length() <= 0 || aArchivePath.Length() > iArchivePath.MaxLength())
        User::Leave(KErrArgument);
    iArchivePath.Copy(aArchivePath);
    iResources = CZipVfs::OpenL(iFs, iArchivePath);
    SelectGenerationL();
    PrepareVirtualProjectL();
    CreateWindowL();
    RgssHostSymbianInstall(this);
}

CRgssEngine::~CRgssEngine()
{
    HostAudioShutdown();
    EasyRpgClearVirtualProject();
    DestroyWindow();
    delete iResources;
    iResources = NULL;
}

void CRgssEngine::SelectGenerationL()
{
    const TBool xp = iResources->Exists(KXpScripts) ||
        iResources->Exists(_L("Game.rgssad"));
    const TBool vx = iResources->Exists(KVxScripts) ||
        iResources->Exists(_L("Game.rgss2a"));
    const TBool vxa = iResources->Exists(KVxaScripts) ||
        iResources->Exists(_L("Game.rgss3a"));
    const TInt count = static_cast<TInt>(xp) + static_cast<TInt>(vx) + static_cast<TInt>(vxa);
    if (count != 1)
        User::Leave(count == 0 ? KErrNotSupported : KErrCorrupt);
    iGeneration = xp ? 1 : (vx ? 2 : 3);
}

void CRgssEngine::PrepareVirtualProjectL()
{
    const TUint32 gameHash = HashText(iArchivePath, 2166136261u);
    TBuf<16> gameDirectory;
    gameDirectory.Format(_L("%08x\\"), gameHash);
    iProjectRoot.Copy(KVirtualProjectBasePath);
    iProjectRoot.Append(gameDirectory);
    EasyRpgSetVirtualProject(iResources, iProjectRoot);
    iSaveRoot.Copy(KSaveBasePath);
    iSaveRoot.Append(gameDirectory);
    const TInt error = iFs.MkDirAll(iSaveRoot);
    if (error != KErrNone && error != KErrAlreadyExists)
        User::Leave(error);
}

void CRgssEngine::CreateWindowL()
{
    User::LeaveIfError(iWs.Connect(iFs));
    iScreenDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(iScreenDevice->Construct(0));
    const TUint32 handle = static_cast<TUint32>(RProcess().Id().Id()) | 1u;
    iWindowGroup = new (ELeave) RWindowGroup(iWs);
    User::LeaveIfError(iWindowGroup->Construct(handle, ETrue));
    iWindowGroup->EnableReceiptOfFocus(ETrue);
    iWindowGroup->AutoForeground(ETrue);
    iWindowGroup->SetOrdinalPosition(0);
    iWindow = new (ELeave) RWindow(iWs);
    User::LeaveIfError(iWindow->Construct(*iWindowGroup, handle + 1));
    iWindow->SetExtent(TPoint(0, 0), iScreenDevice->SizeInPixels());
    iWindow->SetBackgroundColor(TRgb(0, 0, 0));
    iWindow->Activate();
    iWindow->SetVisible(ETrue);
    User::LeaveIfError(iScreenDevice->CreateContext(iWindowGc));
    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iBitmap->Create(TSize(KOutputWidth, KOutputHeight), EColor64K));
    if (iBitmap->DataStride() != KOutputWidth * static_cast<TInt>(sizeof(TUint16)))
        User::Leave(KErrNotSupported);
    iWs.Flush();
    iWindowReady = ETrue;
}

void CRgssEngine::DestroyWindow()
{
    iWindowReady = EFalse;
    delete iBitmap;
    iBitmap = NULL;
    if (iWindowGc)
    {
        iWindowGc->Deactivate();
        delete iWindowGc;
        iWindowGc = NULL;
    }
    if (iWindow)
    {
        iWindow->Close();
        delete iWindow;
        iWindow = NULL;
    }
    if (iWindowGroup)
    {
        iWindowGroup->Close();
        delete iWindowGroup;
        iWindowGroup = NULL;
    }
    delete iScreenDevice;
    iScreenDevice = NULL;
    iWs.Close();
}

void CRgssEngine::RunL()
{
    iLastError.Zero();
    char error[512];
    error[0] = 0;
    const TInt result = RgssRun(iGeneration, error, sizeof(error));
    if (result != 0)
    {
        CopyError(error);
        User::Leave(result);
    }
}

TInt CRgssEngine::Generation() const { return iGeneration; }
const TDesC8& CRgssEngine::LastError() const { return iLastError; }

void CRgssEngine::CopyError(const char* aError)
{
    iLastError.Zero();
    if (!aError) return;
    TInt length = 0;
    while (aError[length] && length < iLastError.MaxLength()) ++length;
    iLastError.Copy(TPtrC8(reinterpret_cast<const TUint8*>(aError), length));
}

int CRgssEngine::HostInit(int aNativeWidth, int aNativeHeight)
{
    if (!iWindowReady || !iBitmap || aNativeWidth <= 0 || aNativeHeight <= 0)
        return KErrNotReady;
    iNativeWidth = aNativeWidth;
    iNativeHeight = aNativeHeight;
    return KErrNone;
}

void CRgssEngine::HostPresent(const unsigned short* aFrame, int aWidth, int aHeight)
{
    if (!aFrame || !iWindowReady || !iBitmap || !iWindowGc || !iWindow ||
        aWidth <= 0 || aHeight <= 0)
        return;

    iBitmap->LockHeap();
    TUint16* destination = reinterpret_cast<TUint16*>(iBitmap->DataAddress());
    if (!destination)
    {
        iBitmap->UnlockHeap();
        return;
    }
    rgss::RgssDownscaleRgb565(aFrame, aWidth, aHeight,
                              destination, KOutputWidth, KOutputHeight);
    iBitmap->UnlockHeap();

    const TSize screen = iScreenDevice->SizeInPixels();
    TPoint position(0, 0);
    if (screen.iWidth > KOutputWidth)
        position.iX = (screen.iWidth - KOutputWidth) / 2;
    if (screen.iHeight > KOutputHeight)
        position.iY = (screen.iHeight - KOutputHeight) / 2;

    iWindow->Invalidate();
    iWindow->BeginRedraw();
    iWindowGc->Activate(*iWindow);
    iWindowGc->BitBlt(position, iBitmap,
                      TRect(TPoint(0, 0), TSize(KOutputWidth, KOutputHeight)));
    iWindowGc->Deactivate();
    iWindow->EndRedraw();
    iWs.Flush();
    PumpActiveObjects();
}

void CRgssEngine::PumpActiveObjects()
{
    for (TInt count = 0; count < 8; ++count)
    {
        TInt error = KErrNone;
        if (!CActiveScheduler::RunIfReady(error, CActive::EPriorityStandard))
            break;
        if (error != KErrNone)
            break;
    }
}

TUint32 CRgssEngine::MapKey(const TKeyEvent& aKey) const
{
    switch (aKey.iScanCode)
    {
    case EStdKeyUpArrow:
    case 'W': return ButtonBit(8);
    case EStdKeyDownArrow:
    case 'S': return ButtonBit(2);
    case EStdKeyLeftArrow:
    case 'A': return ButtonBit(4);
    case EStdKeyRightArrow:
    case 'D': return ButtonBit(6);
    case EStdKeyEnter:
    case EStdKeyDevice3:
    case EStdKeySpace:
    case 'Z': return ButtonBit(13);
    case EStdKeyEscape:
    case 'X': return ButtonBit(12);
    case EStdKeyLeftShift:
    case EStdKeyRightShift: return ButtonBit(21) | ButtonBit(11);
    case 'C': return ButtonBit(11);
    default: break;
    }

    switch (aKey.iCode)
    {
    case EKeyUpArrow:
    case 'W':
    case 'w': return ButtonBit(8);
    case EKeyDownArrow:
    case 'S':
    case 's': return ButtonBit(2);
    case EKeyLeftArrow:
    case 'A':
    case 'a': return ButtonBit(4);
    case EKeyRightArrow:
    case 'D':
    case 'd': return ButtonBit(6);
    case EKeyEnter:
    case EKeyDevice3:
    case 'Z':
    case 'z':
    case ' ': return ButtonBit(13);
    case EKeyEscape:
    case 'X':
    case 'x': return ButtonBit(12);
    case EKeyLeftShift:
    case EKeyRightShift: return ButtonBit(21) | ButtonBit(11);
    case 'C':
    case 'c': return ButtonBit(11);
    default: return 0;
    }
}

void CRgssEngine::HandleWindowEvent(const TWsEvent& aEvent)
{
    if (aEvent.Type() == EEventFocusLost)
    {
        iHeldButtons = 0;
        iPulseButtons = 0;
        iRightSoftHeld = EFalse;
        return;
    }
    if (aEvent.Type() != EEventKeyDown && aEvent.Type() != EEventKeyUp &&
        aEvent.Type() != EEventKey)
        return;

    const TKeyEvent& key = *aEvent.Key();
    const TBool rightSoft = key.iScanCode == EStdKeyDevice1 || key.iCode == EKeyDevice1;
    const TUint32 buttons = rightSoft ? ButtonBit(12) : MapKey(key);
    if (buttons == 0)
        return;

    if (aEvent.Type() == EEventKeyDown)
    {
        iHeldButtons |= buttons;
        if (rightSoft) iRightSoftHeld = ETrue;
    }
    else if (aEvent.Type() == EEventKeyUp)
    {
        iHeldButtons &= ~buttons;
        if (rightSoft) iRightSoftHeld = EFalse;
    }
    else if ((iHeldButtons & buttons) == 0)
    {
        iPulseButtons |= buttons;
        if (rightSoft) iRightSoftHeld = EFalse;
    }
}

void CRgssEngine::DrainWindowEvents()
{
    if (!iWindowReady)
        return;
    for (TInt count = 0; count < 24; ++count)
    {
        TRequestStatus status;
        iWs.EventReady(&status);
        if (status == KRequestPending)
        {
            iWs.EventReadyCancel();
            User::WaitForRequest(status);
            break;
        }
        if (status.Int() != KErrNone)
            break;
        TWsEvent event;
        iWs.GetEvent(event);
        HandleWindowEvent(event);
    }
}

TUint32 CRgssEngine::HostPollInput()
{
    PumpActiveObjects();
    DrainWindowEvents();
    const TUint32 result = iHeldButtons | iPulseButtons;
    iPulseButtons = 0;
    return result;
}

void CRgssEngine::BuildArchivePathL(const char* aPath, TDes& aResult) const
{
    Utf8PathL(aPath, aResult);
    if (!SafeRelativePath(aResult))
        User::Leave(KErrArgument);
}

void CRgssEngine::BuildSavePathL(const char* aPath, TDes& aResult) const
{
    TFileName relative;
    BuildArchivePathL(aPath, relative);
    if (iSaveRoot.Length() + relative.Length() > aResult.MaxLength())
        User::Leave(KErrOverflow);
    aResult.Copy(iSaveRoot);
    aResult.Append(relative);
}

TBool CRgssEngine::ReadSaveFileL(const TDesC& aPath, HBufC8*& aData) const
{
    aData = NULL;
    RFile file;
    TInt error = file.Open(iFs, aPath, EFileRead | EFileShareReadersOnly);
    if (error == KErrNotFound || error == KErrPathNotFound)
        return EFalse;
    User::LeaveIfError(error);
    CleanupClosePushL(file);
    TInt size = 0;
    User::LeaveIfError(file.Size(size));
    if (size < 0 || size > KMaxSaveBytes)
        User::Leave(KErrTooBig);
    aData = HBufC8::NewL(size);
    TPtr8 bytes(aData->Des());
    User::LeaveIfError(file.Read(bytes));
    if (bytes.Length() != size)
        User::Leave(KErrEof);
    CleanupStack::PopAndDestroy(&file);
    return ETrue;
}

int CRgssEngine::HostFileExists(const char* aPath)
{
    TInt result = 0;
    TRAPD(error,
        TFileName savePath;
        BuildSavePathL(aPath, savePath);
        TEntry entry;
        if (iFs.Entry(savePath, entry) == KErrNone && !entry.IsDir())
            result = 1;
        else
        {
            TFileName archivePath;
            BuildArchivePathL(aPath, archivePath);
            result = iResources->Exists(archivePath) ? 1 : 0;
        }
    );
    return error == KErrNone ? result : 0;
}

long CRgssEngine::HostFileRead(const char* aPath, unsigned char** aOutput)
{
    if (!aOutput)
        return -1;
    *aOutput = NULL;
    long result = -1;
    TRAPD(error,
        HBufC8* data = NULL;
        TFileName savePath;
        BuildSavePathL(aPath, savePath);
        if (!ReadSaveFileL(savePath, data))
        {
            TFileName archivePath;
            BuildArchivePathL(aPath, archivePath);
            data = iResources->ReadFileLC(archivePath, KMaxHostReadBytes);
            CleanupStack::Pop(data);
        }
        CleanupStack::PushL(data);
        unsigned char* output = static_cast<unsigned char*>(malloc(data->Length()));
        if (!output && data->Length() > 0)
            User::Leave(KErrNoMemory);
        if (data->Length() > 0)
            memcpy(output, data->Ptr(), data->Length());
        result = data->Length();
        *aOutput = output;
        CleanupStack::PopAndDestroy(data);
    );
    if (error != KErrNone)
    {
        free(*aOutput);
        *aOutput = NULL;
        return -1;
    }
    return result;
}

long CRgssEngine::HostFileWrite(const char* aPath, const unsigned char* aData, long aSize)
{
    if (aSize < 0 || aSize > KMaxSaveBytes || (aSize > 0 && !aData))
        return -1;
    long result = -1;
    TRAPD(error,
        TFileName path;
        BuildSavePathL(aPath, path);
        const TParsePtrC parse(path);
        const TInt dirError = iFs.MkDirAll(parse.DriveAndPath());
        if (dirError != KErrNone && dirError != KErrAlreadyExists)
            User::Leave(dirError);
        RFile file;
        User::LeaveIfError(file.Replace(iFs, path, EFileWrite | EFileShareExclusive));
        CleanupClosePushL(file);
        const TPtrC8 bytes(reinterpret_cast<const TUint8*>(aData), static_cast<TInt>(aSize));
        User::LeaveIfError(file.Write(bytes));
        User::LeaveIfError(file.Flush());
        CleanupStack::PopAndDestroy(&file);
        result = aSize;
    );
    return error == KErrNone ? result : -1;
}

int CRgssEngine::HostFileDelete(const char* aPath)
{
    TInt result = KErrGeneral;
    TRAPD(error,
        TFileName path;
        BuildSavePathL(aPath, path);
        result = iFs.Delete(path);
    );
    return error == KErrNone ? result : error;
}

int CRgssEngine::HostFileRename(const char* aOldPath, const char* aNewPath)
{
    TInt result = KErrGeneral;
    TRAPD(error,
        TFileName oldPath;
        TFileName newPath;
        BuildSavePathL(aOldPath, oldPath);
        BuildSavePathL(aNewPath, newPath);
        const TParsePtrC parse(newPath);
        const TInt dirError = iFs.MkDirAll(parse.DriveAndPath());
        if (dirError != KErrNone && dirError != KErrAlreadyExists)
            User::Leave(dirError);
        result = iFs.Rename(oldPath, newPath);
    );
    return error == KErrNone ? result : error;
}

void CRgssEngine::BuildVirtualAudioPathL(const char* aPath, TDes8& aResult) const
{
    TFileName relative;
    BuildArchivePathL(aPath, relative);
    TFileName virtualPath;
    EasyRpgBuildVirtualPathL(relative, virtualPath);
    aResult.Zero();
    TEncoding::UnicodeToUtf8L(virtualPath, aResult);
    if (aResult.Length() >= aResult.MaxLength())
        User::Leave(KErrOverflow);
    aResult.Append(0);
}

int CRgssEngine::HostBgmPlay(const char* aPath, int aVolume, int aPitch)
{
    TInt result = KErrGeneral;
    TRAPD(error,
        TBuf8<KMaxFileName * 4 + 1> path;
        BuildVirtualAudioPathL(aPath, path);
        result = EasyRpgAudioPlayBgm(reinterpret_cast<const char*>(path.Ptr()),
                                     ClampVolume(aVolume), ClampPitch(aPitch), 0);
    );
    return error == KErrNone ? result : error;
}

void CRgssEngine::HostBgmStop() { EasyRpgAudioStopBgm(); }
void CRgssEngine::HostBgmFade(int aMilliseconds) { EasyRpgAudioFadeBgm(aMilliseconds); }
int CRgssEngine::HostBgmPos() { return static_cast<int>(EasyRpgAudioBgmTicks()); }
void CRgssEngine::HostBgmPause() { EasyRpgAudioPauseBgm(); }
void CRgssEngine::HostBgmResume() { EasyRpgAudioResumeBgm(); }

int CRgssEngine::HostBgsPlay(const char* aPath, int aVolume, int aPitch)
{
    TInt result = KErrGeneral;
    TRAPD(error,
        TBuf8<KMaxFileName * 4 + 1> path;
        BuildVirtualAudioPathL(aPath, path);
        result = EasyRpgAudioPlayBgs(reinterpret_cast<const char*>(path.Ptr()),
                                     ClampVolume(aVolume), ClampPitch(aPitch));
    );
    return error == KErrNone ? result : error;
}
void CRgssEngine::HostBgsStop() { EasyRpgAudioStopBgs(); }
void CRgssEngine::HostBgsFade(int aMilliseconds) { EasyRpgAudioFadeBgs(aMilliseconds); }

int CRgssEngine::HostMePlay(const char* aPath, int aVolume, int aPitch)
{
    TInt result = KErrGeneral;
    TRAPD(error,
        TBuf8<KMaxFileName * 4 + 1> path;
        BuildVirtualAudioPathL(aPath, path);
        result = EasyRpgAudioPlayMe(reinterpret_cast<const char*>(path.Ptr()),
                                    ClampVolume(aVolume), ClampPitch(aPitch));
    );
    return error == KErrNone ? result : error;
}
void CRgssEngine::HostMeStop() { EasyRpgAudioStopMe(); }
void CRgssEngine::HostMeFade(int aMilliseconds) { EasyRpgAudioFadeMe(aMilliseconds); }

int CRgssEngine::HostSePlay(const char* aPath, int aVolume, int aPitch)
{
    TInt result = KErrGeneral;
    TRAPD(error,
        TBuf8<KMaxFileName * 4 + 1> path;
        BuildVirtualAudioPathL(aPath, path);
        result = EasyRpgAudioPlaySe(reinterpret_cast<const char*>(path.Ptr()),
                                    ClampVolume(aVolume), ClampPitch(aPitch));
    );
    return error == KErrNone ? result : error;
}
void CRgssEngine::HostSeStop() { EasyRpgAudioStopSe(); }

void CRgssEngine::HostAudioUpdate()
{
    EasyRpgAudioUpdate();
    PumpActiveObjects();
}

void CRgssEngine::HostAudioShutdown()
{
    EasyRpgAudioShutdown();
}

void CRgssEngine::HostLog(const char* aMessage)
{
    if (!aMessage)
        return;
    TRAP_IGNORE(
        const TInt dirError = iFs.MkDirAll(KLogDirectory);
        if (dirError != KErrNone && dirError != KErrAlreadyExists)
            User::Leave(dirError);
        RFile file;
        TInt error = file.Open(iFs, KLogPath, EFileWrite | EFileShareAny);
        if (error == KErrNotFound || error == KErrPathNotFound)
            error = file.Replace(iFs, KLogPath, EFileWrite | EFileShareAny);
        User::LeaveIfError(error);
        CleanupClosePushL(file);
        TInt position = 0;
        User::LeaveIfError(file.Seek(ESeekEnd, position));
        TInt length = 0;
        while (aMessage[length] && length < 1024)
            ++length;
        const TPtrC8 text(reinterpret_cast<const TUint8*>(aMessage), length);
        User::LeaveIfError(file.Write(text));
        User::LeaveIfError(file.Write(_L8("\r\n")));
        CleanupStack::PopAndDestroy(&file);
    );
}

TUint32 CRgssEngine::HostTicksMs() const
{
    static TInt tickMicroseconds = 0;
    if (tickMicroseconds <= 0)
    {
        TTimeIntervalMicroSeconds32 period;
        if (UserHal::TickPeriod(period) == KErrNone && period.Int() > 0)
            tickMicroseconds = period.Int();
        else
            tickMicroseconds = 1000;
    }
    return static_cast<TUint32>(
        (static_cast<TUint64>(User::TickCount()) * static_cast<TUint64>(tickMicroseconds)) / 1000u);
}

void CRgssEngine::HostDelayMs(TUint32 aMilliseconds)
{
    TUint32 remaining = aMilliseconds;
    while (remaining > 0)
    {
        PumpActiveObjects();
        DrainWindowEvents();
        const TUint32 slice = remaining > 4 ? 4 : remaining;
        User::After(static_cast<TInt>(slice * 1000));
        remaining -= slice;
    }
}
