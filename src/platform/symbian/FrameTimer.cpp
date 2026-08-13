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
    : iObserver(aObserver),
      iCounterFrequency(0),
      iCounterCountsUp(ETrue),
      iFramePeriodCounter(0),
      iFramePeriodRemainder(0),
      iFrameRemainderAccumulator(0),
      iLastCounter(0),
      iElapsedCounter(0),
      iNextDeadlineCounter(0),
      iLogicFrame(0),
      iRunning(EFalse),
      iFrameSkipEnabled(ETrue)
{
}

void CFrameTimer::ConstructL()
{
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
    RunLoop();
}

void CFrameTimer::Stop()
{
    iRunning = EFalse;
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

    TInt64 microseconds = (aTicks * 1000000) / iCounterFrequency;
    if (microseconds <= KSleepGuardMicroseconds)
        return 0;
    microseconds -= KSleepGuardMicroseconds;
    if (microseconds > KMaxSleepMicroseconds)
        microseconds = KMaxSleepMicroseconds;
    return static_cast<TInt>(microseconds);
}

TBool CFrameTimer::PumpReady()
{
    TBool ranAny = EFalse;
    for (TInt count = 0; count < KMaxReadyCallbacks && iRunning; ++count)
    {
        TInt error = KErrNone;
        const TBool ran = CActiveScheduler::RunIfReady(error, CActive::EPriorityIdle);
        if (error != KErrNone)
        {
            iRunning = EFalse;
            iObserver.FrameError(error);
            return EFalse;
        }
        if (!ran)
            break;
        ranAny = ETrue;
    }
    return ranAny;
}

void CFrameTimer::RunLoop()
{
    while (iRunning)
    {
        UpdateElapsedCounter(User::FastCounter());
        if (iElapsedCounter < iNextDeadlineCounter)
        {
            if (PumpReady())
                continue;
            if (!iRunning)
                return;

            UpdateElapsedCounter(User::FastCounter());
            const TInt delay = CounterTicksToMicroseconds(
                iNextDeadlineCounter - iElapsedCounter);
            if (delay > 0)
                User::AfterHighRes(TTimeIntervalMicroSeconds32(delay));
            continue;
        }

        TInt dueTicks = 0;
        while (iElapsedCounter >= iNextDeadlineCounter &&
               dueTicks < KMaxLogicCatchUpTicks)
        {
            AdvanceDeadline();
            ++dueTicks;
        }

        const TBool stillBehind = iElapsedCounter >= iNextDeadlineCounter;
        TBool renderDue = EFalse;
        for (TInt tick = 0; tick < dueTicks && iRunning; ++tick)
        {
            ++iLogicFrame;
            const TBool scheduledRender =
                (iLogicFrame % KLogicTicksPerRender) == 0;
            if (scheduledRender)
                renderDue = ETrue;
            const TBool renderFrame = iFrameSkipEnabled
                ? (!stillBehind && renderDue && tick == dueTicks - 1)
                : scheduledRender;

            TRAPD(error, iObserver.FrameTickL(iLogicFrame, renderFrame));
            if (error != KErrNone)
            {
                iRunning = EFalse;
                iObserver.FrameError(error);
                return;
            }
            if (!iRunning)
                return;

            if (tick + 1 < dueTicks)
                PumpReady();
        }
    }
}
