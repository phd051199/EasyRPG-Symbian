#include "RgssEngine.h"
#include "RgssHost.h"

namespace
{
int HostInit(void* aContext, int aWidth, int aHeight)
{
    return static_cast<CRgssEngine*>(aContext)->HostInit(aWidth, aHeight);
}

void HostPresent(void* aContext, const unsigned short* aFrame, int aWidth, int aHeight)
{
    static_cast<CRgssEngine*>(aContext)->HostPresent(aFrame, aWidth, aHeight);
}

unsigned int HostPollInput(void* aContext)
{
    return static_cast<CRgssEngine*>(aContext)->HostPollInput();
}

int HostBgmPlay(void* aContext, const char* aPath, int aVolume, int aPitch)
{
    return static_cast<CRgssEngine*>(aContext)->HostBgmPlay(aPath, aVolume, aPitch);
}
void HostBgmStop(void* aContext) { static_cast<CRgssEngine*>(aContext)->HostBgmStop(); }
void HostBgmFade(void* aContext, int aMs) { static_cast<CRgssEngine*>(aContext)->HostBgmFade(aMs); }
int HostBgmPos(void* aContext) { return static_cast<CRgssEngine*>(aContext)->HostBgmPos(); }
void HostBgmPause(void* aContext) { static_cast<CRgssEngine*>(aContext)->HostBgmPause(); }
void HostBgmResume(void* aContext) { static_cast<CRgssEngine*>(aContext)->HostBgmResume(); }

int HostBgsPlay(void* aContext, const char* aPath, int aVolume, int aPitch)
{
    return static_cast<CRgssEngine*>(aContext)->HostBgsPlay(aPath, aVolume, aPitch);
}
void HostBgsStop(void* aContext) { static_cast<CRgssEngine*>(aContext)->HostBgsStop(); }
void HostBgsFade(void* aContext, int aMs) { static_cast<CRgssEngine*>(aContext)->HostBgsFade(aMs); }

int HostMePlay(void* aContext, const char* aPath, int aVolume, int aPitch)
{
    return static_cast<CRgssEngine*>(aContext)->HostMePlay(aPath, aVolume, aPitch);
}
void HostMeStop(void* aContext) { static_cast<CRgssEngine*>(aContext)->HostMeStop(); }
void HostMeFade(void* aContext, int aMs) { static_cast<CRgssEngine*>(aContext)->HostMeFade(aMs); }

int HostSePlay(void* aContext, const char* aPath, int aVolume, int aPitch)
{
    return static_cast<CRgssEngine*>(aContext)->HostSePlay(aPath, aVolume, aPitch);
}
void HostSeStop(void* aContext) { static_cast<CRgssEngine*>(aContext)->HostSeStop(); }
void HostAudioUpdate(void* aContext) { static_cast<CRgssEngine*>(aContext)->HostAudioUpdate(); }
void HostAudioShutdown(void* aContext) { static_cast<CRgssEngine*>(aContext)->HostAudioShutdown(); }

int HostFileExists(void* aContext, const char* aPath)
{
    return static_cast<CRgssEngine*>(aContext)->HostFileExists(aPath);
}
long HostFileRead(void* aContext, const char* aPath, unsigned char** aOutput)
{
    return static_cast<CRgssEngine*>(aContext)->HostFileRead(aPath, aOutput);
}
long HostFileWrite(void* aContext, const char* aPath,
                   const unsigned char* aData, long aSize)
{
    return static_cast<CRgssEngine*>(aContext)->HostFileWrite(aPath, aData, aSize);
}
int HostFileDelete(void* aContext, const char* aPath)
{
    return static_cast<CRgssEngine*>(aContext)->HostFileDelete(aPath);
}
int HostFileRename(void* aContext, const char* aOldPath, const char* aNewPath)
{
    return static_cast<CRgssEngine*>(aContext)->HostFileRename(aOldPath, aNewPath);
}

void HostLog(void* aContext, const char* aMessage)
{
    static_cast<CRgssEngine*>(aContext)->HostLog(aMessage);
}
unsigned int HostTicksMs(void* aContext)
{
    return static_cast<CRgssEngine*>(aContext)->HostTicksMs();
}
void HostDelayMs(void* aContext, unsigned int aMilliseconds)
{
    static_cast<CRgssEngine*>(aContext)->HostDelayMs(aMilliseconds);
}
}

void RgssHostSymbianInstall(CRgssEngine* aEngine)
{
    static RgssHostVtable host;
    host.ctx = aEngine;
    host.Init = HostInit;
    host.Present = HostPresent;
    host.PollInput = HostPollInput;
    host.BgmPlay = HostBgmPlay;
    host.BgmStop = HostBgmStop;
    host.BgmFade = HostBgmFade;
    host.BgmPos = HostBgmPos;
    host.BgmPause = HostBgmPause;
    host.BgmResume = HostBgmResume;
    host.BgsPlay = HostBgsPlay;
    host.BgsStop = HostBgsStop;
    host.BgsFade = HostBgsFade;
    host.MePlay = HostMePlay;
    host.MeStop = HostMeStop;
    host.MeFade = HostMeFade;
    host.SePlay = HostSePlay;
    host.SeStop = HostSeStop;
    host.AudioUpdate = HostAudioUpdate;
    host.AudioShutdown = HostAudioShutdown;
    host.FileExists = HostFileExists;
    host.FileRead = HostFileRead;
    host.FileWrite = HostFileWrite;
    host.FileDelete = HostFileDelete;
    host.FileRename = HostFileRename;
    host.Log = HostLog;
    host.TicksMs = HostTicksMs;
    host.DelayMs = HostDelayMs;
    RgssHostSet(&host);
}
