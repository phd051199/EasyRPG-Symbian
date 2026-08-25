#include <apgtask.h>
#include <apgwgnam.h>
#include <bitdev.h>
#include <e32keys.h>
#include <f32file.h>
#include <gdi.h>
#include <w32std.h>

#include "AudioBridge.h"
#include "AppConfig.h"
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

TRect ShiftedRect(const TRect& aRect, TInt aShiftX, TInt aShiftY)
{
    return TRect(aRect.iTl + TPoint(aShiftX, aShiftY),
                 aRect.iBr + TPoint(aShiftX, aShiftY));
}

TBool ClipRect(TRect& aRect, const TRect& aBounds)
{
    aRect.iTl.iX = Max(aRect.iTl.iX, aBounds.iTl.iX);
    aRect.iTl.iY = Max(aRect.iTl.iY, aBounds.iTl.iY);
    aRect.iBr.iX = Min(aRect.iBr.iX, aBounds.iBr.iX);
    aRect.iBr.iY = Min(aRect.iBr.iY, aBounds.iBr.iY);
    return aRect.iBr.iX > aRect.iTl.iX && aRect.iBr.iY > aRect.iTl.iY;
}

TBool IsShutdownSystemEvent(const TWsEvent& aEvent)
{
    const TInt type = aEvent.Type();
    if (type == EEventUser)
    {
        const TInt systemEvent = *aEvent.Int();
        return systemEvent == EApaSystemEventShutdown ||
               systemEvent == EApaSystemEventSecureShutdown;
    }
    return type == EEventPowerMgmt &&
           *aEvent.Int() == EApaSystemEventSecureShutdown;
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
        CFbsScreenDevice* screen = iAccess->ScreenDevice();
        if (screen)
            screen->SetAutoUpdate(EFalse);
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
        if (error == KErrNone)
        {
            CFbsScreenDevice* screen = iAccess->ScreenDevice();
            if (screen)
                screen->SetAutoUpdate(EFalse);
        }
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
      iReturnToLauncher(ETrue),
      iRightSoftHeld(EFalse),
      iHeldPlayerKeys(0),
      iPendingPlayerPulses(0),
      iDirectScreenRetryFrame(0),
      iGameplayPriorityRaised(EFalse),
      iPreviousThreadPriority(EPriorityNormal),
      iFrameError(KErrNone),
      iMemoryPressureLevel(0)
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

    CApaWindowGroupName* windowGroupName = CApaWindowGroupName::NewLC(iWs);
    windowGroupName->SetAppUid(EasyRpgApp::KLauncherUid);
    windowGroupName->SetCaptionL(EasyRpgApp::KApplicationCaption);
    windowGroupName->SetRespondsToShutdownEvent(ETrue);
    windowGroupName->SetAppReady(ETrue);
    User::LeaveIfError(windowGroupName->SetWindowGroupName(*iWindowGroup));
    CleanupStack::PopAndDestroy(windowGroupName);

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

TBool CRuntimeEngine::ReturnToLauncher() const
{
    return iReturnToLauncher;
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
    TBool fullGameUpdate = EFalse;
    TRegionFix<EasyRpgPlayerConfig::KMaximumScreenUpdateRegions> updateRegion;
    const CFbsBitmap& frameBitmap = iDisplaySurface->Bitmap();
    frameBitmap.Begin();

    const TBool menuOpen = iGameMenu && iGameMenu->IsOpen();
    if (aForceFull || iNeedsFullRedraw || menuOpen)
    {
        gc->BitBlt(destination,
                   &frameBitmap,
                   TRect(TPoint(0, 0), bitmapSize));
        updateRegion.AddRect(TRect(destination, bitmapSize));
        fullGameUpdate = ETrue;
        iDisplaySurface->ClearDirtyRegions();
        iNeedsFullRedraw = EFalse;
        overlayNeedsDraw = iPerfOverlay != NULL;
    }
    else
    {
        const TBool presentScroll = iDisplaySurface->HasPresentScroll();
        if (presentScroll)
        {
            TInt shiftX = 0;
            TInt shiftY = 0;
            iDisplaySurface->PresentScroll(shiftX, shiftY);
            const TInt absShiftX = Abs(shiftX);
            const TInt absShiftY = Abs(shiftY);
            if (absShiftX >= bitmapSize.iWidth || absShiftY >= bitmapSize.iHeight)
            {
                gc->BitBlt(destination,
                           &frameBitmap,
                           TRect(TPoint(0, 0), bitmapSize));
                fullGameUpdate = ETrue;
            }
            else
            {
                TRect copySource(TPoint(0, 0), bitmapSize);
                if (shiftX > 0)
                    copySource.iBr.iX -= shiftX;
                else if (shiftX < 0)
                    copySource.iTl.iX -= shiftX;
                if (shiftY > 0)
                    copySource.iBr.iY -= shiftY;
                else if (shiftY < 0)
                    copySource.iTl.iY -= shiftY;

                gc->CopyRect(TPoint(shiftX, shiftY),
                             TRect(destination + copySource.iTl, copySource.Size()));

                const TInt dirtyCount = iDisplaySurface->DirtyRegionCount();
                for (TInt index = 0; index < dirtyCount; ++index)
                {
                    const TRect& dirty = iDisplaySurface->DirtyRegion(index);
                    gc->BitBlt(destination + dirty.iTl,
                               &frameBitmap,
                               dirty);
                }

                if (iPerfOverlay)
                {
                    // The scroll copy moved the old overlay pixels; restore the
                    // frame underneath and let the tail below redraw the overlay
                    // at its anchored position. Recomposing the small overlay
                    // bitmap each scroll frame is cheaper than clipping the
                    // CopyRect around it.
                    TRect shiftedOverlay = ShiftedRect(overlayRect, shiftX, shiftY);
                    const TRect frameRect(TPoint(0, 0), bitmapSize);
                    if (ClipRect(shiftedOverlay, frameRect))
                        gc->BitBlt(destination + shiftedOverlay.iTl,
                                   &frameBitmap,
                                   shiftedOverlay);
                    overlayNeedsDraw = ETrue;
                }
            }
            updateRegion.AddRect(TRect(destination, bitmapSize));
            fullGameUpdate = ETrue;
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
        if (!fullGameUpdate)
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
        // damage at four rectangles; scroll-copy and overlay add only a few
        // more rectangles under KMaximumScreenUpdateRegions.
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

void CRuntimeEngine::RequestExit(TBool aReturnToLauncher)
{
    iReturnToLauncher = aReturnToLauncher;
    ClearPlayerInput();
    if (iRuntime)
        iRuntime->Suspend();
    EasyRpgMovieSetForeground(EFalse);
    StopDirectScreen();

    if (iFrameTimer && iFrameTimer->Running())
        iFrameTimer->Stop();
    else if (CActiveScheduler::Current())
        CActiveScheduler::Stop();
}

TUint32 CRuntimeEngine::GameMenuFeatures() const
{
    return EGameMenuFeatureSound |
           EGameMenuFeatureVolume |
           EGameMenuFeatureFps |
           EGameMenuFeatureHelp;
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
    RequestExit(ETrue);
}

void CRuntimeEngine::GameMenuRequestRedraw()
{
    iNeedsFullRedraw = ETrue;
    if (iDirectScreenActive)
        TRAP_IGNORE(PresentDirectL(ETrue));
}

namespace
{

// One row per physical mapping: scan code (EStdKey* or char-as-scancode on
// QWERTY devices) and character code resolve to the same player key. Lookup
// falls back from scan code to char code, so key-up/repeat events resolve
// on any device without caching scancode associations.
struct TKeyMapEntry
{
    TInt iScanCode;
    TInt iCharCode;
    TUint32 iPlayerKey;
};

const TKeyMapEntry KKeyMap[] =
{
    { EStdKeyUpArrow, EKeyUpArrow, CPlayerRuntime::EKeyUp },
    { 'W', 'w', CPlayerRuntime::EKeyUp },
    { EStdKeyDownArrow, EKeyDownArrow, CPlayerRuntime::EKeyDown },
    { 'S', 's', CPlayerRuntime::EKeyDown },
    { EStdKeyLeftArrow, EKeyLeftArrow, CPlayerRuntime::EKeyLeft },
    { 'A', 'a', CPlayerRuntime::EKeyLeft },
    { EStdKeyRightArrow, EKeyRightArrow, CPlayerRuntime::EKeyRight },
    { 'D', 'd', CPlayerRuntime::EKeyRight },
    { EStdKeyEnter, EKeyEnter, CPlayerRuntime::EKeyDecision },
    { EStdKeyDevice3, EKeyDevice3, CPlayerRuntime::EKeyDecision },
    { EStdKeySpace, ' ', CPlayerRuntime::EKeyDecision },
    { 'Z', 'z', CPlayerRuntime::EKeyDecision },
    { EStdKeyEscape, EKeyEscape, CPlayerRuntime::EKeyCancel },
    { EStdKeyLeftShift, EKeyLeftShift, CPlayerRuntime::EKeyShift },
    { EStdKeyRightShift, EKeyRightShift, CPlayerRuntime::EKeyShift },
    { '0', '0', CPlayerRuntime::EKeyN0 },
    { '1', '1', CPlayerRuntime::EKeyN1 },
    { '2', '2', CPlayerRuntime::EKeyN2 },
    { '3', '3', CPlayerRuntime::EKeyN3 },
    { '4', '4', CPlayerRuntime::EKeyN4 },
    { '5', '5', CPlayerRuntime::EKeyN5 },
    { '6', '6', CPlayerRuntime::EKeyN6 },
    { '7', '7', CPlayerRuntime::EKeyN7 },
    { '8', '8', CPlayerRuntime::EKeyN8 },
    { '9', '9', CPlayerRuntime::EKeyN9 },
    { EStdKeyNkpPlus, '+', CPlayerRuntime::EKeyPlus },
    { EStdKeyMinus, '-', CPlayerRuntime::EKeyMinus },
    { EStdKeyNkpMinus, 0, CPlayerRuntime::EKeyMinus },
    { EStdKeyNkpAsterisk, '*', CPlayerRuntime::EKeyMultiply },
    { EStdKeyForwardSlash, '/', CPlayerRuntime::EKeyDivide },
    { EStdKeyNkpForwardSlash, 0, CPlayerRuntime::EKeyDivide },
    { EStdKeyFullStop, '.', CPlayerRuntime::EKeyPeriod },
    { EStdKeyNkpFullStop, 0, CPlayerRuntime::EKeyPeriod }
};

const TInt KKeyMapCount = sizeof(KKeyMap) / sizeof(KKeyMap[0]);

} // namespace

TUint32 EasyRpgPlayerKeyForScanCode(TInt aScanCode)
{
    for (TInt index = 0; index < KKeyMapCount; ++index)
        if (KKeyMap[index].iScanCode == aScanCode)
            return KKeyMap[index].iPlayerKey;
    return 0;
}

TUint32 EasyRpgPlayerKeyForCharCode(TInt aCode)
{
    for (TInt index = 0; index < KKeyMapCount; ++index)
        if (KKeyMap[index].iCharCode == aCode)
            return KKeyMap[index].iPlayerKey;
    return 0;
}

void CRuntimeEngine::ClearPlayerInput()
{
    iHeldPlayerKeys = 0;
    iPendingPlayerPulses = 0;
    iRightSoftHeld = EFalse;
    if (iRuntime)
        iRuntime->ReleaseAllKeys();
}

void CRuntimeEngine::MaintainMemory(TUint32 aLogicFrame)
{
    if ((aLogicFrame % 120u) != 0u)
        return;

    const TInt freeRam = EasyRpgPlatformFreeRamBytes();
    const TInt memoryPressure = MemoryPressureLevel(freeRam, iMemoryPressureLevel);
    if (memoryPressure == iMemoryPressureLevel)
        return;

    // Keep steady gameplay free from synchronous cache sweeps and heap
    // compaction. Cache loading already performs bounded pressure-aware
    // eviction, scene changes trim stale bitmaps, and allocation failure uses
    // EasyRpgPlatformPrepareAllocation() as the last-resort compaction path.
    // Repeating TrimUnused()/RHeap::Compress() here caused visible stop-the-
    // world stalls even when average CPU usage was low.
    iMemoryPressureLevel = memoryPressure;
    EasyRpgAudioSetMemoryPressure(memoryPressure);
    EasyRpgSetVirtualFileMemoryPressure(memoryPressure);
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

    TUint32 playerKey = EasyRpgPlayerKeyForScanCode(aKey.iScanCode);
    if (playerKey == 0 && aKey.iCode != 0)
        playerKey = EasyRpgPlayerKeyForCharCode(aKey.iCode);

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
        return;
    }

    if (aType == EEventKey && playerKey != 0)
    {
        if ((iHeldPlayerKeys & playerKey) == 0)
            iPendingPlayerPulses |= playerKey;
    }
}

void CRuntimeEngine::HandleWindowEvent(const TWsEvent& aEvent)
{
    if (IsShutdownSystemEvent(aEvent))
    {
        RequestExit(EFalse);
        return;
    }

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

    if (iRuntime->IsRunning() && iFrameTimer &&
        (aLogicFrame % EasyRpgPlayerConfig::KIdleMaintenanceIntervalFrames) == 0u &&
        iFrameTimer->TimeUntilNextTickMicroseconds() >= EasyRpgPlayerConfig::KIdleMaintenanceMinSlackUs)
    {
        EasyRpgUpstreamPlayerIdleMaintenance(iMemoryPressureLevel);
    }

    if (!iRuntime->IsRunning())
        iFrameTimer->Stop();
}

void CRuntimeEngine::FrameError(TInt aError)
{
    iFrameError = aError;
    if (iFrameTimer)
        iFrameTimer->Stop();
}
