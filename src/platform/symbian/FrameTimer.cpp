#include <hal.h>
#include <hal_data.h>

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
      iLastRenderedLogicFrame(0),
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
    if (iCounterFrequency < KLogicFps)
        User::Leave(KErrNotSupported);
    iFramePeriodCounter = iCounterFrequency / KLogicFps;
    iFramePeriodRemainder = iCounterFrequency % KLogicFps;
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
    if (iRunning && iLogicFrame == 0)
        EasyRpgSymbianLogDebug("frame timer stopped before first tick");
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

    if (iLogicFrame == 0)
        EasyRpgSymbianLogDebug("frame timer dispatched");

    if (iStatus.Int() != KErrNone)
    {
        const TInt error = iStatus.Int();
        Stop();
        iObserver.FrameError(error);
        return;
    }

    UpdateElapsedCounter(User::FastCounter());

    TInt ticksToRun = 0;
    while (iElapsedCounter >= iNextDeadlineCounter && ticksToRun < KMaxCatchUpTicks)
    {
        ++ticksToRun;
        AdvanceDeadline();
    }

    if (ticksToRun <= 0)
    {
        QueueToDeadline();
        return;
    }

    if (ticksToRun == KMaxCatchUpTicks && iElapsedCounter >= iNextDeadlineCounter)
    {
        iNextDeadlineCounter = iElapsedCounter;
        iFrameRemainderAccumulator = 0;
        AdvanceDeadline();
    }

    for (TInt tick = 0; tick < ticksToRun; ++tick)
    {
        ++iLogicFrame;
        if (iLogicFrame == 1)
            EasyRpgSymbianLogDebug("frame timer first tick");

        const TBool finalTick = tick == ticksToRun - 1;
        const TBool renderFrame = finalTick &&
            (iLogicFrame == 1 || iLogicFrame - iLastRenderedLogicFrame >= KRenderDivisor);
        if (renderFrame)
            iLastRenderedLogicFrame = iLogicFrame;

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
