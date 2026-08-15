#ifndef EASYRPG_RGSS_ENGINE_H
#define EASYRPG_RGSS_ENGINE_H

#include <e32base.h>
#include <f32file.h>
#include <w32std.h>

class CFbsBitmap;
class CZipVfs;
class CWsScreenDevice;
class CWindowGc;
class RWindowGroup;
class RWindow;

class CRgssEngine : public CBase
{
public:
    static CRgssEngine* NewL(RFs& aFs, const TDesC& aArchivePath);
    ~CRgssEngine();

    void RunL();
    TInt Generation() const;
    const TDesC8& LastError() const;

    int HostInit(int aNativeWidth, int aNativeHeight);
    void HostPresent(const unsigned short* aFrame, int aWidth, int aHeight);
    TUint32 HostPollInput();

    int HostBgmPlay(const char* aPath, int aVolume, int aPitch);
    void HostBgmStop();
    void HostBgmFade(int aMilliseconds);
    int HostBgmPos();
    void HostBgmPause();
    void HostBgmResume();
    int HostBgsPlay(const char* aPath, int aVolume, int aPitch);
    void HostBgsStop();
    void HostBgsFade(int aMilliseconds);
    int HostMePlay(const char* aPath, int aVolume, int aPitch);
    void HostMeStop();
    void HostMeFade(int aMilliseconds);
    int HostSePlay(const char* aPath, int aVolume, int aPitch);
    void HostSeStop();
    void HostAudioUpdate();
    void HostAudioShutdown();

    int HostFileExists(const char* aPath);
    long HostFileRead(const char* aPath, unsigned char** aOutput);
    long HostFileWrite(const char* aPath, const unsigned char* aData, long aSize);
    int HostFileDelete(const char* aPath);
    int HostFileRename(const char* aOldPath, const char* aNewPath);

    void HostLog(const char* aMessage);
    TUint32 HostTicksMs() const;
    void HostDelayMs(TUint32 aMilliseconds);

private:
    explicit CRgssEngine(RFs& aFs);
    void ConstructL(const TDesC& aArchivePath);
    void SelectGenerationL();
    void PrepareVirtualProjectL();
    void CreateWindowL();
    void DestroyWindow();
    void PumpActiveObjects();
    void DrainWindowEvents();
    void HandleWindowEvent(const TWsEvent& aEvent);
    TUint32 MapKey(const TKeyEvent& aKey) const;
    void BuildArchivePathL(const char* aPath, TDes& aResult) const;
    void BuildSavePathL(const char* aPath, TDes& aResult) const;
    void BuildVirtualAudioPathL(const char* aPath, TDes8& aResult) const;
    TBool ReadSaveFileL(const TDesC& aPath, HBufC8*& aData) const;
    void CopyError(const char* aError);

private:
    RFs& iFs;
    CZipVfs* iResources;
    TInt iGeneration;
    TFileName iArchivePath;
    TFileName iProjectRoot;
    TFileName iSaveRoot;
    TBuf8<512> iLastError;

    RWsSession iWs;
    CWsScreenDevice* iScreenDevice;
    RWindowGroup* iWindowGroup;
    RWindow* iWindow;
    CWindowGc* iWindowGc;
    CFbsBitmap* iBitmap;
    TBool iWindowReady;
    TInt iNativeWidth;
    TInt iNativeHeight;
    TUint32 iHeldButtons;
    TUint32 iPulseButtons;
    TBool iRightSoftHeld;
};

void RgssHostSymbianInstall(CRgssEngine* aEngine);

#endif
