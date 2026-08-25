#include <hal.h>
#include <hal_data.h>

#include "FrameTimer.h"
#include "PlayerConfig.h"

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
      iMicrosPerTickX65536(0),
      iCounterCountsUp(ETrue),
      iFramePeriodCounter(0),
      iFramePeriodRemainder(0),
      iFrameRemainderAccumulator(0),
      iLastCounter(0),
      iElapsedCounter(0),
      iNextDeadlineCounter(0),
      iLogicFrame(0),
      iLogicTicksSinceRender(0),
      iImmediateCatchUpTicks(0),
      iRunning(EFalse)
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
    if (iCounterFrequency < EasyRpgPlayerConfig::KLogicFps)
        User::Leave(KErrNotSupported);
    // 16.16 fixed-point reciprocal: Symbian fast counters run at 1 MHz or
    // 32768 Hz on target hardware, where this is exact and the per-tick
    // 64-bit divide becomes a multiply+shift (no FPU, soft __aeabi_ldivmod).
    iMicrosPerTickX65536 = (static_cast<TInt64>(1000000) << 16) / iCounterFrequency;
    iFramePeriodCounter = iCounterFrequency / EasyRpgPlayerConfig::KLogicFps;
    iFramePeriodRemainder = iCounterFrequency % EasyRpgPlayerConfig::KLogicFps;
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
    iLogicTicksSinceRender = 0;
    iImmediateCatchUpTicks = 0;
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

TInt CFrameTimer::TimeUntilNextTickMicroseconds() const
{
    if (!iRunning)
        return 0;
    const TUint32 now = User::FastCounter();
    const TInt64 elapsed = iElapsedCounter + CounterDelta(now, iLastCounter);
    return CounterTicksToMicroseconds(iNextDeadlineCounter - elapsed);
}

void CFrameTimer::AdvanceDeadline()
{
    iNextDeadlineCounter += iFramePeriodCounter;
    iFrameRemainderAccumulator += iFramePeriodRemainder;
    if (iFrameRemainderAccumulator >= EasyRpgPlayerConfig::KLogicFps)
    {
        ++iNextDeadlineCounter;
        iFrameRemainderAccumulator -= EasyRpgPlayerConfig::KLogicFps;
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
    if (aTicks > iCounterFrequency)
        return 1000000;
    const TInt64 microseconds =
        (aTicks * iMicrosPerTickX65536 + 0xffff) >> 16;
    return microseconds > 0 ? static_cast<TInt>(microseconds) : 1;
}

void CFrameTimer::ScheduleNext()
{
    if (!iRunning || IsActive())
        return;

    UpdateElapsedCounter(User::FastCounter());
    const TInt64 remaining = iNextDeadlineCounter - iElapsedCounter;
    if (remaining <= 0)
    {
        if (iImmediateCatchUpTicks < 2)
        {
            ++iImmediateCatchUpTicks;
            iStatus = KRequestPending;
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete(status, KErrNone);
        }
        else
        {
            iImmediateCatchUpTicks = 0;
            iTimer.HighRes(iStatus, TTimeIntervalMicroSeconds32(1000));
            SetActive();
        }
        return;
    }

    iImmediateCatchUpTicks = 0;
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
    const TInt64 behindTicks = iElapsedCounter - iNextDeadlineCounter;
    if (behindTicks > static_cast<TInt64>(EasyRpgPlayerConfig::KMaxCatchUpLogicFrames) * iFramePeriodCounter)
    {
        iNextDeadlineCounter = iElapsedCounter;
        iFrameRemainderAccumulator = 0;
        iLogicTicksSinceRender = EasyRpgPlayerConfig::KLogicTicksPerRender;
    }
    if (iElapsedCounter < iNextDeadlineCounter)
    {
        ScheduleNext();
        return;
    }

    const TInt64 renderLagTicks = iElapsedCounter - iNextDeadlineCounter;
    AdvanceDeadline();
    ++iLogicFrame;
    if (iLogicTicksSinceRender < EasyRpgPlayerConfig::KLogicTicksPerRender)
        ++iLogicTicksSinceRender;
    const TBool scheduledRender =
        iLogicTicksSinceRender >= EasyRpgPlayerConfig::KLogicTicksPerRender;
    const TInt64 renderBudgetTicks =
        static_cast<TInt64>(iFramePeriodCounter) *
        EasyRpgPlayerConfig::KLogicTicksPerRender;
    const TBool renderFrame = scheduledRender &&
        renderLagTicks < renderBudgetTicks;
    if (renderFrame)
        iLogicTicksSinceRender = 0;

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
