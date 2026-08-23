#ifndef EASYRPG_GAME_LAUNCH_WATCHER_H
#define EASYRPG_GAME_LAUNCH_WATCHER_H

#include <e32base.h>
#include <f32file.h>

class CLauncherView;

enum TGameLaunchStage
{
    ELaunchStageNone = 0,
    ELaunchStageCreate,
    ELaunchStageRendezvous,
    ELaunchStageRuntimeStartup
};

const TDesC& GameLaunchStageName(TGameLaunchStage aStage);

class CGameLaunchWatcher : public CActive
{
public:
    static CGameLaunchWatcher* NewL(CLauncherView& aOwner);
    ~CGameLaunchWatcher();

    void StartL(const TDesC& aExecutablePath, const TDesC& aArchivePath);
    TGameLaunchStage FailureStage() const;
    const TDesC& ExitCategory() const;
    TInt ExitReason() const;

private:
    enum TState
    {
        EIdle,
        EDeferredStart,
        EWaitingForRuntime
    };

    explicit CGameLaunchWatcher(CLauncherView& aOwner);
    void StartProcessL();
    void RunL();
    TInt RunError(TInt aError);
    void DoCancel();

private:
    CLauncherView& iOwner;
    RProcess iProcess;
    TFileName iExecutablePath;
    TFileName iArchivePath;
    TState iState;
    TBool iCreated;
    TGameLaunchStage iFailureStage;
    TBuf<32> iExitCategory;
    TInt iExitReason;
};

#endif
