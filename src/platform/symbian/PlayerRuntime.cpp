#include "easyrpgsymbian/PlayerRuntime.h"
#include "easyrpgsymbian/Encoding.h"
#include "easyrpgsymbian/Framebuffer.h"
#include "easyrpgsymbian/LcfProbe.h"
#include "easyrpgsymbian/ModernCoreProbe.h"
#include "easyrpgsymbian/XyzImage.h"

namespace
{
TUint16 Rgb565(TInt aRed, TInt aGreen, TInt aBlue)
{
    return static_cast<TUint16>(((aRed & 0xf8) << 8) |
                               ((aGreen & 0xfc) << 3) |
                               ((aBlue & 0xf8) >> 3));
}

TInt Clamp(TInt aValue, TInt aMinimum, TInt aMaximum)
{
    if (aValue < aMinimum)
        return aMinimum;
    if (aValue > aMaximum)
        return aMaximum;
    return aValue;
}
}

CPlayerRuntime* CPlayerRuntime::NewL(RFs& aFs,
                                    const TDesC& aGameRoot,
                                    CFrameBuffer& aFrameBuffer)
{
    CPlayerRuntime* self = new (ELeave) CPlayerRuntime(aFs, aFrameBuffer);
    CleanupStack::PushL(self);
    self->ConstructL(aGameRoot);
    CleanupStack::Pop(self);
    return self;
}

CPlayerRuntime::CPlayerRuntime(RFs& aFs, CFrameBuffer& aFrameBuffer)
    : iFs(aFs),
      iFrameBuffer(aFrameBuffer),
      iPressedKeys(0),
      iLcfReady(EFalse),
      iTitleReady(EFalse),
      iMarkerX(160),
      iMarkerY(120)
{
}

void CPlayerRuntime::ConstructL(const TDesC& aGameRoot)
{
    iGameRoot.Copy(aGameRoot.Left(iGameRoot.MaxLength()));
    iGameTitle.Zero();

    if (iGameRoot.Length() > 0)
    {
        TLcfProjectSummary projectSummary;
        const TInt probeError = TLcfProbe::ProbeProject(iGameRoot, iGameTitle, projectSummary);
        iLcfReady = (probeError == KErrNone);
        if (iLcfReady)
        {
            const TInt modernCoreResult = EasyRpgModernCoreProbe(5);
            if (modernCoreResult == 33)
            {
                TRAPD(titleError,
                    TBuf<96> titleAssetName;
                    TEncoding::ShiftJisToUnicodeL(iFs,
                                                  projectSummary.iTitleNameSjis,
                                                  titleAssetName);
                    CXyzImage* title = CXyzImage::LoadNamedL(iFs,
                                                            iGameRoot,
                                                            _L("Title"),
                                                            titleAssetName);
                    CleanupStack::PushL(title);
                    const TSize titleSize = title->Size();
                    const TPoint titleOrigin((iFrameBuffer.Size().iWidth - titleSize.iWidth) / 2,
                                             (iFrameBuffer.Size().iHeight - titleSize.iHeight) / 2);
                    title->BlitOpaque565L(iFrameBuffer, titleOrigin);
                    CleanupStack::PopAndDestroy(title);
                );
                iTitleReady = (titleError == KErrNone);

                if (iTitleReady)
                {
                    iStatusText.Format(_L("M%u %ux%u C%u E%u XYZ OK"),
                                       projectSummary.iStartMapId,
                                       projectSummary.iMapWidth,
                                       projectSummary.iMapHeight,
                                       projectSummary.iMapChipsetId,
                                       projectSummary.iMapEventCount);
                }
                else
                {
                    iStatusText.Format(_L("LCF OK, XYZ failed: %d"), titleError);
                }
            }
            else
            {
                iStatusText.Format(_L("C++11 ARM ABI failed: %d"), modernCoreResult);
            }
        }
        else
        {
            iStatusText.Format(_L("LCF probe failed: %d"), probeError);
        }
    }
    else
    {
        iLcfReady = EFalse;
        iStatusText.Copy(_L("Game data not found"));
    }

    if (!iTitleReady)
        RenderDiagnosticL(0);
}

