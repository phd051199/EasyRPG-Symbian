#include <ezdecompressor.h>

#include "easyrpgsymbian/Framebuffer.h"
#include "easyrpgsymbian/PngImage.h"
#include "easyrpgsymbian/ZipVfs.h"

namespace
{
const TUint8 KPngSignature[8] =
{
    0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a
};

TUint32 ReadBigEndian32(const TUint8* aBytes)
{
    return (static_cast<TUint32>(aBytes[0]) << 24) |
           (static_cast<TUint32>(aBytes[1]) << 16) |
           (static_cast<TUint32>(aBytes[2]) << 8) |
           static_cast<TUint32>(aBytes[3]);
}

TBool BytesEqual(const TUint8* aLeft, const TUint8* aRight, TInt aLength)
{
    TInt index = 0;
    for (index = 0; index < aLength; ++index)
    {
        if (aLeft[index] != aRight[index])
            return EFalse;
    }
    return ETrue;
}

TBool ChunkTypeIs(const TUint8* aType, const char* aExpected)
{
    return aType[0] == static_cast<TUint8>(aExpected[0]) &&
           aType[1] == static_cast<TUint8>(aExpected[1]) &&
           aType[2] == static_cast<TUint8>(aExpected[2]) &&
           aType[3] == static_cast<TUint8>(aExpected[3]);
}

TUint16 ToRgb565(TUint8 aRed, TUint8 aGreen, TUint8 aBlue)
{
    return static_cast<TUint16>(((aRed & 0xf8) << 8) |
                               ((aGreen & 0xfc) << 3) |
                               ((aBlue & 0xf8) >> 3));
}

TUint16 BlendRgb565(TUint16 aSource, TUint16 aDestination, TUint8 aAlpha)
{
    if (aAlpha == 255)
        return aSource;
    if (aAlpha == 0)
        return aDestination;

    const TInt inverse = 255 - aAlpha;
    const TInt sourceRed = (aSource >> 11) & 0x1f;
    const TInt sourceGreen = (aSource >> 5) & 0x3f;
    const TInt sourceBlue = aSource & 0x1f;
    const TInt destinationRed = (aDestination >> 11) & 0x1f;
    const TInt destinationGreen = (aDestination >> 5) & 0x3f;
    const TInt destinationBlue = aDestination & 0x1f;

    const TInt red = (sourceRed * aAlpha + destinationRed * inverse + 127) / 255;
    const TInt green = (sourceGreen * aAlpha + destinationGreen * inverse + 127) / 255;
    const TInt blue = (sourceBlue * aAlpha + destinationBlue * inverse + 127) / 255;
    return static_cast<TUint16>((red << 11) | (green << 5) | blue);
}

TUint8 PaethPredictor(TUint8 aLeft, TUint8 aUp, TUint8 aUpLeft)
{
    const TInt p = static_cast<TInt>(aLeft) +
                   static_cast<TInt>(aUp) -
                   static_cast<TInt>(aUpLeft);
    TInt pa = p - static_cast<TInt>(aLeft);
    TInt pb = p - static_cast<TInt>(aUp);
    TInt pc = p - static_cast<TInt>(aUpLeft);
    if (pa < 0)
        pa = -pa;
    if (pb < 0)
        pb = -pb;
    if (pc < 0)
        pc = -pc;

    if (pa <= pb && pa <= pc)
        return aLeft;
    if (pb <= pc)
        return aUp;
    return aUpLeft;
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

CPngImage* CPngImage::LoadL(const TDesC8& aBytes)
{
    CPngImage* self = new (ELeave) CPngImage;
    CleanupStack::PushL(self);
    self->ConstructL(aBytes);
    CleanupStack::Pop(self);
    return self;
}

CPngImage* CPngImage::LoadNamedL(CZipVfs& aResources,
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
    fileName.Append(_L(".png"));
    AppendPathComponentL(path, fileName);

    HBufC8* bytes = aResources.ReadFileLC(path, KMaxFileBytes);
    CPngImage* image = CPngImage::LoadL(*bytes);
    CleanupStack::PopAndDestroy(bytes);
    return image;
}

CPngImage::CPngImage()
    : iPixels(NULL),
      iSize(0, 0),
      iPaletteSize(0)
{
    TInt index = 0;
    for (index = 0; index < KPaletteEntries; ++index)
    {
        iPalette565[index] = 0;
        iAlpha[index] = 255;
    }
}

void CPngImage::ConstructL(const TDesC8& aBytes)
{
    const TInt fileSize = aBytes.Length();
    if (fileSize < 33 || fileSize > KMaxFileBytes)
        User::Leave(KErrTooBig);

    const TUint8* bytes = aBytes.Ptr();
    if (!BytesEqual(bytes, KPngSignature, 8))
        User::Leave(KErrCorrupt);

    TInt width = 0;
    TInt height = 0;
    TInt totalIdatBytes = 0;
    TBool ihdrSeen = EFalse;
    TBool paletteSeen = EFalse;
    TBool iendSeen = EFalse;

    TInt cursor = 8;
    while (cursor <= fileSize - 12)
    {
        const TUint32 chunkLength32 = ReadBigEndian32(bytes + cursor);
        if (chunkLength32 > static_cast<TUint32>(KMaxFileBytes))
            User::Leave(KErrTooBig);
        const TInt chunkLength = static_cast<TInt>(chunkLength32);
        if (chunkLength > fileSize - cursor - 12)
            User::Leave(KErrCorrupt);

        const TUint8* chunkType = bytes + cursor + 4;
        const TUint8* chunkData = bytes + cursor + 8;

        if (ChunkTypeIs(chunkType, "IHDR"))
        {
            if (ihdrSeen || chunkLength != 13)
                User::Leave(KErrCorrupt);
            ihdrSeen = ETrue;

            const TUint32 width32 = ReadBigEndian32(chunkData);
            const TUint32 height32 = ReadBigEndian32(chunkData + 4);
            if (width32 == 0 || height32 == 0 ||
                width32 > static_cast<TUint32>(KMaxPixels) ||
                height32 > static_cast<TUint32>(KMaxPixels))
                User::Leave(KErrTooBig);
            if (width32 > static_cast<TUint32>(KMaxPixels) / height32)
                User::Leave(KErrTooBig);

            width = static_cast<TInt>(width32);
            height = static_cast<TInt>(height32);

            // The first target contains only indexed, 8-bit, non-interlaced
            // PNGs. Keeping this decoder intentionally narrow avoids pulling
            // a desktop-sized PNG/image stack into the E72 process.
            if (chunkData[8] != 8 ||   // bit depth
                chunkData[9] != 3 ||   // indexed colour
                chunkData[10] != 0 ||  // deflate
                chunkData[11] != 0 ||  // adaptive filters
                chunkData[12] != 0)    // no Adam7
                User::Leave(KErrNotSupported);
        }
        else if (ChunkTypeIs(chunkType, "PLTE"))
        {
            if (paletteSeen || chunkLength == 0 ||
                (chunkLength % 3) != 0 || chunkLength > KPaletteEntries * 3)
                User::Leave(KErrCorrupt);
            paletteSeen = ETrue;
            iPaletteSize = chunkLength / 3;

            TInt paletteIndex = 0;
            for (paletteIndex = 0; paletteIndex < iPaletteSize; ++paletteIndex)
            {
                const TInt offset = paletteIndex * 3;
                iPalette565[paletteIndex] = ToRgb565(chunkData[offset],
                                                     chunkData[offset + 1],
                                                     chunkData[offset + 2]);
            }
        }
        else if (ChunkTypeIs(chunkType, "tRNS"))
        {
            if (chunkLength > KPaletteEntries)
                User::Leave(KErrCorrupt);
            TInt alphaIndex = 0;
            for (alphaIndex = 0; alphaIndex < chunkLength; ++alphaIndex)
                iAlpha[alphaIndex] = chunkData[alphaIndex];
        }
        else if (ChunkTypeIs(chunkType, "IDAT"))
        {
            if (chunkLength > KMaxFileBytes - totalIdatBytes)
                User::Leave(KErrTooBig);
            totalIdatBytes += chunkLength;
        }
        else if (ChunkTypeIs(chunkType, "IEND"))
        {
            if (chunkLength != 0)
                User::Leave(KErrCorrupt);
            iendSeen = ETrue;
            cursor += 12;
            break;
        }

        // Four bytes after chunk payload are CRC. The game files are treated
        // as trusted installed assets; structural bounds and zlib stream
        // validation are enforced here without spending code/CPU on CRC.
        cursor += 12 + chunkLength;
    }

    if (!ihdrSeen || !paletteSeen || !iendSeen ||
        width <= 0 || height <= 0 || totalIdatBytes <= 0)
        User::Leave(KErrCorrupt);

    HBufC8* compressed = HBufC8::NewLC(totalIdatBytes);
    TPtr8 compressedBytes = compressed->Des();

    cursor = 8;
    while (cursor <= fileSize - 12)
    {
        const TInt chunkLength = static_cast<TInt>(ReadBigEndian32(bytes + cursor));
        if (chunkLength < 0 || chunkLength > fileSize - cursor - 12)
            User::Leave(KErrCorrupt);

        const TUint8* chunkType = bytes + cursor + 4;
        const TUint8* chunkData = bytes + cursor + 8;
        if (ChunkTypeIs(chunkType, "IDAT"))
            compressedBytes.Append(chunkData, chunkLength);
        if (ChunkTypeIs(chunkType, "IEND"))
            break;
        cursor += 12 + chunkLength;
    }
    if (compressedBytes.Length() != totalIdatBytes)
        User::Leave(KErrCorrupt);

    if (width >= KMaxPixels)
        User::Leave(KErrTooBig);
    const TInt scanlineBytes = width + 1;
    if (height > KMaxPixels / scanlineBytes)
        User::Leave(KErrTooBig);
    const TInt inflatedSize = scanlineBytes * height;

    HBufC8* inflated = HBufC8::NewLC(inflatedSize);
    TPtr8 inflatedBytes = inflated->Des();
    CEZDecompressor::DecompressL(inflatedBytes, compressedBytes);
    if (inflatedBytes.Length() != inflatedSize)
        User::Leave(KErrCorrupt);

    const TInt pixelCount = width * height;
    iPixels = HBufC8::NewL(pixelCount);
    TPtr8 pixels = iPixels->Des();
    pixels.SetLength(pixelCount);

    const TUint8* raw = inflated->Ptr();
    TInt y = 0;
    for (y = 0; y < height; ++y)
    {
        const TUint8 filter = raw[y * scanlineBytes];
        if (filter > 4)
            User::Leave(KErrCorrupt);

        const TUint8* sourceRow = raw + y * scanlineBytes + 1;
        TUint8* destinationRow = const_cast<TUint8*>(pixels.Ptr()) + y * width;
        const TUint8* previousRow = y > 0
            ? reinterpret_cast<const TUint8*>(pixels.Ptr()) + (y - 1) * width
            : NULL;

        TInt x = 0;
        for (x = 0; x < width; ++x)
        {
            const TUint8 encoded = sourceRow[x];
            const TUint8 left = x > 0 ? destinationRow[x - 1] : 0;
            const TUint8 up = previousRow ? previousRow[x] : 0;
            const TUint8 upLeft = (previousRow && x > 0) ? previousRow[x - 1] : 0;
            TUint8 value = encoded;

            switch (filter)
            {
            case 0:
                break;
            case 1:
                value = static_cast<TUint8>(encoded + left);
                break;
            case 2:
                value = static_cast<TUint8>(encoded + up);
                break;
            case 3:
                value = static_cast<TUint8>(encoded +
                    ((static_cast<TInt>(left) + static_cast<TInt>(up)) >> 1));
                break;
            case 4:
                value = static_cast<TUint8>(encoded + PaethPredictor(left, up, upLeft));
                break;
            default:
                User::Leave(KErrCorrupt);
                break;
            }

            if (value >= iPaletteSize)
                User::Leave(KErrCorrupt);
            destinationRow[x] = value;
        }
    }

    iSize = TSize(width, height);

    CleanupStack::PopAndDestroy(inflated);
    CleanupStack::PopAndDestroy(compressed);
}

CPngImage::~CPngImage()
{
    delete iPixels;
    iPixels = NULL;
}

const TSize& CPngImage::Size() const
{
    return iSize;
}

void CPngImage::Blit565L(CFrameBuffer& aTarget, const TPoint& aOrigin) const
{
    if (!iPixels || iSize.iWidth <= 0 || iSize.iHeight <= 0)
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

    TInt stridePixels = 0;
    TUint16* target = aTarget.LockPixelsL(stridePixels);
    const TUint8* source = iPixels->Ptr();

    TInt y = 0;
    for (y = 0; y < copyHeight; ++y)
    {
        const TUint8* sourceRow = source + (sourceY + y) * iSize.iWidth + sourceX;
        TUint16* targetRow = target + (destinationY + y) * stridePixels + destinationX;

        TInt x = 0;
        for (x = 0; x < copyWidth; ++x)
        {
            const TUint8 paletteIndex = sourceRow[x];
            const TUint8 alpha = iAlpha[paletteIndex];
            if (alpha == 255)
                targetRow[x] = iPalette565[paletteIndex];
            else if (alpha != 0)
                targetRow[x] = BlendRgb565(iPalette565[paletteIndex], targetRow[x], alpha);
        }
    }

    aTarget.UnlockPixels();
}
