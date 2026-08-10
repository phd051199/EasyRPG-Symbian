#include "easyrpgsymbian/DisplaySurface.h"
#include "easyrpgsymbian/Framebuffer.h"
#include "easyrpgsymbian/UpstreamGraphicsBridge.h"

CSymbianDisplaySurface* CSymbianDisplaySurface::NewL(const TSize& aSize)
{
    CSymbianDisplaySurface* self = new (ELeave) CSymbianDisplaySurface;
    CleanupStack::PushL(self);
    self->ConstructL(aSize);
    CleanupStack::Pop(self);
    return self;
}

CSymbianDisplaySurface::CSymbianDisplaySurface()
    : iPixels(NULL),
      iNativeBuffer(NULL),
      iSize(0, 0),
      iStridePixels(0)
{
}

void CSymbianDisplaySurface::ConstructL(const TSize& aSize)
{
    if (aSize.iWidth <= 0 || aSize.iHeight <= 0)
        User::Leave(KErrArgument);
    if (aSize.iWidth > KMaxTInt / aSize.iHeight)
        User::Leave(KErrOverflow);

    const TInt pixelCount = aSize.iWidth * aSize.iHeight;
    if (pixelCount > KMaxTInt / static_cast<TInt>(sizeof(TUint32)))
        User::Leave(KErrOverflow);

    iPixels = HBufC8::NewMaxL(pixelCount * sizeof(TUint32));
    iNativeBuffer = CFrameBuffer::NewL(aSize);
    iSize = aSize;
    iStridePixels = aSize.iWidth;
    Clear(0);
    PresentL();
}

CSymbianDisplaySurface::~CSymbianDisplaySurface()
{
    delete iNativeBuffer;
    iNativeBuffer = NULL;
    delete iPixels;
    iPixels = NULL;
}

TUint32* CSymbianDisplaySurface::Pixels()
{
    if (!iPixels)
        return NULL;
    return reinterpret_cast<TUint32*>(const_cast<TUint8*>(iPixels->Ptr()));
}

const TUint32* CSymbianDisplaySurface::Pixels() const
{
    return iPixels ? reinterpret_cast<const TUint32*>(iPixels->Ptr()) : NULL;
}

TInt CSymbianDisplaySurface::StridePixels() const
{
    return iStridePixels;
}

const TSize& CSymbianDisplaySurface::Size() const
{
    return iSize;
}

void CSymbianDisplaySurface::Clear(TUint32 aColor)
{
    TUint32* pixels = Pixels();
    if (!pixels)
        return;

    const TInt count = iStridePixels * iSize.iHeight;
    TInt index = 0;
    for (index = 0; index < count; ++index)
        pixels[index] = aColor;
}

void CSymbianDisplaySurface::PresentL()
{
    if (!iPixels || !iNativeBuffer)
        User::Leave(KErrNotReady);

    TInt nativeStridePixels = 0;
    TUint16* nativePixels = iNativeBuffer->LockPixelsL(nativeStridePixels);
    const TUint32* sourcePixels = Pixels();
    TInt sourceStridePixels = iStridePixels;
    const unsigned int* upstreamPixels = EasyRpgUpstreamGraphicsPixels();
    if (upstreamPixels &&
        EasyRpgUpstreamGraphicsWidth() == iSize.iWidth &&
        EasyRpgUpstreamGraphicsHeight() == iSize.iHeight)
    {
        sourcePixels = reinterpret_cast<const TUint32*>(upstreamPixels);
        sourceStridePixels = EasyRpgUpstreamGraphicsPitchPixels();
    }

    TInt y = 0;
    for (y = 0; y < iSize.iHeight; ++y)
    {
        const TUint32* sourceRow = sourcePixels + y * sourceStridePixels;
        TUint16* nativeRow = nativePixels + y * nativeStridePixels;
        TInt x = 0;
        for (x = 0; x < iSize.iWidth; ++x)
        {
            const TUint32 pixel = sourceRow[x];
            const TUint32 red = (pixel >> 16) & 0xff;
            const TUint32 green = (pixel >> 8) & 0xff;
            const TUint32 blue = pixel & 0xff;
            nativeRow[x] = static_cast<TUint16>(((red & 0xf8) << 8) |
                                                ((green & 0xfc) << 3) |
                                                (blue >> 3));
        }
    }

    iNativeBuffer->UnlockPixels();
}

const CFbsBitmap& CSymbianDisplaySurface::Bitmap() const
{
    return iNativeBuffer->Bitmap();
}
