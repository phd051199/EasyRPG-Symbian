#include <e32base.h>
#include <f32file.h>
#include <videoplayer.h>
#include <w32std.h>

#include "DirentBridge.h"
#include "Encoding.h"
#include "MovieBridge.h"

namespace
{
_LIT(KMovieTempDirectory, "C:\\Data\\EasyRPG\\Movie\\");
_LIT(KMovieTempBase, "movie");

class CMovieBridge : public CBase,
                     public MVideoPlayerUtilityObserver
{
public:
    static CMovieBridge* NewL(RWsSession& aWs,
                              CWsScreenDevice& aScreenDevice,
                              RWindowBase& aWindow)
    {
        return new (ELeave) CMovieBridge(aWs, aScreenDevice, aWindow);
    }

    ~CMovieBridge()
    {
        Stop(EFalse);
        delete iPlayer;
        iPlayer = NULL;
    }

    void PlayL(const TDesC& aPath,
               TInt aX,
               TInt aY,
               TInt aWidth,
               TInt aHeight)
    {
        Stop(EFalse);
        EnsurePlayerL();
        iTargetRect = TargetRectL(aX, aY, aWidth, aHeight);
        iError = KErrNone;
        iState = EActive;
        iPrepared = EFalse;
        iPauseRequested = EFalse;
        iNeedsRedraw = EFalse;

        if (EasyRpgIsVirtualPath(aPath))
        {
            BuildTempPathL(aPath, iPath);
            CopyFileL(aPath, iPath);
            iDeleteFile = ETrue;
        }
        else
        {
            if (aPath.Length() > iPath.MaxLength())
                User::Leave(KErrOverflow);
            iPath.Copy(aPath);
        }

        iPlayer->SetDisplayWindowL(iWs,
                                   iScreenDevice,
                                   iWindow,
                                   iTargetRect,
                                   iTargetRect);
        iPlayer->OpenFileL(iPath);
    }

    TInt Poll() const
    {
        if (iError != KErrNone)
            return iError;
        return iState;
    }

    void Acknowledge()
    {
        if (iState == EDone || iError != KErrNone)
        {
            iState = EIdle;
            iError = KErrNone;
        }
    }

    void Stop(TBool aMarkRedraw)
    {
        const TBool wasActive = iState == EActive;
        if (iPlayer)
        {
            iPlayer->Stop();
            iPlayer->Close();
        }
        CleanupTempFile();
        iState = EIdle;
        iError = KErrNone;
        iPrepared = EFalse;
        iPauseRequested = EFalse;
        if (aMarkRedraw && wasActive)
            iNeedsRedraw = ETrue;
    }

    TBool IsActive() const
    {
        return iState == EActive;
    }

    void SetForeground(TBool aForeground)
    {
        iPauseRequested = !aForeground;
        if (iState != EActive || !iPlayer || !iPrepared)
            return;
        if (aForeground)
            iPlayer->Play();
        else
            TRAP_IGNORE(iPlayer->PauseL());
    }

    TBool ConsumeRedraw()
    {
        const TBool result = iNeedsRedraw;
        iNeedsRedraw = EFalse;
        return result;
    }

private:
    enum TState
    {
        EIdle = 0,
        EActive = 1,
        EDone = 2
    };

    CMovieBridge(RWsSession& aWs,
                 CWsScreenDevice& aScreenDevice,
                 RWindowBase& aWindow)
        : iWs(aWs),
          iScreenDevice(aScreenDevice),
          iWindow(aWindow),
          iPlayer(NULL),
          iState(EIdle),
          iError(KErrNone),
          iDeleteFile(EFalse),
          iPrepared(EFalse),
          iPauseRequested(EFalse),
          iNeedsRedraw(EFalse)
    {
    }

    void EnsurePlayerL()
    {
        if (iPlayer)
            return;
        const TSize size = iScreenDevice.SizeInPixels();
        const TRect rect(TPoint(0, 0), size);
        iPlayer = CVideoPlayerUtility::NewL(*this,
                                            EMdaPriorityNormal,
                                            EMdaPriorityPreferenceTime,
                                            iWs,
                                            iScreenDevice,
                                            iWindow,
                                            rect,
                                            rect);
    }

    TRect TargetRectL(TInt aX,
                      TInt aY,
                      TInt aWidth,
                      TInt aHeight) const
    {
        const TSize size = iScreenDevice.SizeInPixels();
        TInt width = aWidth > 0 ? aWidth : size.iWidth;
        TInt height = aHeight > 0 ? aHeight : size.iHeight;
        TInt left = Max(0, aX);
        TInt top = Max(0, aY);
        TInt right = Min(size.iWidth, aX + width);
        TInt bottom = Min(size.iHeight, aY + height);
        if (right <= left || bottom <= top)
            User::Leave(KErrArgument);
        return TRect(left, top, right, bottom);
    }

    void BuildTempPathL(const TDesC& aSource, TDes& aDestination)
    {
        RFs fs;
        User::LeaveIfError(fs.Connect());
        CleanupClosePushL(fs);
        const TInt directoryError = fs.MkDirAll(KMovieTempDirectory);
        if (directoryError != KErrNone && directoryError != KErrAlreadyExists)
            User::Leave(directoryError);

        aDestination.Copy(KMovieTempDirectory);
        aDestination.Append(KMovieTempBase);
        const TInt dot = aSource.LocateReverse('.');
        if (dot >= 0 && aSource.Length() - dot <= 8)
            aDestination.Append(aSource.Mid(dot));
        CleanupStack::PopAndDestroy(&fs);
    }

    void CopyFileL(const TDesC& aSource, const TDesC& aDestination)
    {
        TBuf8<KMaxFileName * 4 + 1> path8;
        TEncoding::UnicodeToUtf8L(aSource, path8);
        if (path8.Length() >= path8.MaxLength())
            User::Leave(KErrOverflow);
        path8.Append(0);

        FILE* input = EasyRpgFopen(reinterpret_cast<const char*>(path8.Ptr()), "rb");
        if (!input)
            User::Leave(KErrNotFound);

        RFs fs;
        TInt error = fs.Connect();
        if (error != KErrNone)
        {
            EasyRpgFclose(input);
            User::Leave(error);
        }

        RFile output;
        error = output.Replace(fs, aDestination, EFileWrite | EFileShareExclusive);
        if (error == KErrNone)
        {
            TUint8 buffer[8192];
            for (;;)
            {
                const size_t count = EasyRpgFread(buffer, 1, sizeof(buffer), input);
                if (count == 0)
                    break;
                const TPtrC8 chunk(buffer, static_cast<TInt>(count));
                error = output.Write(chunk);
                if (error != KErrNone)
                    break;
            }
            if (error == KErrNone && EasyRpgFerror(input))
                error = KErrCorrupt;
            if (error == KErrNone)
                error = output.Flush();
            output.Close();
        }
        fs.Close();
        EasyRpgFclose(input);
        if (error != KErrNone)
            User::Leave(error);
    }

    void CleanupTempFile()
    {
        if (iDeleteFile && iPath.Length() > 0)
        {
            RFs fs;
            if (fs.Connect() == KErrNone)
            {
                fs.Delete(iPath);
                fs.Close();
            }
        }
        iPath.Zero();
        iDeleteFile = EFalse;
    }

    void Finish(TInt aError)
    {
        if (iPlayer)
            iPlayer->Close();
        CleanupTempFile();
        iError = aError;
        iState = aError == KErrNone ? EDone : EIdle;
        iPrepared = EFalse;
        iPauseRequested = EFalse;
        iNeedsRedraw = ETrue;
    }

    void MvpuoOpenComplete(TInt aError)
    {
        if (aError != KErrNone)
        {
            Finish(aError);
            return;
        }
        iPlayer->Prepare();
    }

    void MvpuoPrepareComplete(TInt aError)
    {
        if (aError != KErrNone)
        {
            Finish(aError);
            return;
        }
        iPrepared = ETrue;
        if (!iPauseRequested)
            iPlayer->Play();
    }

    void MvpuoFrameReady(CFbsBitmap&, TInt)
    {
    }

    void MvpuoPlayComplete(TInt aError)
    {
        Finish(aError);
    }

    void MvpuoEvent(const TMMFEvent&)
    {
    }

private:
    RWsSession& iWs;
    CWsScreenDevice& iScreenDevice;
    RWindowBase& iWindow;
    CVideoPlayerUtility* iPlayer;
    TInt iState;
    TInt iError;
    TBool iDeleteFile;
    TBool iPrepared;
    TBool iPauseRequested;
    TBool iNeedsRedraw;
    TFileName iPath;
    TRect iTargetRect;
};

CMovieBridge* gMovieBridge = NULL;
}

