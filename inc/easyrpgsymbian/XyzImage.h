#ifndef EASYRPG_SYMBIAN_XYZ_IMAGE_H
#define EASYRPG_SYMBIAN_XYZ_IMAGE_H

#include <e32base.h>

class CFrameBuffer;
class CZipVfs;

class CXyzImage : public CBase
{
public:
    static CXyzImage* LoadL(const TDesC8& aBytes);
    static CXyzImage* LoadNamedL(CZipVfs& aResources,
                                 const TDesC& aSubdirectory,
                                 const TDesC& aBaseName);
    ~CXyzImage();

    const TSize& Size() const;
    void BlitOpaque565L(CFrameBuffer& aTarget, const TPoint& aOrigin) const;

private:
    CXyzImage();
    void ConstructL(const TDesC8& aBytes);

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
