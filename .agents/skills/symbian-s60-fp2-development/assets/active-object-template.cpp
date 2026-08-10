#include <e32base.h>

class CRequestAo : public CActive
{
public:
    static CRequestAo* NewL();
    ~CRequestAo();
    void StartL();

private:
    CRequestAo() : CActive(EPriorityStandard), iState(EIdle) {}
    void ConstructL() { CActiveScheduler::Add(this); }
    void RunL();
    void DoCancel();
    TInt RunError(TInt aError);

private:
    enum TState { EIdle, ERequestPending } iState;
};

void CRequestAo::StartL()
{
    if (IsActive()) User::Leave(KErrInUse);
    // Issue provider request with iStatus here.
    iState = ERequestPending;
    SetActive();
}

void CRequestAo::RunL()
{
    User::LeaveIfError(iStatus.Int());
    iState = EIdle;
    // Process completion.
}

void CRequestAo::DoCancel()
{
    // Call the provider-specific cancel function here.
    iState = EIdle;
}

TInt CRequestAo::RunError(TInt aError)
{
    iState = EIdle;
    return aError;
}
