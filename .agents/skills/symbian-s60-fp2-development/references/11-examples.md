# Minimal Symbian C++ examples

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-10-examples-audio-player-md"></a>
## Example: Audio Player Lifecycle

- Original corpus path: `10_examples/audio_player.md`
- Consolidated reference: `11-examples.md#src-10-examples-audio-player-md`

### Example: Audio Player Lifecycle

```cpp
class CAudioEngine : public CBase,
                     public MMdaAudioPlayerCallback
{
public:
    void ConstructL()
    {
        iPlayer = CMdaAudioPlayerUtility::NewL(*this);
    }

    void OpenL(const TDesC& aFile)
    {
        iPlayer->OpenFileL(aFile);
    }

    void MapcInitComplete(TInt aError,
                          const TTimeIntervalMicroSeconds& aDuration)
    {
        if (aError == KErrNone)
        {
            iDuration = aDuration;
            iPlayer->SetVolume(iPlayer->MaxVolume() / 2);
            iPlayer->Play();
        }
    }

    void MapcPlayComplete(TInt aError)
    {
        // report completion/interruption
    }

    ~CAudioEngine()
    {
        if (iPlayer)
            iPlayer->Stop();
        delete iPlayer;
    }
private:
    CMdaAudioPlayerUtility* iPlayer;
    TTimeIntervalMicroSeconds iDuration;
};
```

Verify callback interface/header names in the installed FP2 multimedia headers.

---

<a id="src-10-examples-file-io-md"></a>
## Example: Leave-Safe File Write and Read

- Original corpus path: `10_examples/file_io.md`
- Consolidated reference: `11-examples.md#src-10-examples-file-io-md`

### Example: Leave-Safe File Write and Read

```cpp
#include <f32file.h>

void WriteFileL(const TDesC& aPath, const TDesC8& aData)
{
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);

    User::LeaveIfError(fs.MkDirAll(aPath)); // for real code, pass directory separately

    RFile file;
    User::LeaveIfError(file.Replace(fs, aPath, EFileWrite | EFileShareExclusive));
    CleanupClosePushL(file);

    User::LeaveIfError(file.Write(aData));
    User::LeaveIfError(file.Flush());

    CleanupStack::PopAndDestroy(&file);
    CleanupStack::PopAndDestroy(&fs);
}
```

The `MkDirAll(aPath)` line is intentionally highlighted as a design smell: it expects a directory path, not a full filename. Production code should split or accept the directory separately. Agents should not cargo-cult examples without validating API contracts.

---

<a id="src-10-examples-hello-avkon-md"></a>
## Example: Minimal Avkon Application

- Original corpus path: `10_examples/hello_avkon.md`
- Consolidated reference: `11-examples.md#src-10-examples-hello-avkon-md`

### Example: Minimal Avkon Application

#### MMP

```text
TARGET MyHello.exe
TARGETTYPE exe
UID 0x100039CE 0xE0001234
SECUREID 0xE0001234
CAPABILITY None

SOURCEPATH ..\src
SOURCE MyHelloApplication.cpp
SOURCE MyHelloSymbianDocument.cpp
SOURCE MyHelloSymbianAppUi.cpp
SOURCE MyHelloView.cpp

USERINCLUDE ..\inc
SYSTEMINCLUDE \epoc32\include

START RESOURCE ..\data\MyHello.rss
HEADER
TARGETPATH resource\apps
END

START RESOURCE ..\data\MyHello_reg.rss
TARGETPATH \private\10003a3f\import\apps
END

LIBRARY euser.lib apparc.lib cone.lib eikcore.lib avkon.lib
```

#### View drawing

```cpp
void CMyHelloView::Draw(const TRect& aRect) const
{
    CWindowGc& gc = SystemGc();
    gc.Clear(aRect);
    const CFont* font = CEikonEnv::Static()->TitleFont();
    gc.UseFont(font);
    const TInt baseline = (Rect().Height() + font->AscentInPixels()) / 2;
    gc.DrawText(_L("Hello Symbian 9.3"), Rect(), baseline,
                CGraphicsContext::ECenter);
    gc.DiscardFont();
}
```

