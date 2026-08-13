#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "RgssBitmapBridge.h"
#include "RgssCoreBridge.h"
#include "RgssGraphicsBridge.h"
#include "RgssInputBridge.h"
#include "RgssPlaneBridge.h"
#include "RgssRenderBridge.h"
#include "RgssScriptArchiveBridge.h"
#include "RgssSpriteBridge.h"
#include "RgssTableBridge.h"
#include "RgssViewportBridge.h"
#include "RgssVmBridge.h"

namespace
{
struct TFiles
{
    const char** names;
    int count;
};

struct TGraphicsSync
{
    int calls;
    int frameRate;
    int frameCount;
};

struct TMemoryArchive
{
    const unsigned char* data;
    int size;
};

int MemoryArchiveRead(void* context, int offset, void* buffer, int bytes)
{
    TMemoryArchive* archive = static_cast<TMemoryArchive*>(context);
    if (!archive || offset < 0 || bytes < 0 || offset > archive->size ||
        bytes > archive->size - offset)
        return -1;
    memcpy(buffer, archive->data + offset, bytes);
    return bytes;
}

int GraphicsFrameSync(void* context, int frameRate, int frameCount)
{
    TGraphicsSync* sync = static_cast<TGraphicsSync*>(context);
    ++sync->calls;
    sync->frameRate = frameRate;
    sync->frameCount = frameCount;
    return 0;
}

int FileExists(void* context, const char* relativePath)
{
    TFiles* files = static_cast<TFiles*>(context);
    int index = 0;
    for (; index < files->count; ++index)
    {
        if (strcmp(files->names[index], relativePath) == 0)
            return 1;
    }
    return 0;
}

int VmFileSize(void*, const char*)
{
    return 0;
}

int VmFileRead(void*, const char*, int, void*, int)
{
    return 0;
}

unsigned int VmTicks(void*)
{
    return 0;
}

unsigned int VmInput(void*, int)
{
    return 0;
}

int VmBitmapLoad(void*, const char*, int* handle)
{
    if (handle)
        *handle = 0;
    return ERgssVmOk;
}

int VmFrameSync(void*, int, int)
{
    return ERgssVmOk;
}

void VmDirty(void*, int, int, int, int)
{
}

void* BitmapAllocate(void*, unsigned int bytes)
{
    return malloc(bytes);
}

void BitmapRelease(void*, void* memory)
{
    free(memory);
}
}

