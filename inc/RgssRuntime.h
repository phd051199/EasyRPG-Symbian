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
    void StartVmL();
    TBool AdvanceVmFrameL(TBool aCompose);
    void ComposeL();
    void SetInputWord(TInt aWord, TUint32 aValue);
    const CRgssDisplaySurface& Surface() const;
    CRgssDisplaySurface& Surface();

private:
    CRgssRuntime();
    void ConstructL(const TDesC& aGamePath);
    TInt DetectVersion() const;
    static void* BitmapAllocate(void* aContext, unsigned int aBytes);
    static void BitmapRelease(void* aContext, void* aMemory);

private:
    RFs iFs;
    CZipVfs* iResources;
    CRgssDisplaySurface* iSurface;
    CRgssVmWorker* iVmWorker;
    TFileName iGamePath;
    TInt iVersion;
    TInt iLogicFps;
    TInt iTargetFps;
    TUint32 iInputWords[4];
    TBool iVmStarted;
};

#endif
