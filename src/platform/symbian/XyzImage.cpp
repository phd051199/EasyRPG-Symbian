#include <ezdecompressor.h>

#include "easyrpgsymbian/Framebuffer.h"
#include "easyrpgsymbian/XyzImage.h"
#include "easyrpgsymbian/ZipVfs.h"

namespace
{
_LIT8(KXyzMagic, "XYZ1");

TUint16 ToRgb565(TUint8 aRed, TUint8 aGreen, TUint8 aBlue)
{
    return static_cast<TUint16>(((aRed & 0xf8) << 8) |
                               ((aGreen & 0xfc) << 3) |
                               ((aBlue & 0xf8) >> 3));
}

void AppendPathComponentL(TFileName& aPath, const TDesC& aComponent)
{
    if (aPath.Length() > 0)
    {
        const TChar last = aPath[aPath.Length() - 1];
        if (last != '\\' && last != '/')
        {
            if (aPath.Length() >= aPath.MaxLength())
                User::Leave(KErrOverflow);
            aPath.Append(_L("\\"));
        }
    }

    if (aComponent.Length() > aPath.MaxLength() - aPath.Length())
        User::Leave(KErrOverflow);
    aPath.Append(aComponent);
}
}

CXyzImage* CXyzImage::LoadL(const TDesC8& aBytes)
{
    CXyzImage* self = new (ELeave) CXyzImage;
    CleanupStack::PushL(self);
    self->ConstructL(aBytes);
    CleanupStack::Pop(self);
    return self;
}

CXyzImage* CXyzImage::LoadNamedL(CZipVfs& aResources,
                                 const TDesC& aSubdirectory,
                                 const TDesC& aBaseName)
{
    if (aBaseName.Length() == 0)
        User::Leave(KErrArgument);

    TFileName path;
    AppendPathComponentL(path, aSubdirectory);

    TFileName fileName;
    if (aBaseName.Length() + 4 > fileName.MaxLength())
        User::Leave(KErrOverflow);
    fileName.Copy(aBaseName);
    fileName.Append(_L(".xyz"));
    AppendPathComponentL(path, fileName);

    HBufC8* bytes = aResources.ReadFileLC(path, KMaxCompressedBytes + KHeaderBytes);
    CXyzImage* image = CXyzImage::LoadL(*bytes);
    CleanupStack::PopAndDestroy(bytes);
    return image;
}

CXyzImage::CXyzImage()
    : iDecoded(NULL),
      iSize(0, 0)
{
}

void CXyzImage::ConstructL(const TDesC8& aBytes)
{
    const TInt fileSize = aBytes.Length();
    if (fileSize <= KHeaderBytes || fileSize > KMaxCompressedBytes + KHeaderBytes)
        User::Leave(KErrTooBig);

    const TPtrC8 header = aBytes.Left(KHeaderBytes);
    if (header.Length() != KHeaderBytes || header.Left(KXyzMagic().Length()).Compare(KXyzMagic) != 0)
        User::Leave(KErrCorrupt);

    const TInt width = static_cast<TInt>(header[4]) |
                       (static_cast<TInt>(header[5]) << 8);
    const TInt height = static_cast<TInt>(header[6]) |
                        (static_cast<TInt>(header[7]) << 8);
    if (width <= 0 || height <= 0)
        User::Leave(KErrCorrupt);
    if (width > KMaxPixels / height)
        User::Leave(KErrTooBig);

    const TInt pixelCount = width * height;
    const TInt decodedSize = KPaletteBytes + pixelCount;
    const TInt compressedSize = fileSize - KHeaderBytes;

    const TPtrC8 compressedBytes = aBytes.Mid(KHeaderBytes, compressedSize);

    iDecoded = HBufC8::NewL(decodedSize);
    TPtr8 decodedBytes = iDecoded->Des();
    CEZDecompressor::DecompressL(decodedBytes, compressedBytes);
    if (decodedBytes.Length() != decodedSize)
        User::Leave(KErrCorrupt);

    iSize = TSize(width, height);

}

CXyzImage::~CXyzImage()
{
    delete iDecoded;
    iDecoded = NULL;
}

const TSize& CXyzImage::Size() const
{
    return iSize;
}

void CXyzImage::BlitOpaque565L(CFrameBuffer& aTarget, const TPoint& aOrigin) const
{
    if (!iDecoded || iSize.iWidth <= 0 || iSize.iHeight <= 0)
        User::Leave(KErrNotReady);

    const TSize targetSize = aTarget.Size();
    TInt sourceX = 0;
    TInt sourceY = 0;
    TInt destinationX = aOrigin.iX;
    TInt destinationY = aOrigin.iY;
    TInt copyWidth = iSize.iWidth;
    TInt copyHeight = iSize.iHeight;

    if (destinationX < 0)
    {
        sourceX = -destinationX;
        copyWidth -= sourceX;
        destinationX = 0;
    }
    if (destinationY < 0)
    {
        sourceY = -destinationY;
        copyHeight -= sourceY;
        destinationY = 0;
    }
    if (destinationX + copyWidth > targetSize.iWidth)
        copyWidth = targetSize.iWidth - destinationX;
    if (destinationY + copyHeight > targetSize.iHeight)
        copyHeight = targetSize.iHeight - destinationY;
    if (copyWidth <= 0 || copyHeight <= 0)
        return;

    const TUint8* bytes = iDecoded->Ptr();
    const TUint8* palette = bytes;
    const TUint8* indices = bytes + KPaletteBytes;

    TUint16 palette565[256];
    TInt paletteIndex = 0;
    for (paletteIndex = 0; paletteIndex < 256; ++paletteIndex)
    {
        const TInt offset = paletteIndex * 3;
        palette565[paletteIndex] = ToRgb565(palette[offset],
                                           palette[offset + 1],
                                           palette[offset + 2]);
    }

    TInt stridePixels = 0;
    TUint16* target = aTarget.LockPixelsL(stridePixels);

    TInt y = 0;
    for (y = 0; y < copyHeight; ++y)
    {
        const TUint8* sourceRow = indices + (sourceY + y) * iSize.iWidth + sourceX;
        TUint16* targetRow = target + (destinationY + y) * stridePixels + destinationX;

        TInt x = 0;
        for (x = 0; x < copyWidth; ++x)
            targetRow[x] = palette565[sourceRow[x]];
    }

    aTarget.UnlockPixels();
}
