#include <fbs.h>
#include <imageconversion.h>

#include "RgssBitmapBridge.h"
#include "RgssImageLoader.h"
#include "ZipVfs.h"

namespace
{
class CRgssImageDecodeWaiter : public CActive
{
public:
    static CRgssImageDecodeWaiter* NewLC(CImageDecoder& aDecoder)
    {
        CRgssImageDecodeWaiter* self = new (ELeave) CRgssImageDecodeWaiter(aDecoder);
        CleanupStack::PushL(self);
        CActiveScheduler::Add(self);
        return self;
    }

    ~CRgssImageDecodeWaiter()
    {
        Cancel();
    }

    TInt Decode(CFbsBitmap& aBitmap, CFbsBitmap* aMask)
    {
        iResult = KErrNone;
        iStatus = KRequestPending;
        if (aMask)
            iDecoder.Convert(&iStatus, aBitmap, *aMask, 0);
        else
            iDecoder.Convert(&iStatus, aBitmap, 0);
        SetActive();
        iWait.Start();
        return iResult;
    }

private:
    explicit CRgssImageDecodeWaiter(CImageDecoder& aDecoder)
        : CActive(CActive::EPriorityStandard),
          iDecoder(aDecoder),
          iResult(KErrNone)
    {
    }

    void RunL()
    {
        iResult = iStatus.Int();
        if (iWait.IsStarted())
            iWait.AsyncStop();
    }

    TInt RunError(TInt aError)
    {
        iResult = aError;
        if (iWait.IsStarted())
            iWait.AsyncStop();
        return KErrNone;
    }

    void DoCancel()
    {
        iDecoder.Cancel();
        if (iWait.IsStarted())
            iWait.AsyncStop();
    }

private:
    CImageDecoder& iDecoder;
    CActiveSchedulerWait iWait;
    TInt iResult;
};

TInt CopyAsciiPath(const char* aSource, TDes& aTarget)
{
    if (!aSource)
        return KErrArgument;
    aTarget.Zero();
    while (*aSource)
    {
        const TUint8 value = static_cast<TUint8>(*aSource++);
        if (value < 0x20 || value > 0x7e || aTarget.Length() >= aTarget.MaxLength())
            return KErrArgument;
        aTarget.Append(static_cast<TUint16>(value));
    }
    return aTarget.Length() > 0 ? KErrNone : KErrArgument;
}

void ResolveBitmapPathL(CZipVfs& aResources,
                        const char* aRelativePath,
                        TDes& aResolvedPath)
{
    User::LeaveIfError(CopyAsciiPath(aRelativePath, aResolvedPath));
    if (aResources.Exists(aResolvedPath))
        return;

    // RGSS accepts extensionless Bitmap paths (for example
    // "Graphics/Titles/title") and resolves them to an image file. Keep an
    // explicitly supplied extension strict, but emulate that lookup for the
    // extensionless form used by RPG Maker XP games.
    TInt separator = aResolvedPath.LocateReverse('/');
    const TInt backslash = aResolvedPath.LocateReverse('\\');
    if (backslash > separator)
        separator = backslash;
    const TInt dot = aResolvedPath.LocateReverse('.');
    if (dot > separator)
        User::Leave(KErrNotFound);

    _LIT(KPngExtension, ".png");
    _LIT(KJpgExtension, ".jpg");
    _LIT(KJpegExtension, ".jpeg");
    _LIT(KBmpExtension, ".bmp");
    _LIT(KGifExtension, ".gif");
    const TPtrC extensions[] = {
        KPngExtension(),
        KJpgExtension(),
        KJpegExtension(),
        KBmpExtension(),
        KGifExtension()
    };

    TFileName candidate;
    for (TUint index = 0; index < sizeof(extensions) / sizeof(extensions[0]); ++index)
    {
        if (aResolvedPath.Length() + extensions[index].Length() > candidate.MaxLength())
            continue;
        candidate.Copy(aResolvedPath);
        candidate.Append(extensions[index]);
        if (aResources.Exists(candidate))
        {
            aResolvedPath.Copy(candidate);
            return;
        }
    }

    User::Leave(KErrNotFound);
}

void CopyDecodedBitmapL(const CFbsBitmap& aColor,
                        const CFbsBitmap* aMask,
                        TInt& aBitmapHandle)
{
    const TSize size = aColor.SizeInPixels();
    TInt handle = 0;
    const TInt createResult = EasyRpgRgssBitmapCreate(size.iWidth, size.iHeight, &handle);
    if (createResult != ERgssBitmapOk)
        User::Leave(createResult == ERgssBitmapErrorNoMemory ? KErrNoMemory : KErrGeneral);

    unsigned short* destinationColor = NULL;
    unsigned char* destinationAlpha = NULL;
    TInt destinationPitch = 0;
    if (EasyRpgRgssBitmapLock(handle,
                              &destinationColor,
                              &destinationAlpha,
                              &destinationPitch) != ERgssBitmapOk)
    {
        EasyRpgRgssBitmapDestroy(handle);
        User::Leave(KErrGeneral);
    }

    CFbsBitmap& color = const_cast<CFbsBitmap&>(aColor);
    color.LockHeap();
    const TUint8* colorBytes = reinterpret_cast<const TUint8*>(color.DataAddress());
    const TInt colorStride = color.DataStride();

    CFbsBitmap* mask = const_cast<CFbsBitmap*>(aMask);
    const TUint8* maskBytes = NULL;
    TInt maskStride = 0;
    if (mask)
    {
        mask->LockHeap();
        maskBytes = reinterpret_cast<const TUint8*>(mask->DataAddress());
        maskStride = mask->DataStride();
    }

    if (!colorBytes || (mask && !maskBytes))
    {
        if (mask)
            mask->UnlockHeap();
        color.UnlockHeap();
        EasyRpgRgssBitmapDestroy(handle);
        User::Leave(KErrCorrupt);
    }

    for (TInt y = 0; y < size.iHeight; ++y)
    {
        const TUint8* sourceColor = colorBytes + y * colorStride;
        Mem::Copy(destinationColor + y * destinationPitch,
                  sourceColor,
                  size.iWidth * static_cast<TInt>(sizeof(TUint16)));
        if (mask)
            Mem::Copy(destinationAlpha + y * destinationPitch,
                      maskBytes + y * maskStride,
                      size.iWidth);
        else
            Mem::Fill(destinationAlpha + y * destinationPitch, size.iWidth, 0xff);
    }

    if (mask)
        mask->UnlockHeap();
    color.UnlockHeap();
    aBitmapHandle = handle;
}
}

