#ifndef EASYRPG_SCRIPT_RUNTIME_H
#define EASYRPG_SCRIPT_RUNTIME_H

#include <e32base.h>
#include <f32file.h>

class CZipVfs;

class CScriptRuntime : public CBase
{
public:
    static CScriptRuntime* NewL(RFs& aFs, const TDesC& aArchivePath);
    ~CScriptRuntime();

    void ExecuteL();
    TInt Generation() const;
    const TDesC8& LastError() const;

private:
    explicit CScriptRuntime(RFs& aFs);
    void ConstructL(const TDesC& aArchivePath);
    void SelectGenerationL();
    void CopyVmError(const char* aError);

private:
    RFs& iFs;
    CZipVfs* iResources;
    TInt iGeneration;
    TBool iVmCreated;
    TBuf8<512> iLastError;
};

#endif
