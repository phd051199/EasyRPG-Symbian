#ifndef EASYRPG_RGSS_VM_WORKER_H
#define EASYRPG_RGSS_VM_WORKER_H

#include <e32base.h>
#include <f32file.h>

class CZipVfs;
class CRgssGlyphCache;
struct EasyRpgRgssScriptEntry;

struct TRgssAudioCommand
{
    TInt iCommand;
    TInt iVolume;
    TInt iPitch;
    TInt iDuration;
    TBuf8<256> iPath;
};

class CRgssVmWorker : public CBase
{
public:
    static CRgssVmWorker* NewL(const TDesC& aGamePath, TInt aVersion);
    ~CRgssVmWorker();

    void StartL();
    void Stop();
    TBool TakeFrame(TInt& aFrameRate, TInt& aFrameCount);
    void ResumeFrame();
    void SetInputWord(TInt aWord, TUint32 aValue);
    TBool TakeAudioCommand(TRgssAudioCommand& aCommand);
    TBool Running() const;
    TInt ExitResult() const;
    TInt FrameRate() const;
    TInt BootPhase() const;
    TInt BootPercent() const;

private:
    CRgssVmWorker(TInt aVersion);
    void ConstructL(const TDesC& aGamePath);
    static TInt ThreadEntry(TAny* aContext);
    static TAny* PthreadEntry(TAny* aContext);
    TInt ThreadMain();
    void RunVmL();
    void SignalStarted(TInt aResult);
    static void DisconnectFbs(TAny* aContext);
    static void DestroyGlyphCache(TAny* aContext);
    static void FreeScriptEntries(TAny* aContext);
    static void ShutdownGraphics(TAny* aContext);
    static void ShutdownVm(TAny* aContext);
    static TInt CopyVmPath(const char* aSource, TDes& aTarget);
    static int VmFileSize(void* aContext, const char* aRelativePath);
    static int VmFileRead(void* aContext,
                          const char* aRelativePath,
                          int aOffset,
                          void* aBuffer,
                          int aBytes);
    static int VmFileWrite(void* aContext,
                           const char* aRelativePath,
                           int aOffset,
                           const void* aBuffer,
                           int aBytes,
                           int aTruncate);
    static unsigned int VmTicks(void* aContext);
    static unsigned int VmInput(void* aContext, int aWord);
    static int VmBitmapLoad(void* aContext,
                            const char* aRelativePath,
                            int* aBitmapHandle);
    static int VmTextDecode(void* aContext,
                            const unsigned char* aText,
                            int aTextLength,
                            unsigned int* aCodepoints,
                            int aCapacity);
    static int ScriptArchiveRead(void* aContext,
                                 int aOffset,
                                 void* aBuffer,
                                 int aBytes);
    static int VmScriptCount(void* aContext);
    static int VmScriptInfo(void* aContext,
                            int aIndex,
                            int* aId,
                            int* aNameLength);
    static int VmScriptName(void* aContext,
                            int aIndex,
                            void* aBuffer,
                            int aCapacity);
    static int VmScriptInflate(void* aContext,
                               int aIndex,
                               unsigned char** aSource,
                               int* aSourceLength);
    static void VmRelease(void* aContext, void* aMemory);
    static int VmFrameSync(void* aContext,
                           int aFrameRate,
                           int aFrameCount);
    static void VmDirty(void* aContext,
                        int aX,
                        int aY,
                        int aWidth,
                        int aHeight);
    static int VmAudio(void* aContext,
                       int aCommand,
                       const char* aRelativePath,
                       int aVolume,
                       int aPitch,
                       int aDuration);
    static void VmBootProgress(void* aContext, int aPhase, int aPercent);
    void SetBootProgress(TInt aPhase, TInt aPercent);
    TInt QueueAudioCommand(TInt aCommand,
                           const char* aRelativePath,
                           TInt aVolume,
                           TInt aPitch,
                           TInt aDuration);
    TInt ResolveAudioPath(const char* aRelativePath, TDes8& aResolved) const;
    TInt FileSize(const char* aRelativePath);
    TInt ReadFile(const char* aRelativePath,
                  TInt aOffset,
                  TAny* aBuffer,
                  TInt aBytes);
    TInt WriteFile(const char* aRelativePath,
                   TInt aOffset,
                   const TAny* aBuffer,
                   TInt aBytes,
                   TBool aTruncate);
    TInt BuildSaveFilePath(const char* aRelativePath, TDes& aPath) const;

private:
    TFileName iGamePath;
    TInt iVersion;
    RFs iFs;
    CZipVfs* iResources;
    CRgssGlyphCache* iGlyphCache;
    RThread iThread;
    RSemaphore iStarted;
    RSemaphore iFrameReady;
    RSemaphore iFrameResume;
    volatile TInt iStartResult;
    volatile TInt iExitResult;
    volatile TInt iFrameRate;
    volatile TInt iFrameCount;
    TInt iScriptDataOffset;
    TInt iScriptFileSize;
    EasyRpgRgssScriptEntry* iScriptEntries;
    TInt iScriptCount;
    volatile TUint32 iInputWords[4];
    TRgssAudioCommand iAudioCommands[32];
    volatile TInt iAudioCommandCount;
    TInt iAudioCommandRead;
    volatile TBool iStopRequested;
    volatile TBool iStartSignaled;
    volatile TBool iVmInitialized;
    // Written by the VM thread only, read by the engine thread to drive the
    // boot loading gauge. Aligned 32-bit stores are atomic on this target.
    volatile TInt iBootPhase;
    volatile TInt iBootPercent;
    TBool iThreadCreated;
};

#endif
