#include "RgssRuntime.h"
#include "AudioBridge.h"
#include "DirentBridge.h"
#include "RgssBitmapBridge.h"
#include "RgssCoreBridge.h"
#include "RgssDisplaySurface.h"
#include "RgssInputBridge.h"
#include "RgssPlaneBridge.h"
#include "RgssRenderBridge.h"
#include "RgssSpriteBridge.h"
#include "RgssTableBridge.h"
#include "RgssTilemapBridge.h"
#include "RgssViewportBridge.h"
#include "RgssVmWorker.h"
#include "ZipVfs.h"

CRgssRuntime* CRgssRuntime::NewL(const TDesC& aGamePath)
{
    CRgssRuntime* self = new (ELeave) CRgssRuntime;
    CleanupStack::PushL(self);
    self->ConstructL(aGamePath);
    CleanupStack::Pop(self);
    return self;
}

CRgssRuntime::CRgssRuntime()
    : iAllocator(&User::Allocator()),
      iResources(NULL),
      iSurface(NULL),
      iVmWorker(NULL),
      iVersion(ERgssVersionUnknown),
      iLogicFps(0),
      iTargetFps(0),
      iVmStarted(EFalse)
{
    Mem::FillZ(iInputWords, sizeof(iInputWords));
}

void CRgssRuntime::ConstructL(const TDesC& aGamePath)
{
    if (aGamePath.Length() <= 0 || aGamePath.Length() > iGamePath.MaxLength())
        User::Leave(KErrArgument);
    iGamePath.Copy(aGamePath);

    User::LeaveIfError(iFs.Connect());
    iResources = CZipVfs::OpenL(iFs, iGamePath);
    _LIT(KRgssVirtualRoot, "C:\\Data\\EasyRPG\\RGSS\\");
    EasyRpgSetVirtualProject(iResources, KRgssVirtualRoot);
    iVersion = DetectVersion();
    if (iVersion == ERgssVersionUnknown)
        User::Leave(KErrNotSupported);

    EasyRpgRgssConfig config;
    if (EasyRpgRgssDefaultConfig(iVersion, 320, 240, &config) != ERgssOk)
        User::Leave(KErrArgument);
    if (EasyRpgRgssCoreConfigure(&config) != ERgssOk)
        User::Leave(KErrGeneral);
    iLogicFps = config.logic_fps;
    iTargetFps = config.target_fps;
    EasyRpgRgssInputReset();

    EasyRpgRgssBitmapHost bitmapHost;
    bitmapHost.context = this;
    bitmapHost.allocate = &CRgssRuntime::BitmapAllocate;
    bitmapHost.release = &CRgssRuntime::BitmapRelease;
    bitmapHost.glyph = NULL;
    bitmapHost.max_bitmaps = 512;
    const TInt bitmapResult = EasyRpgRgssBitmapInitialize(&bitmapHost);
    if (bitmapResult != ERgssBitmapOk)
        User::Leave(bitmapResult == ERgssBitmapErrorNoMemory ? KErrNoMemory : KErrGeneral);

    EasyRpgRgssRenderHost renderHost;
    renderHost.context = this;
    renderHost.allocate = &CRgssRuntime::BitmapAllocate;
    renderHost.release = &CRgssRuntime::BitmapRelease;
    renderHost.max_renderables = 768;
    const TInt renderRegistryResult = EasyRpgRgssRenderInitialize(&renderHost);
    if (renderRegistryResult != ERgssRenderOk)
    {
        EasyRpgRgssBitmapShutdown();
        User::Leave(renderRegistryResult == ERgssRenderErrorNoMemory ? KErrNoMemory : KErrGeneral);
    }

    EasyRpgRgssViewportHost viewportHost;
    viewportHost.context = this;
    viewportHost.allocate = &CRgssRuntime::BitmapAllocate;
    viewportHost.release = &CRgssRuntime::BitmapRelease;
    viewportHost.max_viewports = 128;
    const TInt viewportResult = EasyRpgRgssViewportInitialize(&viewportHost);
    if (viewportResult != ERgssViewportOk)
    {
        EasyRpgRgssRenderShutdown();
        EasyRpgRgssBitmapShutdown();
        User::Leave(viewportResult == ERgssViewportErrorNoMemory ? KErrNoMemory : KErrGeneral);
    }

    EasyRpgRgssPlaneHost planeHost;
    planeHost.context = this;
    planeHost.allocate = &CRgssRuntime::BitmapAllocate;
    planeHost.release = &CRgssRuntime::BitmapRelease;
    planeHost.max_planes = 64;
    const TInt planeResult = EasyRpgRgssPlaneInitialize(&planeHost);
    if (planeResult != ERgssPlaneOk)
    {
        EasyRpgRgssViewportShutdown();
        EasyRpgRgssRenderShutdown();
        EasyRpgRgssBitmapShutdown();
        User::Leave(planeResult == ERgssPlaneErrorNoMemory ? KErrNoMemory : KErrGeneral);
    }

    EasyRpgRgssSpriteHost spriteHost;
    spriteHost.context = this;
    spriteHost.allocate = &CRgssRuntime::BitmapAllocate;
    spriteHost.release = &CRgssRuntime::BitmapRelease;
    spriteHost.max_sprites = 512;
    const TInt spriteResult = EasyRpgRgssSpriteInitialize(&spriteHost);
    if (spriteResult != ERgssSpriteOk)
    {
        EasyRpgRgssPlaneShutdown();
        EasyRpgRgssViewportShutdown();
        EasyRpgRgssRenderShutdown();
        EasyRpgRgssBitmapShutdown();
        User::Leave(spriteResult == ERgssSpriteErrorNoMemory ? KErrNoMemory : KErrGeneral);
    }

    EasyRpgRgssTableHost tableHost;
    tableHost.context = this;
    tableHost.allocate = &CRgssRuntime::BitmapAllocate;
    tableHost.release = &CRgssRuntime::BitmapRelease;
    tableHost.max_tables = 2048;
    const TInt tableResult = EasyRpgRgssTableInitialize(&tableHost);
    if (tableResult != ERgssTableOk)
    {
        EasyRpgRgssSpriteShutdown();
        EasyRpgRgssPlaneShutdown();
        EasyRpgRgssViewportShutdown();
        EasyRpgRgssRenderShutdown();
        EasyRpgRgssBitmapShutdown();
        User::Leave(tableResult == ERgssTableErrorNoMemory ? KErrNoMemory : KErrGeneral);
    }

    EasyRpgRgssTilemapHost tilemapHost;
    tilemapHost.context = this;
    tilemapHost.allocate = &CRgssRuntime::BitmapAllocate;
    tilemapHost.release = &CRgssRuntime::BitmapRelease;
    tilemapHost.max_tilemaps = 8;
    const TInt tilemapResult = EasyRpgRgssTilemapInitialize(&tilemapHost);
    if (tilemapResult != ERgssTilemapOk)
    {
        EasyRpgRgssTableShutdown();
        EasyRpgRgssSpriteShutdown();
        EasyRpgRgssPlaneShutdown();
        EasyRpgRgssViewportShutdown();
        EasyRpgRgssRenderShutdown();
        EasyRpgRgssBitmapShutdown();
        User::Leave(tilemapResult == ERgssTilemapErrorNoMemory ? KErrNoMemory : KErrGeneral);
    }

    iSurface = CRgssDisplaySurface::NewL(TSize(config.output_width, config.output_height));
}

