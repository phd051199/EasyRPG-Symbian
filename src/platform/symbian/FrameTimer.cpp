#include "easyrpgsymbian/FrameTimer.h"

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

    iLogicFrame = 0;
    iRunning = ETrue;
    Arm();
}

void CFrameTimer::Stop()
{
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

    iTimer.HighRes(iStatus, TTimeIntervalMicroSeconds32(KLogicTickMicroseconds));
    SetActive();
}

void CFrameTimer::RunL()
{
    if (!iRunning)
        return;

    if (iStatus.Int() != KErrNone)
    {
        Stop();
        return;
    }

    ++iLogicFrame;

    // Arm before running game work. If a frame takes close to one logic tick,
    // the timer can complete while the frame is being processed and the next
    // RunL is dispatched immediately after returning instead of accumulating
    // an extra frame of drift.
    Arm();

    const TBool renderFrame = ((iLogicFrame % KRenderDivisor) == 0);
    TRAPD(error, iObserver.FrameTickL(iLogicFrame, renderFrame));
    if (error != KErrNone)
        Stop();
}

TInt CFrameTimer::RunError(TInt /*aError*/)
{
    Stop();
    return KErrNone;
}

void CFrameTimer::DoCancel()
{
    iTimer.Cancel();
}
