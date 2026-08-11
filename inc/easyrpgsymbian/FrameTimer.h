#ifndef EASYRPG_SYMBIAN_FRAME_TIMER_H
#define EASYRPG_SYMBIAN_FRAME_TIMER_H

#include <e32base.h>

class MFrameTimerObserver
{
public:
    virtual void FrameTickL(TUint32 aLogicFrame, TBool aRenderFrame) = 0;
    virtual void FrameError(TInt aError) = 0;
};

class CFrameTimer : public CActive
{
public:
    static CFrameTimer* NewL(MFrameTimerObserver& aObserver);
    ~CFrameTimer();

    void Start();
    void Stop();
    TBool Running() const;

private:
    enum
    {
        KLogicTickMicroseconds = 16667,
        KRenderDivisor = 2
    };

    explicit CFrameTimer(MFrameTimerObserver& aObserver);
    void ConstructL();
    void Arm();
    void RunL();
    TInt RunError(TInt aError);
    void DoCancel();

private:
    MFrameTimerObserver& iObserver;
    RTimer iTimer;
    TUint32 iLogicFrame;
    TBool iRunning;
};

#endif
