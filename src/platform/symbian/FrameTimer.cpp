#include "easyrpgsymbian/FrameTimer.h"
#include "easyrpgsymbian/SymbianLogBridge.h"

CFrameTimer* CFrameTimer::NewL(MFrameTimerObserver& aObserver)
{
    CFrameTimer* self = new (ELeave) CFrameTimer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CFrameTimer::CFrameTimer(MFrameTimerObserver& aObserver)
    : CActive(CActive::EPriorityHigh),
      iObserver(aObserver),
      iLogicFrame(0),
      iRunning(EFalse)
{
}

void CFrameTimer::ConstructL()
{
    User::LeaveIfError(iTimer.CreateLocal());
    CActiveScheduler::Add(this);
}

CFrameTimer::~CFrameTimer()
{
    Stop();
    iTimer.Close();
}

void CFrameTimer::Start()
{
    if (iRunning)
        return;

    iRunning = ETrue;
    Arm();
}

void CFrameTimer::Stop()
{
    if (iRunning && iLogicFrame == 0)
        EasyRpgSymbianLogDebug("frame timer stopped before first tick");
    iRunning = EFalse;
    Cancel();
}

TBool CFrameTimer::Running() const
{
    return iRunning;
}

void CFrameTimer::Arm()
{
    if (!iRunning || IsActive())
        return;

    iTimer.After(iStatus, TTimeIntervalMicroSeconds32(KLogicTickMicroseconds));
    SetActive();
}

void CFrameTimer::RunL()
{
    if (!iRunning)
        return;

    if (iLogicFrame == 0)
        EasyRpgSymbianLogDebug("frame timer dispatched");

    if (iStatus.Int() != KErrNone)
    {
        const TInt error = iStatus.Int();
        Stop();
        iObserver.FrameError(error);
        return;
    }

    ++iLogicFrame;
    if (iLogicFrame == 1)
        EasyRpgSymbianLogDebug("frame timer first tick");

    Arm();

    const TBool renderFrame = ((iLogicFrame % KRenderDivisor) == 1);
    TRAPD(error, iObserver.FrameTickL(iLogicFrame, renderFrame));
    if (error != KErrNone)
    {
        Stop();
        iObserver.FrameError(error);
    }
}

TInt CFrameTimer::RunError(TInt)
{
    Stop();
    return KErrNone;
}

void CFrameTimer::DoCancel()
{
    iTimer.Cancel();
}
