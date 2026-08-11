#ifndef EASYRPG_SYMBIAN_PLAYER_RUNTIME_H
#define EASYRPG_SYMBIAN_PLAYER_RUNTIME_H

#include <e32base.h>

class CSymbianDisplaySurface;
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
        EKeyDebug = 0x00000080
    };

    static CPlayerRuntime* NewL(CZipVfs* aResources,
                                const TDesC& aGameSource,
                                CSymbianDisplaySurface& aDisplaySurface);
    ~CPlayerRuntime();

    void SetKeyState(TUint32 aKey, TBool aPressed);
    void ReleaseAllKeys();
    void Suspend();
    void Resume();
    void TickL(TUint32 aLogicFrame, TBool aRenderFrame);
    TBool IsRunning() const;

private:
    CPlayerRuntime(CZipVfs* aResources, CSymbianDisplaySurface& aDisplaySurface);
    void ConstructL(const TDesC& aGameSource);
    void PrepareProjectRootL(const TDesC& aGameSource, TDes8& aProjectPath, TDes8& aSavePath);

private:
    CZipVfs* iResources;
    CSymbianDisplaySurface& iDisplaySurface;
    TUint32 iPressedKeys;
    TBool iPlayerRunning;
};

#endif
