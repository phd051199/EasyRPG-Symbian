#ifndef EASYRPG_SYMBIAN_PLAYER_RUNTIME_H
#define EASYRPG_SYMBIAN_PLAYER_RUNTIME_H

#include <e32base.h>
#include <f32file.h>

class CFrameBuffer;

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

    static CPlayerRuntime* NewL(RFs& aFs,
                                const TDesC& aGameRoot,
                                CFrameBuffer& aFrameBuffer);
    ~CPlayerRuntime();

    void SetKeyState(TUint32 aKey, TBool aPressed);
    void ReleaseAllKeys();
    void TickL(TUint32 aLogicFrame, TBool aRenderFrame);

    const TDesC& StatusText() const;
    const TDesC& GameTitle() const;
    const TDesC& GameRoot() const;
    TUint32 PressedKeys() const;

private:
    CPlayerRuntime(RFs& aFs, CFrameBuffer& aFrameBuffer);
    void ConstructL(const TDesC& aGameRoot);
    void RenderDiagnosticL(TUint32 aLogicFrame);

private:
    RFs& iFs;
    CFrameBuffer& iFrameBuffer;
    TFileName iGameRoot;
    TBuf<96> iStatusText;
    TBuf<96> iGameTitle;
    TUint32 iPressedKeys;
    TBool iLcfReady;
    TBool iTitleReady;
    TInt iMarkerX;
    TInt iMarkerY;
};

#endif
