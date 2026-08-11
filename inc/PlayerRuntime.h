#ifndef EASYRPG_PLAYER_RUNTIME_H
#define EASYRPG_PLAYER_RUNTIME_H

#include <e32base.h>

class CDisplaySurface;
class CZipVfs;

class CPlayerRuntime : public CBase
{
public:
    enum TPlayerKey
    {
        EKeyUp = 0x00000001,
        EKeyDown = 0x00000002,
        EKeyLeft = 0x00000004,
        EKeyRight = 0x00000008,
        EKeyDecision = 0x00000010,
        EKeyCancel = 0x00000020,
        EKeyShift = 0x00000040,
        EKeyDebug = 0x00000080,
        EKeyN0 = 0x00000100,
        EKeyN1 = 0x00000200,
        EKeyN2 = 0x00000400,
        EKeyN3 = 0x00000800,
        EKeyN4 = 0x00001000,
        EKeyN5 = 0x00002000,
        EKeyN6 = 0x00004000,
        EKeyN7 = 0x00008000,
        EKeyN8 = 0x00010000,
        EKeyN9 = 0x00020000,
        EKeyPlus = 0x00040000,
        EKeyMinus = 0x00080000,
        EKeyMultiply = 0x00100000,
        EKeyDivide = 0x00200000,
        EKeyPeriod = 0x00400000
    };

    static CPlayerRuntime* NewL(CZipVfs* aResources,
                                const TDesC& aGameSource,
                                CDisplaySurface& aDisplaySurface);
    ~CPlayerRuntime();

    void SetKeyState(TUint32 aKey, TBool aPressed);
    void ReleaseAllKeys();
    void Suspend();
    void Resume();
    void TickL(TUint32 aLogicFrame, TBool aRenderFrame);
    TBool IsRunning() const;

private:
    CPlayerRuntime(CZipVfs* aResources, CDisplaySurface& aDisplaySurface);
    void ConstructL(const TDesC& aGameSource);
    void PrepareProjectRootL(const TDesC& aGameSource, TDes8& aProjectPath, TDes8& aSavePath);

private:
    CZipVfs* iResources;
    CDisplaySurface& iDisplaySurface;
    TUint32 iUpstreamKeys0;
    TUint32 iUpstreamKeys1;
    TUint32 iUpstreamKeys2;
    TUint32 iTickMs;
    TInt iTickRemainder;
    TBool iPlayerRunning;
};

#endif
