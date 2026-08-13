#include <e32keys.h>
#include <gdi.h>

#include "RgssDisplaySurface.h"
#include "RgssEngine.h"
#include "RgssRuntime.h"
#include "RgssVmBridge.h"

class CRgssWindowEventPump : public CActive
{
public:
    static CRgssWindowEventPump* NewL(RWsSession& aWs, CRgssEngine& aEngine)
    {
        CRgssWindowEventPump* self = new (ELeave) CRgssWindowEventPump(aWs, aEngine);
        CleanupStack::PushL(self);
        CActiveScheduler::Add(self);
        self->Request();
        CleanupStack::Pop(self);
        return self;
    }

    ~CRgssWindowEventPump()
    {
        Cancel();
    }

private:
    CRgssWindowEventPump(RWsSession& aWs, CRgssEngine& aEngine)
        : CActive(CActive::EPriorityHigh),
          iWs(aWs),
          iEngine(aEngine)
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
    CRgssEngine& iEngine;
};

class CRgssDirectScreenAccess : public CBase,
                                public MDirectScreenAccess
{
public:
    static CRgssDirectScreenAccess* NewL(RWsSession& aWs,
                                         CWsScreenDevice& aScreen,
                                         RWindowBase& aWindow)
    {
        CRgssDirectScreenAccess* self = new (ELeave) CRgssDirectScreenAccess;
        CleanupStack::PushL(self);
        self->iAccess = CDirectScreenAccess::NewL(aWs,
                                                  aScreen,
                                                  aWindow,
                                                  *self);
        self->StartL();
        CleanupStack::Pop(self);
        return self;
    }

    ~CRgssDirectScreenAccess()
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

    TBool Active() const
    {
        return iActive;
    }

    CFbsBitGc* Gc()
    {
        return iAccess && iActive ? iAccess->Gc() : NULL;
    }

private:
    CRgssDirectScreenAccess()
        : iAccess(NULL),
          iActive(EFalse)
    {
    }

    void AbortNow(RDirectScreenAccess::TTerminationReasons)
    {
        iActive = EFalse;
    }

    void Restart(RDirectScreenAccess::TTerminationReasons)
    {
        TRAPD(error, iAccess->StartL());
        iActive = error == KErrNone;
    }

private:
    CDirectScreenAccess* iAccess;
    TBool iActive;
};