CPlayerRuntime::~CPlayerRuntime()
{
}

void CPlayerRuntime::SetKeyState(TUint32 aKey, TBool aPressed)
{
    if (aPressed)
        iPressedKeys |= aKey;
    else
        iPressedKeys &= ~aKey;
}

void CPlayerRuntime::ReleaseAllKeys()
{
    iPressedKeys = 0;
}

void CPlayerRuntime::TickL(TUint32 aLogicFrame, TBool aRenderFrame)
{
    if (iPressedKeys & EKeyLeft)
        --iMarkerX;
    if (iPressedKeys & EKeyRight)
        ++iMarkerX;
    if (iPressedKeys & EKeyUp)
        --iMarkerY;
    if (iPressedKeys & EKeyDown)
        ++iMarkerY;

    iMarkerX = Clamp(iMarkerX, 3, iFrameBuffer.Size().iWidth - 4);
    iMarkerY = Clamp(iMarkerY, 3, iFrameBuffer.Size().iHeight - 4);

    if (aRenderFrame && !iTitleReady)
        RenderDiagnosticL(aLogicFrame);
}

const TDesC& CPlayerRuntime::StatusText() const
{
    return iStatusText;
}

const TDesC& CPlayerRuntime::GameTitle() const
{
    return iGameTitle;
}

const TDesC& CPlayerRuntime::GameRoot() const
{
    return iGameRoot;
}

TUint32 CPlayerRuntime::PressedKeys() const
{
    return iPressedKeys;
}

void CPlayerRuntime::RenderDiagnosticL(TUint32 aLogicFrame)
{
    TInt stride = 0;
    TUint16* pixels = iFrameBuffer.LockPixelsL(stride);
    const TSize size = iFrameBuffer.Size();

    const TUint16 darkA = Rgb565(12, 16, 24);
    const TUint16 darkB = Rgb565(17, 23, 33);
    const TUint16 line = Rgb565(43, 58, 75);
    const TUint16 ready = iLcfReady ? Rgb565(42, 196, 112) : Rgb565(215, 72, 72);
    const TUint16 marker = (iPressedKeys & EKeyDecision) ? Rgb565(255, 220, 70) : Rgb565(225, 232, 240);

    TInt y = 0;
    for (y = 0; y < size.iHeight; ++y)
    {
        TUint16* row = pixels + y * stride;
        TInt x = 0;
        for (x = 0; x < size.iWidth; ++x)
        {
            const TInt checker = ((x >> 4) + (y >> 4)) & 1;
            row[x] = checker ? darkA : darkB;
        }
    }

    // Native 320x240 alignment guides. These are intentionally simple and
    // cheap so this platform proof measures timer/input/blit cost rather than
    // a temporary renderer.
    for (y = 0; y < size.iHeight; ++y)
    {
        pixels[y * stride + size.iWidth / 2] = line;
    }
    TUint16* middle = pixels + (size.iHeight / 2) * stride;
    TInt x = 0;
    for (x = 0; x < size.iWidth; ++x)
        middle[x] = line;

    for (y = 4; y < 10; ++y)
    {
        TUint16* row = pixels + y * stride;
        for (x = 4; x < size.iWidth - 4; ++x)
            row[x] = ready;
    }

    // Moving marker proves that key-down/key-up state survives independently
    // from the 30 Hz presentation cadence.
    TInt dy = -3;
    for (dy = -3; dy <= 3; ++dy)
    {
        TUint16* row = pixels + (iMarkerY + dy) * stride;
        TInt dx = -3;
        for (dx = -3; dx <= 3; ++dx)
            row[iMarkerX + dx] = marker;
    }

    // Small activity pulse makes stalled timers obvious on-device.
    const TInt pulseX = 8 + static_cast<TInt>((aLogicFrame / 2) % 64);
    for (y = size.iHeight - 10; y < size.iHeight - 5; ++y)
    {
        TUint16* row = pixels + y * stride;
        for (x = 8; x < 72; ++x)
            row[x] = (x <= pulseX) ? Rgb565(74, 144, 226) : line;
    }

    iFrameBuffer.UnlockPixels();
}