int main()
{
    EasyRpgRgssConfig config;
    assert(EasyRpgRgssDefaultConfig(ERgssVersion1, 320, 240, &config) == ERgssOk);
    assert(config.logical_width == 640);
    assert(config.logical_height == 480);
    assert(config.viewport_x == 0);
    assert(config.viewport_y == 0);
    assert(config.viewport_width == 320);
    assert(config.viewport_height == 240);
    assert(config.logic_fps == 40);
    assert(config.target_fps == 30);

    EasyRpgRgssCoreReset();
    assert(EasyRpgRgssCoreConfigure(&config) == ERgssOk);
    assert(EasyRpgRgssCoreVersion() == ERgssVersion1);
    assert(EasyRpgRgssCoreLogicFps() == 40);
    assert(EasyRpgRgssCoreTargetFps() == 30);

    TGraphicsSync graphicsSync = {0, 0, 0};
    EasyRpgRgssGraphicsHost graphicsHost;
    graphicsHost.context = &graphicsSync;
    graphicsHost.frame_sync = GraphicsFrameSync;
    graphicsHost.default_frame_rate = 40;
    assert(EasyRpgRgssGraphicsInitialize(&graphicsHost) == ERgssGraphicsOk);
    assert(EasyRpgRgssGraphicsFrameRate() == 40);
    assert(EasyRpgRgssGraphicsFrameCount() == 0);
    assert(EasyRpgRgssGraphicsUpdate() == ERgssGraphicsOk);
    assert(graphicsSync.calls == 1);
    assert(graphicsSync.frameRate == 40);
    assert(graphicsSync.frameCount == 1);
    assert(EasyRpgRgssGraphicsSetFrameRate(60) == ERgssGraphicsOk);
    assert(EasyRpgRgssGraphicsWait(2) == ERgssGraphicsOk);
    assert(graphicsSync.calls == 3);
    assert(graphicsSync.frameRate == 60);
    assert(graphicsSync.frameCount == 3);
    assert(EasyRpgRgssGraphicsSetFrameCount(120) == ERgssGraphicsOk);
    assert(EasyRpgRgssGraphicsFrameCount() == 120);
    assert(EasyRpgRgssGraphicsSetFrameRate(0) == ERgssGraphicsErrorInvalidArgument);
    EasyRpgRgssGraphicsShutdown();
    assert(EasyRpgRgssGraphicsUpdate() == ERgssGraphicsErrorNotInitialized);

    int outputX = -1;
    int outputY = -1;
    assert(EasyRpgRgssCoreMapPoint(200, 100, &outputX, &outputY) == ERgssOk);
    assert(outputX == 100);
    assert(outputY == 50);

    EasyRpgRgssInputReset();
    EasyRpgRgssInputSetMask(ERgssInputDecision);
    assert(EasyRpgRgssInputUpdate() == ERgssInputOk);
    assert(EasyRpgRgssInputPress(ERgssInputDecision) == 1);
    assert(EasyRpgRgssInputTrigger(ERgssInputDecision) == 1);
    assert(EasyRpgRgssInputRepeat(ERgssInputDecision) == 1);
    assert(EasyRpgRgssInputHeldFrames(ERgssInputDecision) == 1);
    for (int inputFrame = 0; inputFrame < 22; ++inputFrame)
        assert(EasyRpgRgssInputUpdate() == ERgssInputOk);
    assert(EasyRpgRgssInputHeldFrames(ERgssInputDecision) == 23);
    assert(EasyRpgRgssInputRepeat(ERgssInputDecision) == 0);
    assert(EasyRpgRgssInputUpdate() == ERgssInputOk);
    assert(EasyRpgRgssInputHeldFrames(ERgssInputDecision) == 24);
    assert(EasyRpgRgssInputRepeat(ERgssInputDecision) == 1);
    EasyRpgRgssInputSetMask(0);
    assert(EasyRpgRgssInputUpdate() == ERgssInputOk);
    assert(EasyRpgRgssInputPress(ERgssInputDecision) == 0);
    assert(EasyRpgRgssInputTrigger(ERgssInputDecision) == 0);
    assert(EasyRpgRgssInputHeldFrames(ERgssInputDecision) == 0);

    EasyRpgRgssRect sourceRect;
    sourceRect.x = 1;
    sourceRect.y = 1;
    sourceRect.width = 1;
    sourceRect.height = 1;
    EasyRpgRgssRect outputRect;
    assert(EasyRpgRgssCoreMapRect(&sourceRect, &outputRect) == ERgssOk);
    assert(outputRect.x == 0);
    assert(outputRect.y == 0);
    assert(outputRect.width == 1);
    assert(outputRect.height == 1);

    unsigned short framebuffer[320 * 240];
    for (int framebufferIndex = 0; framebufferIndex < 320 * 240; ++framebufferIndex)
        framebuffer[framebufferIndex] = 0;
    assert(EasyRpgRgssCoreBindFramebuffer(framebuffer,
                                           320,
                                           240,
                                           320 * 2,
                                           ERgssPixelRgb565) == ERgssOk);

    EasyRpgRgssBitmapHost bitmapHost;
    bitmapHost.context = 0;
    bitmapHost.allocate = BitmapAllocate;
    bitmapHost.release = BitmapRelease;
    bitmapHost.max_bitmaps = 8;
    assert(EasyRpgRgssBitmapInitialize(&bitmapHost) == ERgssBitmapOk);
    int bitmap = 0;
    assert(EasyRpgRgssBitmapCreate(4, 4, &bitmap) == ERgssBitmapOk);
    assert(EasyRpgRgssBitmapWidth(bitmap) == 4);
    assert(EasyRpgRgssBitmapHeight(bitmap) == 4);
    assert(EasyRpgRgssBitmapFillRect(bitmap, 0, 0, 4, 4, 255, 0, 0, 255) == ERgssBitmapOk);
    assert(EasyRpgRgssBitmapSetPixel(bitmap, 1, 1, 0, 255, 0, 128) == ERgssBitmapOk);
    int red = 0;
    int green = 0;
    int blue = 0;
    int alpha = 0;
    assert(EasyRpgRgssBitmapGetPixel(bitmap, 1, 1, &red, &green, &blue, &alpha) == ERgssBitmapOk);
    assert(red == 0);
    assert(green == 255);
    assert(blue == 0);
    assert(alpha == 128);
    assert(EasyRpgRgssBitmapFillRect(bitmap, 0, 0, 4, 4, 255, 0, 0, 255) == ERgssBitmapOk);
    EasyRpgRgssRect bitmapSource;
    bitmapSource.x = 0;
    bitmapSource.y = 0;
    bitmapSource.width = 4;
    bitmapSource.height = 4;
    EasyRpgRgssRect bitmapDirty;
    assert(EasyRpgRgssBitmapBlit(bitmap, &bitmapSource, 0, 0, 255, &bitmapDirty) == ERgssBitmapOk);
    assert(bitmapDirty.x == 0);
    assert(bitmapDirty.y == 0);
    assert(bitmapDirty.width == 2);
    assert(bitmapDirty.height == 2);
    assert(framebuffer[0] == 0xf800);
    assert(framebuffer[1] == 0xf800);
    assert(framebuffer[320] == 0xf800);

    int greenBitmap = 0;
    assert(EasyRpgRgssBitmapCreate(4, 4, &greenBitmap) == ERgssBitmapOk);
    assert(EasyRpgRgssBitmapFillRect(greenBitmap, 0, 0, 4, 4, 0, 255, 0, 255) == ERgssBitmapOk);
    int patternBitmap = 0;
    assert(EasyRpgRgssBitmapCreate(2, 1, &patternBitmap) == ERgssBitmapOk);
    assert(EasyRpgRgssBitmapFillRect(patternBitmap, 0, 0, 2, 1, 255, 0, 0, 255) == ERgssBitmapOk);
    assert(EasyRpgRgssBitmapSetPixel(patternBitmap, 1, 0, 0, 255, 0, 255) == ERgssBitmapOk);
    EasyRpgRgssRenderHost renderHost;
    renderHost.context = 0;
    renderHost.allocate = BitmapAllocate;
    renderHost.release = BitmapRelease;
    renderHost.max_renderables = 16;
    assert(EasyRpgRgssRenderInitialize(&renderHost) == ERgssRenderOk);

    EasyRpgRgssViewportHost viewportHost;
    viewportHost.context = 0;
    viewportHost.allocate = BitmapAllocate;
    viewportHost.release = BitmapRelease;
    viewportHost.max_viewports = 8;
    assert(EasyRpgRgssViewportInitialize(&viewportHost) == ERgssViewportOk);

    EasyRpgRgssPlaneHost planeHost;
    planeHost.context = 0;
    planeHost.allocate = BitmapAllocate;
    planeHost.release = BitmapRelease;
    planeHost.max_planes = 4;
    assert(EasyRpgRgssPlaneInitialize(&planeHost) == ERgssPlaneOk);

    EasyRpgRgssSpriteHost spriteHost;
    spriteHost.context = 0;
    spriteHost.allocate = BitmapAllocate;
    spriteHost.release = BitmapRelease;
    spriteHost.max_sprites = 8;
    assert(EasyRpgRgssSpriteInitialize(&spriteHost) == ERgssSpriteOk);
    int lowSprite = 0;
    int highSprite = 0;
    assert(EasyRpgRgssSpriteCreate(&lowSprite) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteCreate(&highSprite) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetBitmap(lowSprite, bitmap) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetBitmap(highSprite, greenBitmap) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetPosition(lowSprite, 0, 0, 0) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetPosition(highSprite, 0, 0, 10) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetOrigin(highSprite, 1, 1) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetZoom(highSprite, 2000, 2000) == ERgssSpriteOk);
    EasyRpgRgssRect spriteBounds;
    assert(EasyRpgRgssSpriteLogicalBounds(highSprite, &spriteBounds) == ERgssSpriteOk);
    assert(spriteBounds.x == -2);
    assert(spriteBounds.y == -2);
    assert(spriteBounds.width == 8);
    assert(spriteBounds.height == 8);
    assert(EasyRpgRgssSpriteSetOrigin(highSprite, 0, 0) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetZoom(highSprite, 1000, 1000) == ERgssSpriteOk);
    for (int spriteFramebufferIndex = 0; spriteFramebufferIndex < 320 * 240; ++spriteFramebufferIndex)
        framebuffer[spriteFramebufferIndex] = 0;
    EasyRpgRgssRect spriteDirty;
    assert(EasyRpgRgssSpriteRenderRange(-100, 100, &spriteDirty) == ERgssSpriteOk);
    assert(spriteDirty.x == 0);
    assert(spriteDirty.y == 0);
    assert(spriteDirty.width == 2);
    assert(spriteDirty.height == 2);
    assert(framebuffer[0] == 0x07e0);
    assert(EasyRpgRgssSpriteCount() == 2);
    assert(EasyRpgRgssRenderCount() == 2);
    assert(EasyRpgRgssSpriteDestroy(highSprite) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteDestroy(lowSprite) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteCount() == 0);

    EasyRpgRgssRect viewportRect;
    viewportRect.x = 4;
    viewportRect.y = 4;
    viewportRect.width = 4;
    viewportRect.height = 4;
    int viewport = 0;
    assert(EasyRpgRgssViewportCreate(&viewportRect, &viewport) == ERgssViewportOk);
    const unsigned int viewportRevision = EasyRpgRgssViewportRevision();
    assert(EasyRpgRgssViewportSetOrigin(viewport, 1, 1) == ERgssViewportOk);
    assert(EasyRpgRgssViewportSetZ(viewport, 100) == ERgssViewportOk);
    assert(EasyRpgRgssViewportRevision() != viewportRevision);

    int viewportSprite = 0;
    int rootSprite = 0;
    assert(EasyRpgRgssSpriteCreate(&viewportSprite) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteCreate(&rootSprite) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetBitmap(viewportSprite, greenBitmap) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetViewport(viewportSprite, viewport) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetPosition(viewportSprite, 0, 0, 0) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetBitmap(rootSprite, bitmap) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetPosition(rootSprite, 4, 4, 50) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteLogicalBounds(viewportSprite, &spriteBounds) == ERgssSpriteOk);
    assert(spriteBounds.x == 3);
    assert(spriteBounds.y == 3);
    assert(spriteBounds.width == 4);
    assert(spriteBounds.height == 4);

    for (int viewportFramebufferIndex = 0; viewportFramebufferIndex < 320 * 240; ++viewportFramebufferIndex)
        framebuffer[viewportFramebufferIndex] = 0;
    assert(EasyRpgRgssRenderAll(&spriteDirty) == ERgssRenderOk);
    assert(spriteDirty.x == 2);
    assert(spriteDirty.y == 2);
    assert(spriteDirty.width == 2);
    assert(spriteDirty.height == 2);
    assert(framebuffer[2 + 2 * 320] == 0x07e0);
    assert(framebuffer[1 + 2 * 320] == 0);
    assert(framebuffer[2 + 1 * 320] == 0);

    assert(EasyRpgRgssViewportSetVisible(viewport, 0) == ERgssViewportOk);
    for (int hiddenFramebufferIndex = 0; hiddenFramebufferIndex < 320 * 240; ++hiddenFramebufferIndex)
        framebuffer[hiddenFramebufferIndex] = 0;
    assert(EasyRpgRgssRenderAll(&spriteDirty) == ERgssRenderOk);
    assert(framebuffer[2 + 2 * 320] == 0xf800);
    assert(EasyRpgRgssViewportSetVisible(viewport, 1) == ERgssViewportOk);

    assert(EasyRpgRgssSpriteDestroy(rootSprite) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteDestroy(viewportSprite) == ERgssSpriteOk);
    assert(EasyRpgRgssViewportDestroy(viewport) == ERgssViewportOk);
    assert(EasyRpgRgssSpriteSetViewport(viewportSprite, viewport) == ERgssSpriteErrorInvalidHandle);
    assert(EasyRpgRgssViewportGetState(viewport, 0) == ERgssViewportErrorInvalidHandle);

    int plane = 0;
    assert(EasyRpgRgssPlaneCreate(&plane) == ERgssPlaneOk);
    assert(EasyRpgRgssPlaneSetBitmap(plane, patternBitmap) == ERgssPlaneOk);
    assert(EasyRpgRgssPlaneSetZoom(plane, 2000, 1000) == ERgssPlaneOk);
    assert(EasyRpgRgssPlaneSetZ(plane, -10) == ERgssPlaneOk);
    for (int planeFramebufferIndex = 0; planeFramebufferIndex < 320 * 240; ++planeFramebufferIndex)
        framebuffer[planeFramebufferIndex] = 0;
    assert(EasyRpgRgssRenderAll(&spriteDirty) == ERgssRenderOk);
    assert(spriteDirty.x == 0);
    assert(spriteDirty.y == 0);
    assert(spriteDirty.width == 320);
    assert(spriteDirty.height == 240);
    assert(framebuffer[10 * 320] == 0xf800);
    assert(framebuffer[1 + 10 * 320] == 0x07e0);
    assert(EasyRpgRgssPlaneSetOrigin(plane, 2, 0) == ERgssPlaneOk);
    for (int scrolledPlaneIndex = 0; scrolledPlaneIndex < 320 * 240; ++scrolledPlaneIndex)
        framebuffer[scrolledPlaneIndex] = 0;
    assert(EasyRpgRgssRenderAll(&spriteDirty) == ERgssRenderOk);
    assert(framebuffer[10 * 320] == 0x07e0);
    assert(framebuffer[1 + 10 * 320] == 0xf800);
    assert(EasyRpgRgssPlaneSetOrigin(plane, 0, 0) == ERgssPlaneOk);

    int overlaySprite = 0;
    assert(EasyRpgRgssSpriteCreate(&overlaySprite) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetBitmap(overlaySprite, greenBitmap) == ERgssSpriteOk);
    assert(EasyRpgRgssSpriteSetPosition(overlaySprite, 0, 0, 0) == ERgssSpriteOk);
    for (int planeBelowIndex = 0; planeBelowIndex < 320 * 240; ++planeBelowIndex)
        framebuffer[planeBelowIndex] = 0;
    assert(EasyRpgRgssRenderAll(&spriteDirty) == ERgssRenderOk);
    assert(framebuffer[0] == 0x07e0);
    assert(EasyRpgRgssPlaneSetZ(plane, 10) == ERgssPlaneOk);
    for (int planeAboveIndex = 0; planeAboveIndex < 320 * 240; ++planeAboveIndex)
        framebuffer[planeAboveIndex] = 0;
    assert(EasyRpgRgssRenderAll(&spriteDirty) == ERgssRenderOk);
    assert(framebuffer[0] == 0xf800);
    assert(EasyRpgRgssSpriteDestroy(overlaySprite) == ERgssSpriteOk);
    assert(EasyRpgRgssPlaneDestroy(plane) == ERgssPlaneOk);
    assert(EasyRpgRgssPlaneCount() == 0);

    EasyRpgRgssSpriteShutdown();
    EasyRpgRgssPlaneShutdown();
    EasyRpgRgssViewportShutdown();
    assert(EasyRpgRgssRenderCount() == 0);
    EasyRpgRgssRenderShutdown();

    EasyRpgRgssTableHost tableHost;
    tableHost.context = 0;
    tableHost.allocate = BitmapAllocate;
    tableHost.release = BitmapRelease;
    tableHost.max_tables = 8;
    assert(EasyRpgRgssTableInitialize(&tableHost) == ERgssTableOk);
    int table = 0;
    assert(EasyRpgRgssTableCreate(2, 2, 2, 1, &table) == ERgssTableOk);
    assert(EasyRpgRgssTableDimensions(table) == 2);
    assert(EasyRpgRgssTableXSize(table) == 2);
    assert(EasyRpgRgssTableYSize(table) == 2);
    assert(EasyRpgRgssTableZSize(table) == 1);
    assert(EasyRpgRgssTableSet(table, 1, 1, 0, 1234) == ERgssTableOk);
    int tableValue = 0;
    assert(EasyRpgRgssTableGet(table, 1, 1, 0, &tableValue) == ERgssTableOk);
    assert(tableValue == 1234);
    assert(EasyRpgRgssTableResize(table, 3, 3, 3, 2) == ERgssTableOk);
    assert(EasyRpgRgssTableDimensions(table) == 3);
    assert(EasyRpgRgssTableGet(table, 1, 1, 0, &tableValue) == ERgssTableOk);
    assert(tableValue == 1234);
    assert(EasyRpgRgssTableGet(table, 2, 2, 1, &tableValue) == ERgssTableOk);
    assert(tableValue == 0);
    short* tableData = 0;
    int tableCount = 0;
    assert(EasyRpgRgssTableLock(table, &tableData, &tableCount) == ERgssTableOk);
    assert(tableData != 0);
    assert(tableCount == 18);
    assert(EasyRpgRgssTableGet(table, 3, 0, 0, &tableValue) == ERgssTableErrorOutOfBounds);
    assert(EasyRpgRgssTableCount() == 1);
    assert(EasyRpgRgssTableDestroy(table) == ERgssTableOk);
    assert(EasyRpgRgssTableXSize(table) == ERgssTableErrorInvalidHandle);
    EasyRpgRgssTableShutdown();

    assert(EasyRpgRgssBitmapDestroy(patternBitmap) == ERgssBitmapOk);
    assert(EasyRpgRgssBitmapDestroy(greenBitmap) == ERgssBitmapOk);
    assert(EasyRpgRgssBitmapDestroy(bitmap) == ERgssBitmapOk);
    assert(EasyRpgRgssBitmapWidth(bitmap) == ERgssBitmapErrorInvalidHandle);
    EasyRpgRgssBitmapShutdown();

    assert(EasyRpgRgssDefaultConfig(ERgssVersion2, 320, 240, &config) == ERgssOk);
    assert(config.logical_width == 544);
    assert(config.logical_height == 416);
    assert(config.viewport_x == 3);
    assert(config.viewport_y == 0);
    assert(config.viewport_width == 314);
    assert(config.viewport_height == 240);
    assert(config.logic_fps == 60);
    assert(config.target_fps == 30);

    const char* xpNames[] = {"Game.ini", "Data/Scripts.rxdata"};
    TFiles xpFiles = {xpNames, 2};
    assert(EasyRpgRgssDetectProject(FileExists, &xpFiles) == ERgssVersion1);

    const char* vxNames[] = {"Game.ini", "Data/Scripts.rvdata"};
    TFiles vxFiles = {vxNames, 2};
    assert(EasyRpgRgssDetectProject(FileExists, &vxFiles) == ERgssVersion2);

    const char* aceNames[] = {"Game.ini", "Data/Scripts.rvdata2"};
    TFiles aceFiles = {aceNames, 2};
    assert(EasyRpgRgssDetectProject(FileExists, &aceFiles) == ERgssVersion3);

    const char* ambiguousNames[] = {"Data/Scripts.rxdata", "Data/Scripts.rvdata"};
    TFiles ambiguousFiles = {ambiguousNames, 2};
    assert(EasyRpgRgssDetectProject(FileExists, &ambiguousFiles) == ERgssErrorAmbiguousProject);

    TFiles emptyFiles = {0, 0};
    assert(EasyRpgRgssDetectProject(FileExists, &emptyFiles) == ERgssErrorProjectNotFound);

    const unsigned char xpScriptArchive[] = {
        0x04, 0x08, '[', 0x06, '[', 0x08,
        'i', 0x01, 0x7b,
        '"', 0x09, 'T', 'e', 's', 't',
        '"', 0x08, 0x01, 0x02, 0x03
    };
    TMemoryArchive xpArchive = {xpScriptArchive, static_cast<int>(sizeof(xpScriptArchive))};
    EasyRpgRgssScriptArchiveReader archiveReader;
    archiveReader.context = &xpArchive;
    archiveReader.size = xpArchive.size;
    archiveReader.read = MemoryArchiveRead;
    int scriptCount = 0;
    assert(EasyRpgRgssScriptArchiveInspect(&archiveReader, 0, 0, &scriptCount) ==
           ERgssScriptArchiveErrorCapacity);
    assert(scriptCount == 1);
    assert(EasyRpgRgssScriptArchiveValidate(&archiveReader, &scriptCount) ==
           ERgssScriptArchiveOk);
    assert(scriptCount == 1);
    EasyRpgRgssScriptEntry scriptEntry;
    assert(EasyRpgRgssScriptArchiveInspect(&archiveReader, &scriptEntry, 1, &scriptCount) ==
           ERgssScriptArchiveOk);
    assert(scriptEntry.id == 123);
    assert(scriptEntry.name_length == 4);
    assert(memcmp(xpScriptArchive + scriptEntry.name_offset, "Test", 4) == 0);
    assert(scriptEntry.data_length == 3);
    assert(xpScriptArchive[scriptEntry.data_offset] == 0x01);
    assert(xpScriptArchive[scriptEntry.data_offset + 2] == 0x03);

    const unsigned char aceScriptArchive[] = {
        0x04, 0x08, '[', 0x06, '[', 0x08,
        'i', 0x06,
        'I', '"', 0x08, 'A', 'c', 'e', 0x06, ':', 0x06, 'E', 'T',
        'I', '"', 0x07, 0xaa, 0xbb, 0x06, ';', 0x00, 'F'
    };
    TMemoryArchive aceArchive = {aceScriptArchive, static_cast<int>(sizeof(aceScriptArchive))};
    archiveReader.context = &aceArchive;
    archiveReader.size = aceArchive.size;
    assert(EasyRpgRgssScriptArchiveInspect(&archiveReader, &scriptEntry, 1, &scriptCount) ==
           ERgssScriptArchiveOk);
    assert(scriptCount == 1);
    assert(scriptEntry.id == 1);
    assert(scriptEntry.name_length == 3);
    assert(memcmp(aceScriptArchive + scriptEntry.name_offset, "Ace", 3) == 0);
    assert(scriptEntry.data_length == 2);
    assert(aceScriptArchive[scriptEntry.data_offset] == 0xaa);
    assert(aceScriptArchive[scriptEntry.data_offset + 1] == 0xbb);

    archiveReader.context = &xpArchive;
    archiveReader.size = xpArchive.size - 1;
    assert(EasyRpgRgssScriptArchiveInspect(&archiveReader, &scriptEntry, 1, &scriptCount) ==
           ERgssScriptArchiveErrorCorrupt);

    EasyRpgRgssVmHost host;
    host.context = 0;
    host.file_size = VmFileSize;
    host.file_read = VmFileRead;
    host.ticks = VmTicks;
    host.input = VmInput;
    host.bitmap_load = VmBitmapLoad;
    host.frame_sync = VmFrameSync;
    host.dirty = VmDirty;
    assert(EasyRpgRgssVmAvailable() == 0);
    assert(EasyRpgRgssVmInitialize(ERgssVersion1, &host) == ERgssVmErrorUnavailable);
    assert(EasyRpgRgssVmLoadScripts("Data/Scripts.rxdata") == ERgssVmErrorNotInitialized);
    assert(EasyRpgRgssVmTick() == ERgssVmErrorNotInitialized);
    assert(EasyRpgRgssVmCollect() == ERgssVmErrorNotInitialized);
    assert(EasyRpgRgssVmHeapBytes() == ERgssVmErrorNotInitialized);
    EasyRpgRgssVmShutdown();

    return 0;
}