CRgssRuntime::~CRgssRuntime()
{
    if (iVmWorker)
        iVmWorker->Stop();
    delete iVmWorker;
    iVmWorker = NULL;
    iVmStarted = EFalse;
    EasyRpgAudioShutdown();
    EasyRpgClearVirtualProject();
    EasyRpgRgssTilemapShutdown();
    EasyRpgRgssTableShutdown();
    EasyRpgRgssSpriteShutdown();
    EasyRpgRgssPlaneShutdown();
    EasyRpgRgssViewportShutdown();
    EasyRpgRgssRenderShutdown();
    EasyRpgRgssBitmapShutdown();
    EasyRpgRgssCoreReset();
    delete iSurface;
    iSurface = NULL;
    delete iResources;
    iResources = NULL;
    iFs.Close();
}

TInt CRgssRuntime::DetectVersion() const
{
    if (!iResources)
        return ERgssVersionUnknown;

    const TBool xp = iResources->Exists(_L("Data/Scripts.rxdata")) ||
                     iResources->Exists(_L("Game.rgssad"));
    const TBool vx = iResources->Exists(_L("Data/Scripts.rvdata")) ||
                     iResources->Exists(_L("Game.rgss2a"));
    const TBool vxa = iResources->Exists(_L("Data/Scripts.rvdata2")) ||
                      iResources->Exists(_L("Game.rgss3a"));
    const TInt matches = static_cast<TInt>(xp) +
                         static_cast<TInt>(vx) +
                         static_cast<TInt>(vxa);
    if (matches != 1)
        return ERgssVersionUnknown;
    if (xp)
        return ERgssVersion1;
    if (vx)
        return ERgssVersion2;
    return ERgssVersion3;
}

TInt CRgssRuntime::Version() const
{
    return iVersion;
}

TInt CRgssRuntime::LogicFps() const
{
    return iLogicFps;
}

TInt CRgssRuntime::TargetFps() const
{
    return iTargetFps;
}

void CRgssRuntime::StartVmL()
{
    if (iVmStarted || iVmWorker)
        User::Leave(KErrAlreadyExists);

    iVmWorker = CRgssVmWorker::NewL(iGamePath, iVersion);
    TRAPD(error, iVmWorker->StartL());
    if (error != KErrNone)
    {
        delete iVmWorker;
        iVmWorker = NULL;
        User::Leave(error);
    }

    for (TInt index = 0; index < 4; ++index)
        iVmWorker->SetInputWord(index, iInputWords[index]);
    iVmStarted = ETrue;
}

