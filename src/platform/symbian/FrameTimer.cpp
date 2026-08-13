#include <hal.h>
#include <hal_data.h>

#include "FrameTimer.h"

CFrameTimer* CFrameTimer::NewL(MFrameTimerObserver& aObserver)
{
    CFrameTimer* self = new (ELeave) CFrameTimer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

CFrameTimer::CFrameTimer(MFrameTimerObserver& aObserver)
    : CActive(CActive::EPriorityStandard),
      iObserver(aObserver),
      iCounterFrequency(0),
      iCounterCountsUp(ETrue),
      iFramePeriodCounter(0),
      iFramePeriodRemainder(0),
      iFrameRemainderAccumulator(0),
      iLastCounter(0),
      iElapsedCounter(0),
      iNextDeadlineCounter(0),
      iLogicFrame(0),
      iNextRenderLogicFrame(KLogicTicksPerRender),
      iRunning(EFalse),
      iFrameSkipEnabled(ETrue),
      iRenderPending(EFalse)
{
}

void CFrameTimer::ConstructL()
{
    User::LeaveIfError(iTimer.CreateLocal());
    CActiveScheduler::Add(this);
    User::LeaveIfError(HAL::Get(HALData::EFastCounterFrequency, iCounterFrequency));
    TInt counterCountsUp = 0;
    User::LeaveIfError(HAL::Get(HALData::EFastCounterCountsUp, counterCountsUp));
    iCounterCountsUp = counterCountsUp != 0;
    if (iCounterFrequency < KLogicFps)
        User::Leave(KErrNotSupported);
    iFramePeriodCounter = iCounterFrequency / KLogicFps;
    iFramePeriodRemainder = iCounterFrequency % KLogicFps;
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
    iFrameRemainderAccumulator = 0;
    iElapsedCounter = 0;
    iLastCounter = User::FastCounter();
    iNextDeadlineCounter = 0;
    iNextRenderLogicFrame = KLogicTicksPerRender;
    iRenderPending = EFalse;
    AdvanceDeadline();
    ScheduleNext();
    CActiveScheduler::Start();
}

void CFrameTimer::Stop()
{
    if (!iRunning)
        return;
    iRunning = EFalse;
    Cancel();
    CActiveScheduler::Stop();
}

TBool CFrameTimer::Running() const
{
    return iRunning;
}

void CFrameTimer::SetFrameSkipEnabled(TBool aEnabled)
{
    iFrameSkipEnabled = aEnabled;
}

TBool CFrameTimer::FrameSkipEnabled() const
{
    return iFrameSkipEnabled;
}

void CFrameTimer::RequestRender()
{
    iRenderPending = ETrue;
}

void CFrameTimer::AdvanceDeadline()
{
    iNextDeadlineCounter += iFramePeriodCounter;
    iFrameRemainderAccumulator += iFramePeriodRemainder;
    if (iFrameRemainderAccumulator >= KLogicFps)
    {
        ++iNextDeadlineCounter;
        iFrameRemainderAccumulator -= KLogicFps;
    }
}

TUint32 CFrameTimer::CounterDelta(TUint32 aCurrent, TUint32 aPrevious) const
{
    return iCounterCountsUp
        ? static_cast<TUint32>(aCurrent - aPrevious)
        : static_cast<TUint32>(aPrevious - aCurrent);
}

void CFrameTimer::UpdateElapsedCounter(TUint32 aCounter)
{
    iElapsedCounter += CounterDelta(aCounter, iLastCounter);
    iLastCounter = aCounter;
}

TInt CFrameTimer::CounterTicksToMicroseconds(TInt64 aTicks) const
{
    if (aTicks <= 0)
        return 0;
    const TInt64 microseconds =
        (aTicks * 1000000 + iCounterFrequency - 1) / iCounterFrequency;
    return static_cast<TInt>(microseconds > 0 ? microseconds : 1);
}

void CFrameTimer::ScheduleNext()
{
    if (!iRunning || IsActive())
        return;

    UpdateElapsedCounter(User::FastCounter());
    const TInt64 remaining = iNextDeadlineCounter - iElapsedCounter;
    if (remaining <= 0)
    {
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status, KErrNone);
        return;
    }

    TInt delay = CounterTicksToMicroseconds(remaining);
    if (delay < 1)
        delay = 1;
    iTimer.HighRes(iStatus, TTimeIntervalMicroSeconds32(delay));
    SetActive();
}

void CFrameTimer::RunL()
{
    if (!iRunning)
        return;
    if (iStatus.Int() != KErrNone)
    {
        const TInt error = iStatus.Int();
        iObserver.FrameError(error);
        return;
    }

    UpdateElapsedCounter(User::FastCounter());
    if (iElapsedCounter < iNextDeadlineCounter)
    {
        ScheduleNext();
        return;
    }

    AdvanceDeadline();
    ++iLogicFrame;
    const TBool scheduledRender =
        static_cast<TInt32>(iLogicFrame - iNextRenderLogicFrame) >= 0;
    if (scheduledRender)
        iRenderPending = ETrue;

    const TBool stillBehind = iElapsedCounter >= iNextDeadlineCounter;
    const TBool renderFrame = iFrameSkipEnabled
        ? (!stillBehind && iRenderPending)
        : iRenderPending;
    if (renderFrame)
    {
        iRenderPending = EFalse;
        iNextRenderLogicFrame = iLogicFrame + KLogicTicksPerRender;
    }

    TRAPD(error, iObserver.FrameTickL(iLogicFrame, renderFrame));
    if (error != KErrNone)
    {
        iObserver.FrameError(error);
        return;
    }
    if (!iRunning)
        return;

    ScheduleNext();
}

void CFrameTimer::DoCancel()
{
    iTimer.Cancel();
}
