#include <bitdev.h>
#include <e32keys.h>
#include <f32file.h>
#include <gdi.h>
#include <w32std.h>

#include "AudioBridge.h"
#include "DirentBridge.h"
#include "DisplaySurface.h"
#include "MovieBridge.h"
#include "PerfOverlay.h"
#include "PlayerRuntime.h"
#include "PlayerConfig.h"
#include "RuntimeEngine.h"
#include "UpstreamPlayerBridge.h"
#include "ZipVfs.h"

namespace
{
TInt MemoryPressureLevel(TInt aFreeRam, TInt aCurrentLevel)
{
    if (aFreeRam < 0)
        return aCurrentLevel;
    if (aFreeRam < EasyRpgPlayerConfig::KCriticalMemoryBytes)
        return 2;
    if (aCurrentLevel >= 2 && aFreeRam < EasyRpgPlayerConfig::KCriticalMemoryRecoveryBytes)
        return 2;
    if (aFreeRam < EasyRpgPlayerConfig::KLowMemoryBytes)
        return 1;
    if (aCurrentLevel >= 1 && aFreeRam < EasyRpgPlayerConfig::KLowMemoryRecoveryBytes)
        return 1;
    return 0;
}

TBool RectsOverlap(const TRect& aFirst, const TRect& aSecond)
{
    return aFirst.iTl.iX < aSecond.iBr.iX &&
        aSecond.iTl.iX < aFirst.iBr.iX &&
        aFirst.iTl.iY < aSecond.iBr.iY &&
        aSecond.iTl.iY < aFirst.iBr.iY;
}

}

class CWindowEventPump : public CActive
{
public:
    static CWindowEventPump* NewL(RWsSession& aWs, CRuntimeEngine& aEngine)
    {
        CWindowEventPump* self = new (ELeave) CWindowEventPump(aWs, aEngine);
        CleanupStack::PushL(self);
        CActiveScheduler::Add(self);
        self->Request();
        CleanupStack::Pop(self);
        return self;
    }

    ~CWindowEventPump()
    {
        Cancel();
    }

private:
    CWindowEventPump(RWsSession& aWs, CRuntimeEngine& aEngine)
        : CActive(CActive::EPriorityHigh), iWs(aWs), iEngine(aEngine)
    {
    }

    void Request()
    {
        iWs.EventReady(&iStatus);
        SetActive();
    }

    void RunL()
    {
        if (iStatus.Int() == KErrNone)
        {
            TWsEvent event;
            iWs.GetEvent(event);
            iEngine.HandleWindowEvent(event);
        }
        Request();
    }

    TInt RunError(TInt)
    {
        Request();
        return KErrNone;
    }

    void DoCancel()
    {
        iWs.EventReadyCancel();
    }

private:
    RWsSession& iWs;
    CRuntimeEngine& iEngine;
};

class CRawDirectScreenAccess : public CBase,
                               public MDirectScreenAccess
{
public:
    static CRawDirectScreenAccess* NewL(RWsSession& aWs,
                                        CWsScreenDevice& aScreenDevice,
                                        RWindowBase& aWindow)
    {
        CRawDirectScreenAccess* self = new (ELeave) CRawDirectScreenAccess;
        CleanupStack::PushL(self);
        self->iAccess = CDirectScreenAccess::NewL(aWs,
                                                  aScreenDevice,
                                                  aWindow,
                                                  *self);
        self->StartL();
        CleanupStack::Pop(self);
        return self;
    }

    ~CRawDirectScreenAccess()
    {
        if (iAccess)
            iAccess->Cancel();
        delete iAccess;
        iAccess = NULL;
    }

    void StartL()
    {
        if (!iAccess || iActive)
            return;
        iAccess->StartL();
        iActive = ETrue;
    }

    void Cancel()
    {
        if (iAccess)
            iAccess->Cancel();
        iActive = EFalse;
    }

    TBool IsActive() const
    {
        return iActive;
    }

    TBool ConsumeAbort()
    {
        const TBool aborted = iAborted;
        iAborted = EFalse;
        return aborted;
    }

    CFbsBitGc* Gc()
    {
        return iAccess && iActive ? iAccess->Gc() : NULL;
    }

    CFbsScreenDevice* ScreenDevice()
    {
        return iAccess && iActive ? iAccess->ScreenDevice() : NULL;
    }

private:
    CRawDirectScreenAccess()
        : iAccess(NULL),
          iActive(EFalse),
          iAborted(EFalse)
    {
    }

    void AbortNow(RDirectScreenAccess::TTerminationReasons)
    {
        iActive = EFalse;
        iAborted = ETrue;
    }

    void Restart(RDirectScreenAccess::TTerminationReasons)
    {
        TRAPD(error, iAccess->StartL());
        iActive = error == KErrNone;
    }

private:
    CDirectScreenAccess* iAccess;
    TBool iActive;
    TBool iAborted;
};