TBool CRgssRuntime::AdvanceVmFrameL(TBool aCompose, TUint32 aInputWord0)
{
    if (!iVmStarted || !iVmWorker)
        User::Leave(KErrNotReady);

    TInt frameRate = 0;
    TInt frameCount = 0;
    if (!iVmWorker->TakeFrame(frameRate, frameCount))
    {
        if (!iVmWorker->Running() && iVmWorker->ExitResult() != KRequestPending)
        {
            const TInt result = iVmWorker->ExitResult();
            User::Leave(result == KErrNone ? KErrEof : result);
        }
        return EFalse;
    }

    if (frameRate > 0)
        iLogicFps = Min(frameRate, 60);

    // The VM is blocked inside Graphics.update at this point. Publish the
    // input state immediately before resuming it so one-shot Symbian key
    // events cannot be lost between window-server callbacks and Input.update.
    SetInputWord(0, aInputWord0);

    ProcessAudioCommands();
    EasyRpgAudioUpdate();

    TInt composeError = KErrNone;
    if (aCompose)
        TRAP(composeError, ComposeL());
    iVmWorker->ResumeFrame();
    User::LeaveIfError(composeError);
    return ETrue;
}

void CRgssRuntime::ComposeL()
{
    if (!iSurface)
        User::Leave(KErrNotReady);

    TUint16* pixels = iSurface->BeginRenderL();
    Mem::FillZ(pixels, iSurface->StrideBytes() * iSurface->Size().iHeight);
    const TInt bindResult = EasyRpgRgssCoreBindFramebuffer(pixels,
                                                           iSurface->Size().iWidth,
                                                           iSurface->Size().iHeight,
                                                           iSurface->StrideBytes(),
                                                           ERgssPixelRgb565);
    if (bindResult != ERgssOk)
    {
        iSurface->EndRender();
        User::Leave(KErrGeneral);
    }

    EasyRpgRgssRect dirty;
    const TInt renderResult = EasyRpgRgssRenderAll(&dirty);
    iSurface->EndRender();
    if (renderResult != ERgssRenderOk)
        User::Leave(KErrGeneral);
    iSurface->MarkFullDirty();
}

void CRgssRuntime::ProcessAudioCommands()
{
    if (!iVmWorker)
        return;
    TRgssAudioCommand command;
    while (iVmWorker->TakeAudioCommand(command))
    {
        TBuf8<320> path;
        const char* audioPath = NULL;
        if (command.iPath.Length() > 0)
        {
            _LIT8(KRgssVirtualRoot8, "C:\\Data\\EasyRPG\\RGSS\\");
            if (KRgssVirtualRoot8().Length() + command.iPath.Length() + 1 <= path.MaxLength())
            {
                path.Copy(KRgssVirtualRoot8);
                path.Append(command.iPath);
                path.Append(0);
                audioPath = reinterpret_cast<const char*>(path.Ptr());
            }
        }
        switch (command.iCommand)
        {
        case ERgssAudioBgmPlay:
            EasyRpgAudioPlayBgm(audioPath, command.iVolume, command.iPitch, 0);
            break;
        case ERgssAudioBgmStop:
            EasyRpgAudioStopBgm();
            break;
        case ERgssAudioBgmFade:
            EasyRpgAudioFadeBgm(command.iDuration);
            break;
        case ERgssAudioBgsPlay:
            EasyRpgAudioPlayBgs(audioPath, command.iVolume, command.iPitch);
            break;
        case ERgssAudioBgsStop:
            EasyRpgAudioStopBgs();
            break;
        case ERgssAudioBgsFade:
            EasyRpgAudioFadeBgs(command.iDuration);
            break;
        case ERgssAudioMePlay:
            EasyRpgAudioPlayMe(audioPath, command.iVolume, command.iPitch);
            break;
        case ERgssAudioMeStop:
            EasyRpgAudioStopMe();
            break;
        case ERgssAudioMeFade:
            EasyRpgAudioFadeMe(command.iDuration);
            break;
        case ERgssAudioSePlay:
            EasyRpgAudioPlaySe(audioPath, command.iVolume, command.iPitch);
            break;
        case ERgssAudioSeStop:
            EasyRpgAudioStopSe();
            break;
        default:
            break;
        }
    }
}

void CRgssRuntime::SetInputWord(TInt aWord, TUint32 aValue)
{
    if (aWord < 0 || aWord >= 4)
        return;
    iInputWords[aWord] = aValue;
    if (iVmWorker)
        iVmWorker->SetInputWord(aWord, aValue);
    if (aWord == 0)
        EasyRpgRgssInputSetMask(aValue);
}

const CRgssDisplaySurface& CRgssRuntime::Surface() const
{
    return *iSurface;
}

CRgssDisplaySurface& CRgssRuntime::Surface()
{
    return *iSurface;
}

void* CRgssRuntime::BitmapAllocate(void* aContext, unsigned int aBytes)
{
    if (!aContext || aBytes > 0x7fffffffu)
        return NULL;
    CRgssRuntime* self = static_cast<CRgssRuntime*>(aContext);
    return self->iAllocator->Alloc(static_cast<TInt>(aBytes));
}

void CRgssRuntime::BitmapRelease(void* aContext, void* aMemory)
{
    if (!aContext || !aMemory)
        return;
    CRgssRuntime* self = static_cast<CRgssRuntime*>(aContext);
    self->iAllocator->Free(aMemory);
}