void EasyRpgMovieInitializeL(RWsSession& aWs,
                             CWsScreenDevice& aScreenDevice,
                             RWindowBase& aWindow)
{
    delete gMovieBridge;
    gMovieBridge = CMovieBridge::NewL(aWs, aScreenDevice, aWindow);
}

void EasyRpgMovieShutdown(void)
{
    delete gMovieBridge;
    gMovieBridge = NULL;
}

TBool EasyRpgMovieIsActive(void)
{
    return gMovieBridge && gMovieBridge->IsActive();
}

void EasyRpgMovieSetForeground(TBool aForeground)
{
    if (gMovieBridge)
        gMovieBridge->SetForeground(aForeground);
}

TBool EasyRpgMovieConsumeRedraw(void)
{
    return gMovieBridge && gMovieBridge->ConsumeRedraw();
}

extern "C" int EasyRpgMovieStart(const char* aPath,
                                  int aX,
                                  int aY,
                                  int aWidth,
                                  int aHeight)
{
    if (!gMovieBridge || !aPath || !aPath[0])
        return KErrNotReady;

    TInt error = KErrNone;
    TRAP(error,
    {
        TInt length = 0;
        while (aPath[length])
            ++length;
        TPtrC8 source(reinterpret_cast<const TUint8*>(aPath), length);
        TFileName path;
        TEncoding::Utf8ToUnicodeL(source, path);
        gMovieBridge->PlayL(path, aX, aY, aWidth, aHeight);
    });
    if (error != KErrNone)
        gMovieBridge->Stop(ETrue);
    return error;
}

extern "C" int EasyRpgMoviePoll(void)
{
    return gMovieBridge ? gMovieBridge->Poll() : KErrNotReady;
}

extern "C" void EasyRpgMovieAcknowledge(void)
{
    if (gMovieBridge)
        gMovieBridge->Acknowledge();
}

extern "C" void EasyRpgMovieStop(void)
{
    if (gMovieBridge)
        gMovieBridge->Stop(ETrue);
}
