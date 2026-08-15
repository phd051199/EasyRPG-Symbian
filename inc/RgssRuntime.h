#ifndef EASYRPG_RGSS_RUNTIME_H
#define EASYRPG_RGSS_RUNTIME_H

#include <e32base.h>
#include <f32file.h>

class CZipVfs;
class CRgssDisplaySurface;
class CRgssVmWorker;

class CRgssRuntime : public CBase
{
public:
    static CRgssRuntime* NewL(const TDesC& aGamePath);
    ~CRgssRuntime();

    TInt Version() const;
    TInt LogicFps() const;
    TInt TargetFps() const;
    TInt BootPhase() const;
    TInt BootPercent() const;
    void StartVmL();
    TBool AdvanceVmFrameL(TBool aCompose, TUint32 aInputWord0);
    void ComposeL();
    void SetInputWord(TInt aWord, TUint32 aValue);
    const CRgssDisplaySurface& Surface() const;
    CRgssDisplaySurface& Surface();

private:
    CRgssRuntime();
    void ConstructL(const TDesC& aGamePath);
    TInt DetectVersion() const;
    void ProcessAudioCommands();
    static void* BitmapAllocate(void* aContext, unsigned int aBytes);
    static void BitmapRelease(void* aContext, void* aMemory);

private:
    RFs iFs;
    RAllocator* iAllocator;
    CZipVfs* iResources;
    CRgssDisplaySurface* iSurface;
    CRgssVmWorker* iVmWorker;
    TFileName iGamePath;
    TInt iVersion;
    TInt iLogicFps;
    TInt iTargetFps;
    TUint32 iInputWords[4];
    TBool iVmStarted;
    // Previous frame's rendered dirty rect (physical coords). The present
    // path marks the union of previous and current dirty so a renderable
    // that moved away is still repainted (its old area is now clear color).
    TInt iPreviousDirtyX;
    TInt iPreviousDirtyY;
    TInt iPreviousDirtyWidth;
    TInt iPreviousDirtyHeight;
    TBool iPreviousDirtyValid;
};

#endif