CRuntimeEngine* CRuntimeEngine::NewL()
{
    CRuntimeEngine* self = new (ELeave) CRuntimeEngine;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CRuntimeEngine::CRuntimeEngine()
    : iWindowGroup(NULL),
      iWindow(NULL),
      iScreenDevice(NULL),
      iDisplaySurface(NULL),
      iResources(NULL),
      iRuntime(NULL),
      iFrameTimer(NULL),
      iEventPump(NULL),
      iDirectScreen(NULL),
      iPerfOverlay(NULL),
      iGameMenu(NULL),
      iDirectScreenActive(EFalse),
      iNeedsFullRedraw(ETrue),
      iForeground(ETrue),
      iPaused(EFalse),
      iRightSoftHeld(EFalse),
      iHeldPlayerKeys(0),
      iPendingPlayerPulses(0),
      iDynamicPlayerScanCount(0),
      iDirectScreenRetryFrame(0),
      iGameplayPriorityRaised(EFalse),
      iPreviousThreadPriority(EPriorityNormal),
      iFrameError(KErrNone),
      iMemoryPressureLevel(0),
      iNextMemoryTrimFrame(0),
      iNextHeapCompressFrame(0)
{
}

void CRuntimeEngine::ConstructL()
{
    User::LeaveIfError(iFs.Connect());
    User::CommandLine(iGamePath);
    if (iGamePath.Length() <= 0)
        User::Leave(KErrArgument);

    User::LeaveIfError(iWs.Connect(iFs));
    iScreenDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(iScreenDevice->Construct(0));

    const TUint32 handle = 1;
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
    iWs.Flush();
    iForeground = iWs.GetFocusWindowGroup() == iWindowGroup->Identifier();
    EasyRpgMovieInitializeL(iWs, *iScreenDevice, *iWindow);
    iDisplaySurface = CDisplaySurface::NewL(TSize(EasyRpgPlayerConfig::KGameWidth, EasyRpgPlayerConfig::KGameHeight));
    iResources = CZipVfs::OpenL(iFs, iGamePath);
    iRuntime = CPlayerRuntime::NewL(iResources, iGamePath, *iDisplaySurface);
    iEventPump = CWindowEventPump::NewL(iWs, *this);
    iFrameTimer = CFrameTimer::NewL(*this);
    iGameMenu = CGameMenu::NewL(*this, iFs);
    RaisePriority();
}

CRuntimeEngine::~CRuntimeEngine()
{
    RestorePriority();
    if (iFrameTimer)
        iFrameTimer->Stop();
    delete iFrameTimer;
    iFrameTimer = NULL;
    delete iEventPump;
    iEventPump = NULL;
    DestroyDirectScreen();
    EasyRpgMovieShutdown();
    delete iRuntime;
    iRuntime = NULL;
    delete iResources;
    iResources = NULL;
    delete iPerfOverlay;
    iPerfOverlay = NULL;
    delete iGameMenu;
    iGameMenu = NULL;
    delete iDisplaySurface;
    iDisplaySurface = NULL;
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
    iFs.Close();
}

void CRuntimeEngine::Run()
{
    iFrameTimer->Start();
}

void CRuntimeEngine::StartDirectScreenL()
{
    if (!iForeground || iDirectScreenActive || EasyRpgMovieIsActive())
        return;
    if (!iDirectScreen)
        iDirectScreen = CRawDirectScreenAccess::NewL(iWs,
                                                     *iScreenDevice,
                                                     *iWindow);
    else
        iDirectScreen->StartL();
    iDirectScreenActive = ETrue;
    iNeedsFullRedraw = ETrue;
}

void CRuntimeEngine::StopDirectScreen()
{
    if (iDirectScreen && iDirectScreenActive)
        iDirectScreen->Cancel();
    iDirectScreenActive = EFalse;
    iNeedsFullRedraw = ETrue;
}

void CRuntimeEngine::DestroyDirectScreen()
{
    if (iDirectScreen && iDirectScreenActive)
        iDirectScreen->Cancel();
    delete iDirectScreen;
    iDirectScreen = NULL;
    iDirectScreenActive = EFalse;
}

void CRuntimeEngine::PresentDirectL(TBool aForceFull)
{
    if (!iDirectScreenActive || !iDirectScreen || !iDisplaySurface ||
        !iDisplaySurface->HasFrame())
        return;
    if (!iDirectScreen->IsActive())
    {
        iDirectScreenActive = EFalse;
        return;
    }

    CFbsBitGc* gc = iDirectScreen->Gc();
    CFbsScreenDevice* screen = iDirectScreen->ScreenDevice();
    if (!gc || !screen)
        return;

    const TSize viewSize = iScreenDevice->SizeInPixels();
    const TSize bitmapSize = iDisplaySurface->Size();
    TPoint destination(0, 0);
    if (viewSize.iWidth > bitmapSize.iWidth)
        destination.iX = (viewSize.iWidth - bitmapSize.iWidth) / 2;
    if (viewSize.iHeight > bitmapSize.iHeight)
        destination.iY = (viewSize.iHeight - bitmapSize.iHeight) / 2;

    TBool overlayNeedsDraw = iPerfOverlay && iPerfOverlay->IsDirty();
    TRect overlayRect(0, 0, 0, 0);
    TPoint overlayPosition(0, 0);
    if (iPerfOverlay)
    {
        const TSize overlaySize = iPerfOverlay->Bitmap().SizeInPixels();
        overlayPosition = TPoint(2, EasyRpgPlayerConfig::KGameHeight - overlaySize.iHeight - 2);
        overlayRect = TRect(overlayPosition, overlaySize);
    }

    TBool fullScreenUpdate = EFalse;
    TRegionFix<EasyRpgPlayerConfig::KMaximumScreenUpdateRegions> updateRegion;
    const CFbsBitmap& frameBitmap = iDisplaySurface->Bitmap();
    frameBitmap.Begin();

    if (aForceFull || iNeedsFullRedraw)
    {
        gc->BitBlt(destination,
                   &frameBitmap,
                   TRect(TPoint(0, 0), bitmapSize));
        updateRegion.AddRect(TRect(destination, bitmapSize));
        iDisplaySurface->ClearDirtyRegions();
        iNeedsFullRedraw = EFalse;
        overlayNeedsDraw = iPerfOverlay != NULL;
    }
    else
    {
        const TBool presentScroll = iDisplaySurface->HasPresentScroll();
        if (presentScroll)
        {
            gc->BitBlt(destination,
                       &frameBitmap,
                       TRect(TPoint(0, 0), bitmapSize));
            updateRegion.AddRect(TRect(destination, bitmapSize));
            overlayNeedsDraw = iPerfOverlay != NULL;
        }
        else
        {
            const TInt dirtyCount = iDisplaySurface->DirtyRegionCount();
            for (TInt index = 0; index < dirtyCount; ++index)
            {
                const TRect& dirty = iDisplaySurface->DirtyRegion(index);
                gc->BitBlt(destination + dirty.iTl,
                           &frameBitmap,
                           dirty);
                updateRegion.AddRect(TRect(destination + dirty.iTl, dirty.Size()));
                if (iPerfOverlay && RectsOverlap(dirty, overlayRect))
                    overlayNeedsDraw = ETrue;
            }
        }
        iDisplaySurface->ClearDirtyRegions();
    }

    frameBitmap.End();

    if (iPerfOverlay && overlayNeedsDraw)
    {
        iPerfOverlay->Compose(frameBitmap,
                              overlayRect.iTl.iX,
                              overlayRect.iTl.iY);
        const CFbsBitmap& overlayBitmap = iPerfOverlay->Bitmap();
        const TSize overlaySize = overlayBitmap.SizeInPixels();
        overlayBitmap.Begin();
        gc->BitBlt(destination + overlayPosition,
                   &overlayBitmap,
                   TRect(TPoint(0, 0), overlaySize));
        overlayBitmap.End();
        updateRegion.AddRect(TRect(destination + overlayPosition, overlaySize));
        iPerfOverlay->ClearDirty();
    }

    if (iGameMenu && iGameMenu->IsOpen())
    {
        iGameMenu->DrawL(*gc, *screen);
        fullScreenUpdate = ETrue;
    }

    if (fullScreenUpdate)
    {
        screen->Update();
    }
    else if (updateRegion.CheckError())
    {
        // Fixed-region overflow should not happen: DisplaySurface caps frame
        // damage at four rectangles and the overlay contributes at most one.
        // Fall back to a full update rather than risk dropping pixels.
        screen->Update();
    }
    else if (!updateRegion.IsEmpty())
    {
        screen->Update(updateRegion);
    }
}

void CRuntimeEngine::RaisePriority()
{
    if (iGameplayPriorityRaised)
        return;
    RThread thread;
    if (thread.Open(RThread().Id()) != KErrNone)
        return;
    iPreviousThreadPriority = thread.Priority();
    thread.SetPriority(EPriorityMore);
    thread.Close();
    iGameplayPriorityRaised = ETrue;
}

void CRuntimeEngine::RestorePriority()
{
    if (!iGameplayPriorityRaised)
        return;
    RThread thread;
    if (thread.Open(RThread().Id()) == KErrNone)
    {
        thread.SetPriority(iPreviousThreadPriority);
        thread.Close();
    }
    iGameplayPriorityRaised = EFalse;
}

TUint32 CRuntimeEngine::GameMenuFeatures() const
{
    return EGameMenuFeatureSound |
           EGameMenuFeatureVolume |
           EGameMenuFeatureFrameSkip |
           EGameMenuFeatureFps |
           EGameMenuFeatureKeys;
}

void CRuntimeEngine::GameMenuSetSoundEnabledL(TBool aEnabled)
{
    EasyRpgAudioSetEnabled(aEnabled ? 1 : 0);
    if (aEnabled)
        EasyRpgUpstreamPlayerRefreshAudio();
}

void CRuntimeEngine::GameMenuSetVolumeL(TInt aVolume)
{
    EasyRpgAudioSetMasterVolume(aVolume);
}

void CRuntimeEngine::GameMenuSetFrameSkipEnabledL(TBool aEnabled)
{
    if (iFrameTimer)
        iFrameTimer->SetFrameSkipEnabled(aEnabled);
}

void CRuntimeEngine::GameMenuSetFpsEnabledL(TBool aEnabled)
{
    if (aEnabled)
    {
        if (!iPerfOverlay)
            iPerfOverlay = CPerfOverlay::NewL();
    }
    else
    {
        delete iPerfOverlay;
        iPerfOverlay = NULL;
    }
    iNeedsFullRedraw = ETrue;
}

void CRuntimeEngine::GameMenuPauseL(TBool aPaused)
{
    if (!iRuntime || iPaused == aPaused)
        return;
    ClearPlayerInput();
    iPaused = aPaused;
    if (aPaused)
        iRuntime->Suspend();
    else
        iRuntime->Resume();
    iNeedsFullRedraw = ETrue;
    if (!iDirectScreenActive)
        TRAP_IGNORE(StartDirectScreenL());
}

void CRuntimeEngine::GameMenuExitL()
{
    if (iFrameTimer)
        iFrameTimer->Stop();
}

void CRuntimeEngine::GameMenuRequestRedraw()
{
    iNeedsFullRedraw = ETrue;
    if (iDirectScreenActive)
        TRAP_IGNORE(PresentDirectL(ETrue));
}

TUint32 CRuntimeEngine::MapScanKey(const TKeyEvent& aKey) const
{
    switch (aKey.iScanCode)
    {
    case EStdKeyUpArrow:
    case 'W': return CPlayerRuntime::EKeyUp;
    case EStdKeyDownArrow:
    case 'S': return CPlayerRuntime::EKeyDown;
    case EStdKeyLeftArrow:
    case 'A': return CPlayerRuntime::EKeyLeft;
    case EStdKeyRightArrow:
    case 'D': return CPlayerRuntime::EKeyRight;
    case EStdKeyEnter:
    case EStdKeyDevice3:
    case EStdKeySpace:
    case 'Z': return CPlayerRuntime::EKeyDecision;
    case EStdKeyEscape: return CPlayerRuntime::EKeyCancel;
    case EStdKeyLeftShift:
    case EStdKeyRightShift: return CPlayerRuntime::EKeyShift;
    case '0': return CPlayerRuntime::EKeyN0;
    case '1': return CPlayerRuntime::EKeyN1;
    case '2': return CPlayerRuntime::EKeyN2;
    case '3': return CPlayerRuntime::EKeyN3;
    case '4': return CPlayerRuntime::EKeyN4;
    case '5': return CPlayerRuntime::EKeyN5;
    case '6': return CPlayerRuntime::EKeyN6;
    case '7': return CPlayerRuntime::EKeyN7;
    case '8': return CPlayerRuntime::EKeyN8;
    case '9': return CPlayerRuntime::EKeyN9;
    case EStdKeyNkpPlus: return CPlayerRuntime::EKeyPlus;
    case EStdKeyMinus:
    case EStdKeyNkpMinus: return CPlayerRuntime::EKeyMinus;
    case EStdKeyNkpAsterisk: return CPlayerRuntime::EKeyMultiply;
    case EStdKeyForwardSlash:
    case EStdKeyNkpForwardSlash: return CPlayerRuntime::EKeyDivide;
    case EStdKeyFullStop:
    case EStdKeyNkpFullStop: return CPlayerRuntime::EKeyPeriod;
    default: return 0;
    }
}

TUint32 CRuntimeEngine::MapCharacterKey(TInt aCode) const
{
    switch (aCode)
    {
    case EKeyUpArrow:
    case 'W':
    case 'w': return CPlayerRuntime::EKeyUp;
    case EKeyDownArrow:
    case 'S':
    case 's': return CPlayerRuntime::EKeyDown;
    case EKeyLeftArrow:
    case 'A':
    case 'a': return CPlayerRuntime::EKeyLeft;
    case EKeyRightArrow:
    case 'D':
    case 'd': return CPlayerRuntime::EKeyRight;
    case EKeyEnter:
    case EKeyDevice3:
    case 'Z':
    case 'z':
    case ' ': return CPlayerRuntime::EKeyDecision;
    case EKeyEscape: return CPlayerRuntime::EKeyCancel;
    case EKeyLeftShift:
    case EKeyRightShift: return CPlayerRuntime::EKeyShift;
    case '0': return CPlayerRuntime::EKeyN0;
    case '1': return CPlayerRuntime::EKeyN1;
    case '2': return CPlayerRuntime::EKeyN2;
    case '3': return CPlayerRuntime::EKeyN3;
    case '4': return CPlayerRuntime::EKeyN4;
    case '5': return CPlayerRuntime::EKeyN5;
    case '6': return CPlayerRuntime::EKeyN6;
    case '7': return CPlayerRuntime::EKeyN7;
    case '8': return CPlayerRuntime::EKeyN8;
    case '9': return CPlayerRuntime::EKeyN9;
    case '+': return CPlayerRuntime::EKeyPlus;
    case '-': return CPlayerRuntime::EKeyMinus;
    case '*': return CPlayerRuntime::EKeyMultiply;
    case '/': return CPlayerRuntime::EKeyDivide;
    case '.': return CPlayerRuntime::EKeyPeriod;
    default: return 0;
    }
}

TUint32 CRuntimeEngine::FindDynamicScanKey(TInt aScanCode) const
{
    for (TInt index = 0; index < iDynamicPlayerScanCount; ++index)
    {
        if (iDynamicPlayerScanCodes[index] == aScanCode)
            return iDynamicPlayerScanKeys[index];
    }
    return 0;
}

void CRuntimeEngine::RememberDynamicScanKey(TInt aScanCode, TUint32 aPlayerKey)
{
    if (aScanCode == 0)
        return;
    for (TInt index = 0; index < iDynamicPlayerScanCount; ++index)
    {
        if (iDynamicPlayerScanCodes[index] == aScanCode)
        {
            iDynamicPlayerScanKeys[index] = aPlayerKey;
            return;
        }
    }
    if (iDynamicPlayerScanCount >= 16)
        return;
    iDynamicPlayerScanCodes[iDynamicPlayerScanCount] = aScanCode;
    iDynamicPlayerScanKeys[iDynamicPlayerScanCount] = aPlayerKey;
    ++iDynamicPlayerScanCount;
}

void CRuntimeEngine::ForgetDynamicScanKey(TInt aScanCode)
{
    for (TInt index = 0; index < iDynamicPlayerScanCount; ++index)
    {
        if (iDynamicPlayerScanCodes[index] != aScanCode)
            continue;
        for (TInt next = index + 1; next < iDynamicPlayerScanCount; ++next)
        {
            iDynamicPlayerScanCodes[next - 1] = iDynamicPlayerScanCodes[next];
            iDynamicPlayerScanKeys[next - 1] = iDynamicPlayerScanKeys[next];
        }
        --iDynamicPlayerScanCount;
        return;
    }
}

void CRuntimeEngine::ClearPlayerInput()
{
    iHeldPlayerKeys = 0;
    iPendingPlayerPulses = 0;
    iDynamicPlayerScanCount = 0;
    iRightSoftHeld = EFalse;
    if (iRuntime)
        iRuntime->ReleaseAllKeys();
}

void CRuntimeEngine::MaintainMemory(TUint32 aLogicFrame)
{
    TInt freedVfsBytes = 0;
    if ((aLogicFrame % 60u) == 0u)
    {
        const TInt freeRam = EasyRpgPlatformFreeRamBytes();
        const TInt memoryPressure = MemoryPressureLevel(freeRam, iMemoryPressureLevel);
        const TBool pressureChanged = memoryPressure != iMemoryPressureLevel;
        if (pressureChanged)
        {
            iMemoryPressureLevel = memoryPressure;
            EasyRpgAudioSetMemoryPressure(memoryPressure);
            freedVfsBytes = EasyRpgSetVirtualFileMemoryPressure(memoryPressure);
            iNextMemoryTrimFrame = aLogicFrame;
        }
    }

    TInt cacheFreedBytes = 0;
    if (iMemoryPressureLevel > 0 && aLogicFrame >= iNextMemoryTrimFrame)
    {
        cacheFreedBytes = EasyRpgUpstreamPlayerTrimMemory(iMemoryPressureLevel);
        const TUint32 interval = iMemoryPressureLevel >= 2
            ? EasyRpgPlayerConfig::KCriticalMemoryTrimFrames
            : EasyRpgPlayerConfig::KLowMemoryTrimFrames;
        iNextMemoryTrimFrame = aLogicFrame + interval;
    }

    const TInt freedBytes = freedVfsBytes > KMaxTInt - cacheFreedBytes
        ? KMaxTInt
        : freedVfsBytes + cacheFreedBytes;
    if (aLogicFrame < iNextHeapCompressFrame ||
        (iMemoryPressureLevel < 2 && freedBytes < EasyRpgPlayerConfig::KHeapCompressMinFreedBytes))
        return;

    User::Heap().Compress();
    iNextHeapCompressFrame = aLogicFrame + EasyRpgPlayerConfig::KHeapCompressFrames;
}

void CRuntimeEngine::HandleKeyEvent(TInt aType, const TKeyEvent& aKey)
{
    const TBool rightSoft = aKey.iScanCode == EStdKeyDevice1 || aKey.iCode == EKeyDevice1;

    TBool menuConsumed = EFalse;
    if (iGameMenu)
    {
        TRAPD(menuError, menuConsumed = iGameMenu->HandleKeyEventL(aType, aKey));
        if (menuError != KErrNone)
        {
            FrameError(menuError);
            return;
        }
    }
    if (menuConsumed)
        return;

    if (rightSoft)
    {
        if (aType == EEventKeyDown)
        {
            iHeldPlayerKeys |= CPlayerRuntime::EKeyCancel;
            iRuntime->SetKeyState(CPlayerRuntime::EKeyCancel, ETrue);
            iRightSoftHeld = ETrue;
        }
        else if (aType == EEventKeyUp)
        {
            iHeldPlayerKeys &= ~CPlayerRuntime::EKeyCancel;
            iRuntime->SetKeyState(CPlayerRuntime::EKeyCancel, EFalse);
            iRightSoftHeld = EFalse;
        }
        else if (aType == EEventKey &&
                 !iRightSoftHeld &&
                 (iHeldPlayerKeys & CPlayerRuntime::EKeyCancel) == 0)
        {
            iPendingPlayerPulses |= CPlayerRuntime::EKeyCancel;
        }
        return;
    }

    TUint32 playerKey = MapScanKey(aKey);
    if (playerKey == 0)
        playerKey = FindDynamicScanKey(aKey.iScanCode);

    if (aType == EEventKeyDown && playerKey != 0)
    {
        iHeldPlayerKeys |= playerKey;
        iRuntime->SetKeyState(playerKey, ETrue);
        return;
    }

    if (aType == EEventKeyUp && playerKey != 0)
    {
        iHeldPlayerKeys &= ~playerKey;
        iRuntime->SetKeyState(playerKey, EFalse);
        ForgetDynamicScanKey(aKey.iScanCode);
        return;
    }

    if (aType == EEventKey)
    {
        if (playerKey == 0)
            playerKey = MapCharacterKey(aKey.iCode);
        if (playerKey == 0)
            return;
        if (aKey.iScanCode != 0 && MapScanKey(aKey) == 0)
            RememberDynamicScanKey(aKey.iScanCode, playerKey);
        if ((iHeldPlayerKeys & playerKey) == 0)
            iPendingPlayerPulses |= playerKey;
    }
}

void CRuntimeEngine::HandleWindowEvent(const TWsEvent& aEvent)
{
    switch (aEvent.Type())
    {
    case EEventFocusLost:
        iForeground = EFalse;
        if (iGameMenu)
            iGameMenu->ResetInputState();
        ClearPlayerInput();
        iRuntime->Suspend();
        EasyRpgMovieSetForeground(EFalse);
        StopDirectScreen();
        break;
    case EEventFocusGained:
        iForeground = ETrue;
        EasyRpgMovieSetForeground(ETrue);
        if (iPaused)
        {
            TRAP_IGNORE(StartDirectScreenL());
            if (iDirectScreenActive)
                TRAP_IGNORE(PresentDirectL(ETrue));
        }
        else
        {
            iRuntime->Resume();
            TRAP_IGNORE(StartDirectScreenL());
        }
        break;
    case EEventKeyDown:
    case EEventKeyUp:
    case EEventKey:
        if (iForeground && iRuntime)
            HandleKeyEvent(aEvent.Type(), *aEvent.Key());
        break;
    default:
        break;
    }
}

void CRuntimeEngine::SetPlayerKeyMask(TUint32 aMask, TBool aPressed)
{
    const TUint32 keys[] =
    {
        CPlayerRuntime::EKeyUp,
        CPlayerRuntime::EKeyDown,
        CPlayerRuntime::EKeyLeft,
        CPlayerRuntime::EKeyRight,
        CPlayerRuntime::EKeyDecision,
        CPlayerRuntime::EKeyCancel,
        CPlayerRuntime::EKeyShift,
        CPlayerRuntime::EKeyN0,
        CPlayerRuntime::EKeyN1,
        CPlayerRuntime::EKeyN2,
        CPlayerRuntime::EKeyN3,
        CPlayerRuntime::EKeyN4,
        CPlayerRuntime::EKeyN5,
        CPlayerRuntime::EKeyN6,
        CPlayerRuntime::EKeyN7,
        CPlayerRuntime::EKeyN8,
        CPlayerRuntime::EKeyN9,
        CPlayerRuntime::EKeyPlus,
        CPlayerRuntime::EKeyMinus,
        CPlayerRuntime::EKeyMultiply,
        CPlayerRuntime::EKeyDivide,
        CPlayerRuntime::EKeyPeriod
    };
    for (TUint index = 0; index < sizeof(keys) / sizeof(keys[0]); ++index)
    {
        if ((aMask & keys[index]) != 0)
            iRuntime->SetKeyState(keys[index], aPressed);
    }
}

void CRuntimeEngine::FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame)
{
    if (iDirectScreen && iDirectScreen->ConsumeAbort())
    {
        iDirectScreenActive = EFalse;
        ClearPlayerInput();
        iDirectScreenRetryFrame = aLogicFrame + 1;
    }
    if (!iForeground || iPaused || !iRuntime)
        return;

    const TUint32 pulseKeys = iPendingPlayerPulses;
    if (pulseKeys != 0)
        SetPlayerKeyMask(pulseKeys, ETrue);

    const TBool movieWasActive = EasyRpgMovieIsActive();
    iRuntime->TickL(aLogicFrame, aRenderFrame && !movieWasActive);

    if (pulseKeys != 0)
        SetPlayerKeyMask(pulseKeys & ~iHeldPlayerKeys, EFalse);
    iPendingPlayerPulses = 0;

    const TBool movieActive = EasyRpgMovieIsActive();
    if (movieActive)
    {
        if (iDirectScreenActive)
            StopDirectScreen();
    }
    else if (!iDirectScreenActive && aLogicFrame >= iDirectScreenRetryFrame)
    {
        TRAPD(startError, StartDirectScreenL());
        if (startError != KErrNone)
        {
            DestroyDirectScreen();
            ClearPlayerInput();
            iDirectScreenRetryFrame = aLogicFrame + 30;
        }
        else
        {
            iDirectScreenRetryFrame = 0;
        }
    }

    if (EasyRpgMovieConsumeRedraw())
        iNeedsFullRedraw = ETrue;

    if (aRenderFrame && !movieActive && iDisplaySurface->HasFrame())
    {
        if (iPerfOverlay)
            iPerfOverlay->FrameRendered();
        const TBool overlayDirty = iPerfOverlay && iPerfOverlay->IsDirty();
        if (iDisplaySurface->HasDirtyRegions() || iDisplaySurface->HasPresentScroll() ||
            overlayDirty || iNeedsFullRedraw)
        {
            TRAPD(presentError, PresentDirectL(EFalse));
            if (presentError != KErrNone)
            {
                DestroyDirectScreen();
                ClearPlayerInput();
                iDirectScreenRetryFrame = aLogicFrame + 30;
            }
        }
    }

    if (iRuntime->IsRunning())
        MaintainMemory(aLogicFrame);

    if (!iRuntime->IsRunning())
        iFrameTimer->Stop();
}

void CRuntimeEngine::FrameError(TInt aError)
{
    iFrameError = aError;
    if (iFrameTimer)
        iFrameTimer->Stop();
}