void EasyRpgRgssLoadBitmapL(RFs& aFs,
                            CZipVfs& aResources,
                            const char* aRelativePath,
                            TInt& aBitmapHandle)
{
    aBitmapHandle = 0;
    TFileName path;
    ResolveBitmapPathL(aResources, aRelativePath, path);

    HBufC8* imageData = aResources.ReadFileLC(path, 16 * 1024 * 1024);
    CImageDecoder* decoder = CImageDecoder::DataNewL(aFs, *imageData);
    CleanupStack::PushL(decoder);

    const TFrameInfo& frame = decoder->FrameInfo(0);
    const TSize size = frame.iOverallSizeInPixels;
    if (size.iWidth <= 0 || size.iHeight <= 0 ||
        size.iWidth > 4096 || size.iHeight > 4096)
        User::Leave(KErrTooBig);

    CFbsBitmap* color = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(color);
    User::LeaveIfError(color->Create(size, EColor64K));

    CFbsBitmap* mask = NULL;
    if ((frame.iFlags & TFrameInfo::ETransparencyPossible) != 0)
    {
        mask = new (ELeave) CFbsBitmap;
        CleanupStack::PushL(mask);
        User::LeaveIfError(mask->Create(size, EGray256));
    }

    CRgssImageDecodeWaiter* waiter = CRgssImageDecodeWaiter::NewLC(*decoder);
    User::LeaveIfError(waiter->Decode(*color, mask));
    CopyDecodedBitmapL(*color, mask, aBitmapHandle);
    CleanupStack::PopAndDestroy(waiter);

    if (mask)
        CleanupStack::PopAndDestroy(mask);
    CleanupStack::PopAndDestroy(color);
    CleanupStack::PopAndDestroy(decoder);
    CleanupStack::PopAndDestroy(imageData);
}
