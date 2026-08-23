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
      iCounterCountsUp(ETrue),
      iFramePeriodCounter(0),
      iFramePeriodRemainder(0),
      iFrameRemainderAccumulator(0),
      iLastCounter(0),
      iElapsedCounter(0),
      iNextDeadlineCounter(0),
      iLogicFrame(0),
      iRunning(EFalse),
      iFrameSkipEnabled(EFalse),
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
    if (iCounterFrequency < EasyRpgPlayerConfig::KLogicFps)
        User::Leave(KErrNotSupported);
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
        (aTicks * 1000000 + iCounterFrequency - 1) / iCounterFrequency;
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
    const TInt64 behindTicks = iElapsedCounter - iNextDeadlineCounter;
    if (behindTicks > static_cast<TInt64>(EasyRpgPlayerConfig::KMaxCatchUpLogicFrames) * iFramePeriodCounter)
    {
        iNextDeadlineCounter = iElapsedCounter;
        iFrameRemainderAccumulator = 0;
    }
    if (iElapsedCounter < iNextDeadlineCounter)
    {
        ScheduleNext();
        return;
    }

    AdvanceDeadline();
    ++iLogicFrame;
    const TBool scheduledRender =
        (iLogicFrame % EasyRpgPlayerConfig::KLogicTicksPerRender) == 0;
    if (scheduledRender)
        iRenderPending = ETrue;

    const TBool stillBehind = iElapsedCounter >= iNextDeadlineCounter;
    TBool renderFrame = EFalse;
    if (!iFrameSkipEnabled)
        renderFrame = scheduledRender;
    else if (iRenderPending && !stillBehind)
        renderFrame = ETrue;
    if (renderFrame)
        iRenderPending = EFalse;

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
