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
    : CActive(CActive::EPriorityHigh),
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
      iRunning(EFalse)
{
}

void CFrameTimer::ConstructL()
{
    User::LeaveIfError(iTimer.CreateLocal());
    User::LeaveIfError(HAL::Get(HALData::EFastCounterFrequency, iCounterFrequency));
    TInt counterCountsUp = 0;
    User::LeaveIfError(HAL::Get(HALData::EFastCounterCountsUp, counterCountsUp));
    iCounterCountsUp = counterCountsUp != 0;
    if (iCounterFrequency < KRenderFps)
        User::Leave(KErrNotSupported);
    iFramePeriodCounter = iCounterFrequency / KRenderFps;
    iFramePeriodRemainder = iCounterFrequency % KRenderFps;
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
    iFrameRemainderAccumulator = 0;
    iElapsedCounter = 0;
    iLastCounter = User::FastCounter();
    iNextDeadlineCounter = 0;
    AdvanceDeadline();
    QueueToDeadline();
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

void CFrameTimer::AdvanceDeadline()
{
    iNextDeadlineCounter += iFramePeriodCounter;
    iFrameRemainderAccumulator += iFramePeriodRemainder;
    if (iFrameRemainderAccumulator >= KRenderFps)
    {
        ++iNextDeadlineCounter;
        iFrameRemainderAccumulator -= KRenderFps;
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
        return 1;

    TInt64 microseconds = (aTicks * 1000000 + iCounterFrequency - 1) / iCounterFrequency;
    if (microseconds < 1)
        microseconds = 1;
    if (microseconds > KMaxTimerDelayMicroseconds)
        microseconds = KMaxTimerDelayMicroseconds;
    return static_cast<TInt>(microseconds);
}

void CFrameTimer::QueueToDeadline()
{
    if (!iRunning)
        return;

    iTimer.HighRes(iStatus,
                   TTimeIntervalMicroSeconds32(
                       CounterTicksToMicroseconds(iNextDeadlineCounter - iElapsedCounter)));
    SetActive();
}

void CFrameTimer::RunL()
{
    if (!iRunning)
        return;

    if (iStatus.Int() != KErrNone)
    {
        const TInt error = iStatus.Int();
        Stop();
        iObserver.FrameError(error);
        return;
    }

    UpdateElapsedCounter(User::FastCounter());

    TInt framesToRun = 0;
    while (iElapsedCounter >= iNextDeadlineCounter && framesToRun < KMaxCatchUpFrames)
    {
        ++framesToRun;
        AdvanceDeadline();
    }

    if (framesToRun <= 0)
    {
        QueueToDeadline();
        return;
    }

    if (framesToRun == KMaxCatchUpFrames && iElapsedCounter >= iNextDeadlineCounter)
    {
        iNextDeadlineCounter = iElapsedCounter;
        iFrameRemainderAccumulator = 0;
        AdvanceDeadline();
    }

    for (TInt frame = 0; frame < framesToRun; ++frame)
    {
        for (TInt tick = 0; tick < KLogicTicksPerRender; ++tick)
        {
            ++iLogicFrame;
            const TBool renderFrame = frame == framesToRun - 1 && tick == KLogicTicksPerRender - 1;
            TRAPD(error, iObserver.FrameTickL(iLogicFrame, renderFrame));
            if (error != KErrNone)
            {
                Stop();
                iObserver.FrameError(error);
                return;
            }
            if (!iRunning)
                return;
        }
    }

    UpdateElapsedCounter(User::FastCounter());
    QueueToDeadline();
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