#### Build

```bat
cd group
bldmake bldfiles
abld build winscw udeb
abld build gcce urel
```

---

<a id="src-10-examples-packaging-example-md"></a>
## Example: Complete PKG Template

- Original corpus path: `10_examples/packaging_example.md`
- Consolidated reference: `11-examples.md#src-10-examples-packaging-example-md`

### Example: Complete PKG Template

```text
; MyApp.pkg
#{"MyApp"},(0xE0001234),1,0,0

%{"Duy Pham"}
:"Duy Pham"

[0x101F7961], 0, 0, 0, {"S60ProductID"}

"$(EPOCROOT)Epoc32\release\gcce\urel\MyApp.exe" - "!:\sys\bin\MyApp.exe"
"$(EPOCROOT)Epoc32\data\z\resource\apps\MyApp.rsc" - "!:\resource\apps\MyApp.rsc"
"$(EPOCROOT)Epoc32\data\z\private\10003a3f\import\apps\MyApp_reg.rsc" - "!:\private\10003a3f\import\apps\MyApp_reg.rsc"
```

Commands:

```bat
makesis MyApp.pkg MyApp_unsigned.sis
signsis -s MyApp_unsigned.sis MyApp.sisx cert.cer key.key
```

Before packaging, confirm the generated files exist at the source paths.

---

<a id="src-10-examples-socket-http-patterns-md"></a>
## Example Patterns: Socket and HTTP State Machines

- Original corpus path: `10_examples/socket_http_patterns.md`
- Consolidated reference: `11-examples.md#src-10-examples-socket-http-patterns-md`

### Example Patterns: Socket and HTTP State Machines

#### Socket state machine

```text
EIdle -> EResolving -> EConnecting -> EWriting -> EReading -> EDone
                              \-> EFailed
```

Each state starts one asynchronous request and `RunL()` advances only after checking `iStatus.Int()`.

#### Buffer lifetime

Descriptors passed to async methods must remain alive until completion. Use members, not stack temporaries.

#### Cancellation

`DoCancel()` switches on current state and calls the matching cancel method. Then reset state in a controlled location.

#### HTTP callback state

```text
Submit transaction
  -> response headers event
  -> one or more body-data events
  -> response complete or failed
```

For each body-data event:

1. Obtain current data part.
2. Copy/process before it becomes invalid.
3. Release the data part.
4. Return quickly.

#### Retry policy

Retry only idempotent operations automatically, with a limit and backoff. On old mobile bearers, endless immediate retries are a battery-powered denial of service against yourself.

---

<a id="src-10-examples-timer-active-object-md"></a>
## Example: Timer Active Object

- Original corpus path: `10_examples/timer_active_object.md`
- Consolidated reference: `11-examples.md#src-10-examples-timer-active-object-md`

### Example: Timer Active Object

```cpp
class COneShotTimer : public CActive
{
public:
    static COneShotTimer* NewL(MTimerObserver& aObserver);
    ~COneShotTimer();
    void After(TTimeIntervalMicroSeconds32 aDelay);
private:
    COneShotTimer(MTimerObserver& aObserver)
    : CActive(EPriorityStandard), iObserver(aObserver) {}
    void ConstructL()
    {
        User::LeaveIfError(iTimer.CreateLocal());
        CActiveScheduler::Add(this);
    }
    void RunL()
    {
        if (iStatus.Int() == KErrNone)
            iObserver.TimerFired();
    }
    void DoCancel() { iTimer.Cancel(); }
private:
    RTimer iTimer;
    MTimerObserver& iObserver;
};

void COneShotTimer::After(TTimeIntervalMicroSeconds32 aDelay)
{
    Cancel();
    iTimer.After(iStatus, aDelay);
    SetActive();
}

COneShotTimer::~COneShotTimer()
{
    Cancel();
    iTimer.Close();
}
```
