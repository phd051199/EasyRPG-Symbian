#ifndef EASYRPG_FRAME_TIMER_H
#define EASYRPG_FRAME_TIMER_H

#include <e32base.h>

class MFrameTimerObserver
{
public:
    virtual void FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame) = 0;
    virtual void FrameError(TInt aError) = 0;
};

class CFrameTimer : public CBase
{
public:
    static CFrameTimer* NewL(MFrameTimerObserver& aObserver);
    ~CFrameTimer();

    void Start();
    void Stop();
    TBool Running() const;
    void SetFrameSkipEnabled(TBool aEnabled);
    TBool FrameSkipEnabled() const;

private:
    enum
    {
        KLogicFps = 60,
        KRenderFps = 30,
        KLogicTicksPerRender = KLogicFps / KRenderFps,
        KMaxLogicCatchUpTicks = 2,
        KMaxReadyCallbacks = 8,
        KSleepGuardMicroseconds = 250,
        KMaxSleepMicroseconds = 2000
    };

    explicit CFrameTimer(MFrameTimerObserver& aObserver);
    void ConstructL();
    void RunLoop();
    TBool PumpReady();
    void AdvanceDeadline();
    void UpdateElapsedCounter(TUint32 aCounter);
    TUint32 CounterDelta(TUint32 aCurrent, TUint32 aPrevious) const;
    TInt CounterTicksToMicroseconds(TInt64 aTicks) const;

private:
    MFrameTimerObserver& iObserver;
    TInt iCounterFrequency;
    TBool iCounterCountsUp;
    TInt iFramePeriodCounter;
    TInt iFramePeriodRemainder;
    TInt iFrameRemainderAccumulator;
    TUint32 iLastCounter;
    TInt64 iElapsedCounter;
    TInt64 iNextDeadlineCounter;
    TUint32 iLogicFrame;
    TBool iRunning;
    TBool iFrameSkipEnabled;
};

#endif
