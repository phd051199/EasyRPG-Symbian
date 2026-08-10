#ifndef EASYRPG_SYMBIAN_PNG_IMAGE_H
#define EASYRPG_SYMBIAN_PNG_IMAGE_H

#include <e32base.h>

class CFrameBuffer;
class CZipVfs;

class CPngImage : public CBase
{
public:
    static CPngImage* LoadL(const TDesC8& aBytes);
    static CPngImage* LoadNamedL(CZipVfs& aResources,
                                 const TDesC& aSubdirectory,
                                 const TDesC& aBaseName);
    ~CPngImage();

    const TSize& Size() const;
    void Blit565L(CFrameBuffer& aTarget, const TPoint& aOrigin) const;

private:
    CPngImage();
    void ConstructL(const TDesC8& aBytes);

private:
    enum
    {
        KMaxPixels = 1024 * 1024,
        KMaxFileBytes = 4 * 1024 * 1024,
        KPaletteEntries = 256
    };

private:
    HBufC8* iPixels;
    TSize iSize;
    TUint16 iPalette565[KPaletteEntries];
    TUint8 iAlpha[KPaletteEntries];
    TInt iPaletteSize;
};

#endif
