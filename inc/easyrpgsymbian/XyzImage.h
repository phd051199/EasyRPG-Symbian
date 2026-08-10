#ifndef EASYRPG_SYMBIAN_XYZ_IMAGE_H
#define EASYRPG_SYMBIAN_XYZ_IMAGE_H

#include <e32base.h>
#include <f32file.h>

class CFrameBuffer;

class CXyzImage : public CBase
{
public:
    static CXyzImage* LoadL(RFs& aFs, const TDesC& aPath);
    static CXyzImage* LoadNamedL(RFs& aFs,
                                 const TDesC& aGameRoot,
                                 const TDesC& aSubdirectory,
                                 const TDesC& aBaseName);
    static CXyzImage* LoadFirstL(RFs& aFs,
                                 const TDesC& aGameRoot,
                                 const TDesC& aSubdirectory);
    ~CXyzImage();

    const TSize& Size() const;
    void BlitOpaque565L(CFrameBuffer& aTarget, const TPoint& aOrigin) const;

private:
    CXyzImage();
    void ConstructL(RFs& aFs, const TDesC& aPath);

private:
    enum
    {
        KPaletteBytes = 256 * 3,
        KHeaderBytes = 8,
        KMaxPixels = 1024 * 1024,
        KMaxCompressedBytes = 4 * 1024 * 1024
    };

private:
    HBufC8* iDecoded;
    TSize iSize;
};

#endif