CRgssEngine* CRgssEngine::NewL()
{
    CRgssEngine* self = new (ELeave) CRgssEngine;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CRgssEngine::CRgssEngine()
    : iScreenDevice(NULL),
      iWindowGroup(NULL),
      iWindow(NULL),
      iRuntime(NULL),
      iFrameTimer(NULL),
      iEventPump(NULL),
      iDirectScreen(NULL),
      iInputWord(0),
      iLogicAccumulator(0),
      iPresentAccumulator(0),
      iFrameError(KErrNone),
      iForeground(ETrue),
      iDirectScreenActive(EFalse)
{
}

void CRgssEngine::ConstructL()
{
    const TInt KGamePathParameterSlot = 1;
    User::LeaveIfError(iFs.Connect());
    if (User::GetDesParameter(KGamePathParameterSlot, iGamePath) != KErrNone)
        User::CommandLine(iGamePath);
    if (iGamePath.Length() == 0)
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
    iRuntime = CRgssRuntime::NewL(iGamePath);
    iEventPump = CRgssWindowEventPump::NewL(iWs, *this);
    iFrameTimer = CFrameTimer::NewL(*this);
}

CRgssEngine::~CRgssEngine()
{
    if (iFrameTimer)
        iFrameTimer->Stop();
    delete iFrameTimer;
    iFrameTimer = NULL;
    delete iEventPump;
    iEventPump = NULL;
    StopDirectScreen();
    delete iDirectScreen;
    iDirectScreen = NULL;
    delete iRuntime;
    iRuntime = NULL;
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

void CRgssEngine::RunL()
{
    iRuntime->StartVmL();
    StartDirectScreenL();
    iFrameTimer->Start();
    if (iFrameError != KErrNone)
        User::Leave(iFrameError);
}

void CRgssEngine::StartDirectScreenL()
{
    if (!iForeground || iDirectScreenActive)
        return;
    if (!iDirectScreen)
        iDirectScreen = CRgssDirectScreenAccess::NewL(iWs,
                                                       *iScreenDevice,
                                                       *iWindow);
    else
        iDirectScreen->StartL();
    iDirectScreenActive = ETrue;
    if (iRuntime)
        iRuntime->Surface().MarkFullDirty();
}

void CRgssEngine::StopDirectScreen()
{
    if (iDirectScreen && iDirectScreenActive)
        iDirectScreen->Cancel();
    iDirectScreenActive = EFalse;
}

void CRgssEngine::PresentL(TBool aForceFull)
{
    if (!iRuntime || !iDirectScreenActive || !iDirectScreen || !iDirectScreen->Active())
        return;

    CFbsBitGc* gc = iDirectScreen->Gc();
    if (!gc)
        return;

    CRgssDisplaySurface& surface = iRuntime->Surface();
    if (aForceFull)
        surface.MarkFullDirty();
    if (surface.DirtyCount() <= 0)
        return;

    const TSize screenSize = iScreenDevice->SizeInPixels();
    const TSize surfaceSize = surface.Size();
    const TPoint offset((screenSize.iWidth - surfaceSize.iWidth) / 2,
                        (screenSize.iHeight - surfaceSize.iHeight) / 2);

    for (TInt index = 0; index < surface.DirtyCount(); ++index)
    {
        const TRect& source = surface.DirtyRect(index);
        gc->BitBlt(offset + source.iTl, &surface.Bitmap(), source);
    }
    iWs.Flush();
    surface.ClearDirty();
}

void CRgssEngine::FrameTickL(TUint32, TBool)
{
    if (!iForeground || !iRuntime)
        return;

    const TInt logicFps = Max(1, Min(iRuntime->LogicFps(), 60));
    iLogicAccumulator += logicFps;
    if (iLogicAccumulator < 60)
        return;

    const TInt presentRate = Min(iRuntime->TargetFps(), logicFps);
    TInt nextPresentAccumulator = iPresentAccumulator + presentRate;
    const TBool compose = nextPresentAccumulator >= logicFps;

    if (!iRuntime->AdvanceVmFrameL(compose))
    {
        iLogicAccumulator = Min(iLogicAccumulator, 60);
        return;
    }

    iLogicAccumulator -= 60;
    iPresentAccumulator = nextPresentAccumulator;
    if (compose)
    {
        iPresentAccumulator -= logicFps;
        PresentL(EFalse);
    }
}

void CRgssEngine::FrameError(TInt aError)
{
    iFrameError = aError;
    if (iFrameTimer)
        iFrameTimer->Stop();
}

TUint32 CRgssEngine::MapScanKey(const TKeyEvent& aKey) const
{
    switch (aKey.iScanCode)
    {
    case EStdKeyUpArrow:
    case 'W': return ERgssInputUp;
    case EStdKeyDownArrow:
    case 'S': return ERgssInputDown;
    case EStdKeyLeftArrow:
    case 'A': return ERgssInputLeft;
    case EStdKeyRightArrow:
    case 'D': return ERgssInputRight;
    case EStdKeyEnter:
    case EStdKeyDevice3:
    case EStdKeySpace:
    case 'Z': return ERgssInputDecision;
    case EStdKeyEscape:
    case 'X': return ERgssInputCancel;
    case EStdKeyLeftShift:
    case EStdKeyRightShift: return ERgssInputShift;
    case 'Q': return ERgssInputL;
    case 'E': return ERgssInputR;
    default: return 0;
    }
}

TUint32 CRgssEngine::MapCharacterKey(TInt aCode) const
{
    switch (aCode)
    {
    case EKeyUpArrow:
    case 'W':
    case 'w': return ERgssInputUp;
    case EKeyDownArrow:
    case 'S':
    case 's': return ERgssInputDown;
    case EKeyLeftArrow:
    case 'A':
    case 'a': return ERgssInputLeft;
    case EKeyRightArrow:
    case 'D':
    case 'd': return ERgssInputRight;
    case EKeyEnter:
    case EKeyDevice3:
    case 'Z':
    case 'z':
    case ' ': return ERgssInputDecision;
    case EKeyEscape:
    case 'X':
    case 'x': return ERgssInputCancel;
    case EKeyLeftShift:
    case EKeyRightShift: return ERgssInputShift;
    case 'Q':
    case 'q': return ERgssInputL;
    case 'E':
    case 'e': return ERgssInputR;
    default: return 0;
    }
}

void CRgssEngine::SetInput(TUint32 aMask, TBool aPressed)
{
    if (aPressed)
        iInputWord |= aMask;
    else
        iInputWord &= ~aMask;
    if (iRuntime)
        iRuntime->SetInputWord(0, iInputWord);
}

void CRgssEngine::ClearInput()
{
    iInputWord = 0;
    if (iRuntime)
        iRuntime->SetInputWord(0, 0);
}

void CRgssEngine::HandleKeyEvent(TInt aType, const TKeyEvent& aKey)
{
    const TBool rightSoft = aKey.iScanCode == EStdKeyDevice1 || aKey.iCode == EKeyDevice1;
    TUint32 key = rightSoft ? ERgssInputCancel : MapScanKey(aKey);
    if (key == 0)
        key = MapCharacterKey(aKey.iCode);
    if (key == 0)
        return;

    if (aType == EEventKeyDown)
        SetInput(key, ETrue);
    else if (aType == EEventKeyUp)
        SetInput(key, EFalse);
    else if (aType == EEventKey && aKey.iRepeats == 0)
    {
        SetInput(key, ETrue);
        SetInput(key, EFalse);
    }
}

void CRgssEngine::HandleWindowEvent(const TWsEvent& aEvent)
{
    switch (aEvent.Type())
    {
    case EEventFocusLost:
        iForeground = EFalse;
        ClearInput();
        StopDirectScreen();
        break;
    case EEventFocusGained:
        iForeground = ETrue;
        TRAP_IGNORE(StartDirectScreenL());
        if (iDirectScreenActive)
            TRAP_IGNORE(PresentL(ETrue));
        break;
    case EEventKeyDown:
    case EEventKeyUp:
    case EEventKey:
        if (iForeground)
            HandleKeyEvent(aEvent.Type(), *aEvent.Key());
        break;
    default:
        break;
    }
}
