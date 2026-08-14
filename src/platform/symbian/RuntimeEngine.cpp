#include <bitdev.h>
#include <e32keys.h>
#include <f32file.h>
#include <gdi.h>
#include <w32std.h>

#include "AudioBridge.h"
#include "DisplaySurface.h"
#include "MovieBridge.h"
#include "PerfOverlay.h"
#include "PlayerRuntime.h"
#include "RuntimeEngine.h"
#include "UpstreamPlayerBridge.h"
#include "ZipVfs.h"

namespace
{
const TInt KGameWidth = 320;
const TInt KGameHeight = 240;
const TInt KDefaultMasterVolume = 50;
const TInt KVolumeStep = 10;
_LIT(KSettingsDirectory, "C:\\Data\\EasyRPG\\");
_LIT(KSettingsPath, "C:\\Data\\EasyRPG\\settings.ini");
_LIT8(KMasterVolumeSetting, "master_volume=");
_LIT8(KSoundSetting, "sound_enabled=");
_LIT8(KFrameSkipSetting, "frameskip_enabled=");
_LIT8(KFpsSetting, "fps_enabled=");

enum TGameMenuItem
{
    EGameMenuSound = 0,
    EGameMenuVolume,
    EGameMenuFrameSkip,
    EGameMenuFps,
    EGameMenuKeys,
    EGameMenuExit,
    EGameMenuItemCount
};

TInt ClampVolume(TInt aVolume)
{
    if (aVolume < 0)
        return 0;
    if (aVolume > 100)
        return 100;
    return aVolume;
}

struct TRuntimeSettings
{
    TInt iVolume;
    TBool iSoundEnabled;
    TBool iFrameSkipEnabled;
    TBool iFpsEnabled;
};

TBool ReadSettingInt(const TDesC8& aData, const TDesC8& aKey, TInt& aValue)
{
    const TInt keyPosition = aData.Find(aKey);
    if (keyPosition < 0)
        return EFalse;

    TInt position = keyPosition + aKey.Length();
    TInt value = 0;
    TBool hasDigit = EFalse;
    while (position < aData.Length())
    {
        const TUint8 character = aData[position++];
        if (character < '0' || character > '9')
            break;
        hasDigit = ETrue;
        value = value * 10 + character - '0';
    }
    if (!hasDigit)
        return EFalse;
    aValue = value;
    return ETrue;
}

TRuntimeSettings LoadSettings(RFs& aFs)
{
    TRuntimeSettings settings;
    settings.iVolume = KDefaultMasterVolume;
    settings.iSoundEnabled = ETrue;
    settings.iFrameSkipEnabled = ETrue;
    settings.iFpsEnabled = ETrue;

    RFile file;
    if (file.Open(aFs, KSettingsPath, EFileRead | EFileShareReadersOnly) != KErrNone)
        return settings;

    TBuf8<256> data;
    const TInt error = file.Read(data);
    file.Close();
    if (error != KErrNone)
        return settings;

    TInt value = 0;
    const TBool hasVolume = ReadSettingInt(data, KMasterVolumeSetting, value);
    if (hasVolume)
        settings.iVolume = ClampVolume(value);

    if (ReadSettingInt(data, KSoundSetting, value))
        settings.iSoundEnabled = value != 0;
    else if (hasVolume)
        settings.iSoundEnabled = settings.iVolume > 0;

    if (ReadSettingInt(data, KFrameSkipSetting, value))
        settings.iFrameSkipEnabled = value != 0;
    if (ReadSettingInt(data, KFpsSetting, value))
        settings.iFpsEnabled = value != 0;

    if (settings.iVolume <= 0)
        settings.iVolume = KDefaultMasterVolume;
    return settings;
}

void SaveSettings(RFs& aFs,
                  TInt aVolume,
                  TBool aSoundEnabled,
                  TBool aFrameSkipEnabled,
                  TBool aFpsEnabled)
{
    const TInt directoryError = aFs.MkDirAll(KSettingsDirectory);
    if (directoryError != KErrNone && directoryError != KErrAlreadyExists)
        return;

    RFile file;
    if (file.Replace(aFs, KSettingsPath, EFileWrite | EFileShareExclusive) != KErrNone)
        return;
    TBuf8<128> data;
    data.Format(_L8("master_volume=%d\nsound_enabled=%d\nframeskip_enabled=%d\nfps_enabled=%d\n"),
                ClampVolume(aVolume),
                aSoundEnabled ? 1 : 0,
                aFrameSkipEnabled ? 1 : 0,
                aFpsEnabled ? 1 : 0);
    file.Write(data);
    file.Close();
}

TBool RectsOverlap(const TRect& aFirst, const TRect& aSecond)
{
    return aFirst.iTl.iX < aSecond.iBr.iX &&
        aSecond.iTl.iX < aFirst.iBr.iX &&
        aFirst.iTl.iY < aSecond.iBr.iY &&
        aSecond.iTl.iY < aFirst.iBr.iY;
}

const TUint8 KSmallDigits[10][7] =
{
    {14, 17, 19, 21, 25, 17, 14},
    {4, 12, 4, 4, 4, 4, 14},
    {14, 17, 1, 2, 4, 8, 31},
    {30, 1, 1, 14, 1, 1, 30},
    {2, 6, 10, 18, 31, 2, 2},
    {31, 16, 16, 30, 1, 1, 30},
    {14, 16, 16, 30, 17, 17, 14},
    {31, 1, 2, 4, 8, 8, 8},
    {14, 17, 17, 14, 17, 17, 14},
    {14, 17, 17, 15, 1, 1, 14}
};

const TUint8 KSmallLetters[26][7] =
{
    {14, 17, 17, 31, 17, 17, 17},
    {30, 17, 17, 30, 17, 17, 30},
    {14, 17, 16, 16, 16, 17, 14},
    {30, 17, 17, 17, 17, 17, 30},
    {31, 16, 16, 30, 16, 16, 31},
    {31, 16, 16, 30, 16, 16, 16},
    {14, 17, 16, 23, 17, 17, 15},
    {17, 17, 17, 31, 17, 17, 17},
    {14, 4, 4, 4, 4, 4, 14},
    {7, 2, 2, 2, 18, 18, 12},
    {17, 18, 20, 24, 20, 18, 17},
    {16, 16, 16, 16, 16, 16, 31},
    {17, 27, 21, 21, 17, 17, 17},
    {17, 25, 21, 19, 17, 17, 17},
    {14, 17, 17, 17, 17, 17, 14},
    {30, 17, 17, 30, 16, 16, 16},
    {14, 17, 17, 17, 21, 18, 13},
    {30, 17, 17, 30, 20, 18, 17},
    {15, 16, 16, 14, 1, 1, 30},
    {31, 4, 4, 4, 4, 4, 4},
    {17, 17, 17, 17, 17, 17, 14},
    {17, 17, 17, 17, 17, 10, 4},
    {17, 17, 17, 21, 21, 21, 10},
    {17, 17, 10, 4, 10, 17, 17},
    {17, 17, 10, 4, 4, 4, 4},
    {31, 1, 2, 4, 8, 16, 31}
};

TUint8 SmallGlyphRow(TUint8 aCharacter, TInt aRow)
{
    if (aRow < 0 || aRow >= 7)
        return 0;
    if (aCharacter >= 'a' && aCharacter <= 'z')
        aCharacter = static_cast<TUint8>(aCharacter - 'a' + 'A');
    if (aCharacter >= '0' && aCharacter <= '9')
        return KSmallDigits[aCharacter - '0'][aRow];
    if (aCharacter >= 'A' && aCharacter <= 'Z')
        return KSmallLetters[aCharacter - 'A'][aRow];

    static const TUint8 slash[7] = {1, 2, 2, 4, 8, 8, 16};
    static const TUint8 plus[7] = {0, 4, 4, 31, 4, 4, 0};
    static const TUint8 minus[7] = {0, 0, 0, 31, 0, 0, 0};
    static const TUint8 multiply[7] = {0, 17, 10, 4, 10, 17, 0};
    static const TUint8 period[7] = {0, 0, 0, 0, 0, 12, 12};
    static const TUint8 colon[7] = {0, 12, 12, 0, 12, 12, 0};
    static const TUint8 percent[7] = {17, 2, 4, 4, 8, 16, 17};

    switch (aCharacter)
    {
    case '/': return slash[aRow];
    case '+': return plus[aRow];
    case '-': return minus[aRow];
    case '*': return multiply[aRow];
    case '.': return period[aRow];
    case ':': return colon[aRow];
    case '%': return percent[aRow];
    default: return 0;
    }
}

void DrawSmallBitmapText(CFbsBitGc& aGc,
                         TInt aX,
                         TInt aY,
                         const TDesC8& aText)
{
    for (TInt index = 0; index < aText.Length(); ++index)
    {
        for (TInt row = 0; row < 7; ++row)
        {
            const TUint8 bits = SmallGlyphRow(aText[index], row);
            for (TInt column = 0; column < 5; ++column)
            {
                if ((bits & (1u << (4 - column))) != 0)
                    aGc.DrawRect(TRect(TPoint(aX + index * 6 + column, aY + row), TSize(1, 1)));
            }
        }
    }
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
      iDirectScreenActive(EFalse),
      iNeedsFullRedraw(ETrue),
      iForeground(ETrue),
      iPaused(EFalse),
      iKeyMapVisible(EFalse),
      iMenuSelection(EGameMenuSound),
      iLeftSoftHeld(EFalse),
      iRightSoftHeld(EFalse),
      iHeldPlayerKeys(0),
      iPendingPlayerPulses(0),
      iDynamicPlayerScanCount(0),
      iDirectScreenRetryFrame(0),
      iMasterVolume(KDefaultMasterVolume),
      iAudibleVolume(KDefaultMasterVolume),
      iVolumeOverlayTicks(0),
      iVolumeOverlayErasePending(EFalse),
      iGameplayPriorityRaised(EFalse),
      iPreviousThreadPriority(EPriorityNormal),
      iFrameError(KErrNone)
{
}

void CRuntimeEngine::ConstructL()
{
    const TInt KGamePathParameterSlot = 1;
    User::LeaveIfError(iFs.Connect());
    if (User::GetDesParameter(KGamePathParameterSlot, iGamePath) != KErrNone)
        User::CommandLine(iGamePath);
    if (iGamePath.Length() <= 0)
        User::Leave(KErrArgument);
    const TRuntimeSettings settings = LoadSettings(iFs);
    iAudibleVolume = settings.iVolume;
    iMasterVolume = settings.iSoundEnabled ? settings.iVolume : 0;
    EasyRpgAudioSetEnabled(settings.iSoundEnabled ? 1 : 0);
    EasyRpgAudioSetMasterVolume(iMasterVolume);

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
    iDisplaySurface = CDisplaySurface::NewL(TSize(KGameWidth, KGameHeight));
    iResources = CZipVfs::OpenL(iFs, iGamePath);
    iRuntime = CPlayerRuntime::NewL(iResources, iGamePath, *iDisplaySurface);
    if (settings.iFpsEnabled)
        iPerfOverlay = CPerfOverlay::NewL();
    iEventPump = CWindowEventPump::NewL(iWs, *this);
    iFrameTimer = CFrameTimer::NewL(*this);
    iFrameTimer->SetFrameSkipEnabled(settings.iFrameSkipEnabled);
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
        overlayPosition = TPoint(2, KGameHeight - overlaySize.iHeight - 2);
        overlayRect = TRect(overlayPosition, overlaySize);
    }

    const CFbsBitmap& frameBitmap = iDisplaySurface->Bitmap();
    frameBitmap.Begin();

    if (aForceFull || iNeedsFullRedraw)
    {
        gc->BitBlt(destination,
                   &frameBitmap,
                   TRect(TPoint(0, 0), bitmapSize));
        iDisplaySurface->ClearDirtyRegions();
        iNeedsFullRedraw = EFalse;
        overlayNeedsDraw = iPerfOverlay != NULL;
    }
    else
    {
        TInt scrollX = 0;
        TInt scrollY = 0;
        const TBool presentScroll = iDisplaySurface->HasPresentScroll();
        if (presentScroll)
        {
            iDisplaySurface->PresentScroll(scrollX, scrollY);
            const TInt absX = Abs(scrollX);
            const TInt absY = Abs(scrollY);
            const TInt copyWidth = bitmapSize.iWidth - absX;
            const TInt copyHeight = bitmapSize.iHeight - absY;
            if (copyWidth > 0 && copyHeight > 0)
            {
                const TInt sourceX = scrollX < 0 ? -scrollX : 0;
                const TInt sourceY = scrollY < 0 ? -scrollY : 0;
                const TRect sourceRect(destination + TPoint(sourceX, sourceY),
                                       TSize(copyWidth, copyHeight));
                gc->CopyRect(TPoint(scrollX, scrollY), sourceRect);
            }
        }

        const TInt dirtyCount = iDisplaySurface->DirtyRegionCount();
        for (TInt index = 0; index < dirtyCount; ++index)
        {
            const TRect& dirty = iDisplaySurface->DirtyRegion(index);
            gc->BitBlt(destination + dirty.iTl,
                       &frameBitmap,
                       dirty);
            if (iPerfOverlay && RectsOverlap(dirty, overlayRect))
                overlayNeedsDraw = ETrue;
        }

        if (presentScroll && iPerfOverlay)
        {
            TRect shiftedOverlay(overlayRect);
            shiftedOverlay.Move(scrollX, scrollY);
            shiftedOverlay.Intersection(TRect(TPoint(0, 0), bitmapSize));
            if (!shiftedOverlay.IsEmpty())
            {
                gc->BitBlt(destination + shiftedOverlay.iTl,
                           &frameBitmap,
                           shiftedOverlay);
            }
            overlayNeedsDraw = ETrue;
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
        iPerfOverlay->ClearDirty();
    }

    if (iPaused)
    {
        if (iKeyMapVisible)
            DrawKeyMapOverlayL(*gc, *screen);
        else
            DrawMenuOverlayL(*gc, *screen);
    }

    screen->Update();
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

void CRuntimeEngine::OpenMenu()
{
    if (iPaused || !iRuntime)
        return;
    const TBool leftSoftHeld = iLeftSoftHeld;
    ClearPlayerInput();
    iLeftSoftHeld = leftSoftHeld;
    iPaused = ETrue;
    iKeyMapVisible = EFalse;
    iRuntime->Suspend();
    iNeedsFullRedraw = ETrue;
    if (!iDirectScreenActive)
        TRAP_IGNORE(StartDirectScreenL());
    if (iDirectScreenActive)
        TRAP_IGNORE(PresentDirectL(ETrue));
}

void CRuntimeEngine::CloseMenu()
{
    if (!iPaused || !iRuntime)
        return;
    const TBool rightSoftHeld = iRightSoftHeld;
    iKeyMapVisible = EFalse;
    iPaused = EFalse;
    ClearPlayerInput();
    iRightSoftHeld = rightSoftHeld;
    iRuntime->Resume();
    iNeedsFullRedraw = ETrue;
    if (!iDirectScreenActive)
        TRAP_IGNORE(StartDirectScreenL());
    if (iDirectScreenActive)
        TRAP_IGNORE(PresentDirectL(ETrue));
}

void CRuntimeEngine::MoveMenuSelection(TInt aDelta)
{
    if (!iPaused || iKeyMapVisible)
        return;
    iMenuSelection += aDelta;
    while (iMenuSelection < 0)
        iMenuSelection += EGameMenuItemCount;
    while (iMenuSelection >= EGameMenuItemCount)
        iMenuSelection -= EGameMenuItemCount;
    if (iDirectScreenActive)
        TRAP_IGNORE(PresentDirectL(ETrue));
}

void CRuntimeEngine::ActivateMenuItemL()
{
    if (!iPaused || iKeyMapVisible)
        return;
    switch (iMenuSelection)
    {
    case EGameMenuSound:
        ToggleSound();
        break;
    case EGameMenuVolume:
        AdjustVolume(KVolumeStep);
        break;
    case EGameMenuFrameSkip:
        ToggleFrameSkip();
        break;
    case EGameMenuFps:
        TogglePerfOverlay();
        break;
    case EGameMenuKeys:
        iKeyMapVisible = ETrue;
        break;
    case EGameMenuExit:
        if (iFrameTimer)
            iFrameTimer->Stop();
        return;
    default:
        break;
    }
    if (iDirectScreenActive)
        PresentDirectL(ETrue);
}

void CRuntimeEngine::ToggleSound()
{
    if (iMasterVolume > 0)
    {
        iAudibleVolume = iMasterVolume;
        iMasterVolume = 0;
        EasyRpgAudioSetMasterVolume(0);
        EasyRpgAudioSetEnabled(0);
    }
    else
    {
        if (iAudibleVolume <= 0)
            iAudibleVolume = KDefaultMasterVolume;
        iMasterVolume = iAudibleVolume;
        EasyRpgAudioSetEnabled(1);
        EasyRpgAudioSetMasterVolume(iMasterVolume);
        EasyRpgUpstreamPlayerRefreshAudio();
    }
    PersistSettings();
}

void CRuntimeEngine::AdjustVolume(TInt aDelta)
{
    if (iMasterVolume > 0)
    {
        iMasterVolume = ClampVolume(iMasterVolume + aDelta);
        iAudibleVolume = iMasterVolume;
        EasyRpgAudioSetMasterVolume(iMasterVolume);
    }
    else
    {
        iAudibleVolume = ClampVolume(iAudibleVolume + aDelta);
    }
    PersistSettings();
}

void CRuntimeEngine::ToggleFrameSkip()
{
    if (!iFrameTimer)
        return;
    iFrameTimer->SetFrameSkipEnabled(!iFrameTimer->FrameSkipEnabled());
    PersistSettings();
}

void CRuntimeEngine::TogglePerfOverlay()
{
    if (iPerfOverlay)
    {
        delete iPerfOverlay;
        iPerfOverlay = NULL;
    }
    else
    {
        TRAP_IGNORE(iPerfOverlay = CPerfOverlay::NewL());
    }
    iNeedsFullRedraw = ETrue;
    PersistSettings();
}

void CRuntimeEngine::PersistSettings()
{
    const TInt volume = iAudibleVolume > 0 ? iAudibleVolume : KDefaultMasterVolume;
    const TBool frameSkipEnabled = iFrameTimer ? iFrameTimer->FrameSkipEnabled() : ETrue;
    SaveSettings(iFs,
                 volume,
                 iMasterVolume > 0,
                 frameSkipEnabled,
                 iPerfOverlay != NULL);
}

void CRuntimeEngine::DrawMenuOverlayL(CFbsBitGc& aGc, CFbsScreenDevice& aScreen)
{
    const TSize screenSize = aScreen.SizeInPixels();
    const TInt panelWidth = Min(156, Max(1, screenSize.iWidth - 12));
    const TInt panelHeight = Min(116, Max(1, screenSize.iHeight - 12));
    const TPoint origin(0, screenSize.iHeight - panelHeight);
    const TRect panel(origin, TSize(panelWidth, panelHeight));

    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    aGc.DrawRect(panel);

    aGc.SetPenStyle(CGraphicsContext::ENullPen);
    aGc.SetBrushColor(TRgb(255, 255, 255));
    DrawSmallBitmapText(aGc, panel.iTl.iX + 7, panel.iTl.iY + 6, _L8("MENU"));

    const TInt rowX = panel.iTl.iX + 5;
    const TInt rowWidth = panelWidth - 10;
    const TInt firstY = panel.iTl.iY + 19;
    const TInt rowStep = 11;
    for (TInt item = 0; item < EGameMenuItemCount; ++item)
    {
        TBuf8<28> text;
        switch (item)
        {
        case EGameMenuSound:
            text.Copy(iMasterVolume > 0 ? _L8("SOUND ON") : _L8("SOUND OFF"));
            break;
        case EGameMenuVolume:
            text.Format(_L8("VOLUME %d%%"), iMasterVolume);
            break;
        case EGameMenuFrameSkip:
            text.Copy(iFrameTimer && iFrameTimer->FrameSkipEnabled()
                ? _L8("FRAMESKIP AUTO")
                : _L8("FRAMESKIP OFF"));
            break;
        case EGameMenuFps:
            text.Copy(iPerfOverlay ? _L8("FPS ON") : _L8("FPS OFF"));
            break;
        case EGameMenuKeys:
            text.Copy(_L8("KEYS"));
            break;
        case EGameMenuExit:
            text.Copy(_L8("EXIT"));
            break;
        default:
            text.Zero();
            break;
        }

        const TInt y = firstY + item * rowStep;
        if (item == iMenuSelection)
        {
            aGc.SetBrushColor(TRgb(255, 255, 255));
            aGc.DrawRect(TRect(TPoint(rowX, y - 2), TSize(rowWidth, 10)));
            aGc.SetBrushColor(TRgb(0, 0, 0));
            DrawSmallBitmapText(aGc, rowX + 4, y, text);
            aGc.SetBrushColor(TRgb(255, 255, 255));
        }
        else
        {
            DrawSmallBitmapText(aGc, rowX + 4, y, text);
        }
    }

    DrawSmallBitmapText(aGc,
                        panel.iTl.iX + 7,
                        panel.iBr.iY - 10,
                        _L8("SELECT"));
    DrawSmallBitmapText(aGc,
                        panel.iBr.iX - 31,
                        panel.iBr.iY - 10,
                        _L8("BACK"));
}

void CRuntimeEngine::DrawKeyMapOverlayL(CFbsBitGc& aGc, CFbsScreenDevice& aScreen)
{
    CFont* font = NULL;
    User::LeaveIfError(aScreen.GetNearestFontInPixels(font, TFontSpec(_L("Series 60 Sans"), 11)));
    if (!font)
        User::Leave(KErrNotFound);

    const TSize screenSize = aScreen.SizeInPixels();
    const TInt panelWidth = Min(304, Max(1, screenSize.iWidth - 12));
    const TInt panelHeight = Min(156, Max(1, screenSize.iHeight - 12));
    const TPoint origin((screenSize.iWidth - panelWidth) / 2,
                        (screenSize.iHeight - panelHeight) / 2);
    const TRect panel(origin, TSize(panelWidth, panelHeight));

    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenColor(TRgb(255, 255, 255));
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(TRgb(0, 0, 0));
    aGc.DrawRect(panel);
    aGc.UseFont(font);
    aGc.SetBrushStyle(CGraphicsContext::ENullBrush);
    aGc.SetPenColor(TRgb(255, 255, 255));

    const TInt leftX = panel.iTl.iX + 8;
    const TInt rightX = panel.iTl.iX + panelWidth / 2 + 4;
    TInt leftY = panel.iTl.iY + 16;
    TInt rightY = leftY;
    const TInt step = 18;
    aGc.DrawText(_L("UP/W -> UP"), TPoint(leftX, leftY));
    leftY += step;
    aGc.DrawText(_L("DOWN/S -> DOWN"), TPoint(leftX, leftY));
    leftY += step;
    aGc.DrawText(_L("LEFT/A -> LEFT"), TPoint(leftX, leftY));
    leftY += step;
    aGc.DrawText(_L("RIGHT/D -> RIGHT"), TPoint(leftX, leftY));
    leftY += step;
    aGc.DrawText(_L("CENTER/Z -> OK"), TPoint(leftX, leftY));

    aGc.DrawText(_L("R SOFT -> BACK"), TPoint(rightX, rightY));
    rightY += step;
    aGc.DrawText(_L("SHIFT -> SHIFT"), TPoint(rightX, rightY));
    rightY += step;
    aGc.DrawText(_L("0-9 -> NUMBER"), TPoint(rightX, rightY));
    rightY += step;
    aGc.DrawText(_L("+-*/. -> OPERATOR"), TPoint(rightX, rightY));

    aGc.DrawText(_L("CENTER/BACK -> MENU"),
                 TPoint(panel.iTl.iX + 8, panel.iBr.iY - 8));
    aGc.DiscardFont();
    aScreen.ReleaseFont(font);
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
    iLeftSoftHeld = EFalse;
    iRightSoftHeld = EFalse;
    if (iRuntime)
        iRuntime->ReleaseAllKeys();
}

void CRuntimeEngine::HandleKeyEvent(TInt aType, const TKeyEvent& aKey)
{
    const TBool leftSoft = aKey.iScanCode == EStdKeyDevice0 || aKey.iCode == EKeyDevice0;
    const TBool rightSoft = aKey.iScanCode == EStdKeyDevice1 || aKey.iCode == EKeyDevice1;

    if (leftSoft)
    {
        TBool trigger = EFalse;
        if (aType == EEventKeyDown)
        {
            trigger = !iLeftSoftHeld;
            iLeftSoftHeld = ETrue;
        }
        else if (aType == EEventKey)
        {
            trigger = !iLeftSoftHeld && aKey.iRepeats == 0;
            iLeftSoftHeld = EFalse;
        }
        else if (aType == EEventKeyUp)
        {
            iLeftSoftHeld = EFalse;
        }
        if (trigger)
        {
            if (!iPaused)
                OpenMenu();
            else if (!iKeyMapVisible)
                TRAP_IGNORE(ActivateMenuItemL());
        }
        return;
    }

    if (rightSoft)
    {
        if (iPaused)
        {
            TBool trigger = EFalse;
            if (aType == EEventKeyDown)
            {
                trigger = !iRightSoftHeld;
                iRightSoftHeld = ETrue;
            }
            else if (aType == EEventKey)
            {
                trigger = !iRightSoftHeld && aKey.iRepeats == 0;
                iRightSoftHeld = EFalse;
            }
            else if (aType == EEventKeyUp)
            {
                iRightSoftHeld = EFalse;
            }
            if (trigger)
            {
                if (iKeyMapVisible)
                {
                    iKeyMapVisible = EFalse;
                    if (iDirectScreenActive)
                        TRAP_IGNORE(PresentDirectL(ETrue));
                }
                else
                {
                    CloseMenu();
                }
            }
            return;
        }

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

    if (iPaused)
    {
        TUint32 menuKey = MapScanKey(aKey);
        if (menuKey == 0)
            menuKey = MapCharacterKey(aKey.iCode);
        const TBool menuNavigationKey =
            menuKey == CPlayerRuntime::EKeyUp ||
            menuKey == CPlayerRuntime::EKeyDown ||
            menuKey == CPlayerRuntime::EKeyLeft ||
            menuKey == CPlayerRuntime::EKeyRight ||
            menuKey == CPlayerRuntime::EKeyDecision ||
            menuKey == CPlayerRuntime::EKeyCancel;
        if (!menuNavigationKey)
            return;

        TBool trigger = EFalse;
        if (aType == EEventKeyDown)
        {
            trigger = (iHeldPlayerKeys & menuKey) == 0;
            iHeldPlayerKeys |= menuKey;
        }
        else if (aType == EEventKeyUp)
        {
            iHeldPlayerKeys &= ~menuKey;
        }
        else if (aType == EEventKey)
        {
            trigger = (iHeldPlayerKeys & menuKey) == 0 || aKey.iRepeats > 0;
        }
        if (!trigger)
            return;

        if (iKeyMapVisible)
        {
            if (menuKey == CPlayerRuntime::EKeyDecision ||
                menuKey == CPlayerRuntime::EKeyCancel)
            {
                iKeyMapVisible = EFalse;
                if (iDirectScreenActive)
                    TRAP_IGNORE(PresentDirectL(ETrue));
            }
            return;
        }

        if (menuKey == CPlayerRuntime::EKeyUp)
            MoveMenuSelection(-1);
        else if (menuKey == CPlayerRuntime::EKeyDown)
            MoveMenuSelection(1);
        else if (menuKey == CPlayerRuntime::EKeyLeft &&
                 iMenuSelection == EGameMenuVolume)
        {
            AdjustVolume(-KVolumeStep);
            if (iDirectScreenActive)
                TRAP_IGNORE(PresentDirectL(ETrue));
        }
        else if (menuKey == CPlayerRuntime::EKeyRight &&
                 iMenuSelection == EGameMenuVolume)
        {
            AdjustVolume(KVolumeStep);
            if (iDirectScreenActive)
                TRAP_IGNORE(PresentDirectL(ETrue));
        }
        else if (menuKey == CPlayerRuntime::EKeyDecision)
        {
            TRAP_IGNORE(ActivateMenuItemL());
        }
        else if (menuKey == CPlayerRuntime::EKeyCancel)
        {
            CloseMenu();
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
        if (iDisplaySurface->HasDirtyRegions() || overlayDirty || iNeedsFullRedraw)
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

    if (!iRuntime->IsRunning())
        iFrameTimer->Stop();
}

void CRuntimeEngine::FrameError(TInt aError)
{
    iFrameError = aError;
    if (iFrameTimer)
        iFrameTimer->Stop();
}
