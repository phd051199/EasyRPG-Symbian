#ifndef EASYRPG_RGSS_IMAGE_LOADER_H
#define EASYRPG_RGSS_IMAGE_LOADER_H

#include <e32base.h>
#include <f32file.h>

class CZipVfs;

void EasyRpgRgssLoadBitmapL(RFs& aFs,
                            CZipVfs& aResources,
                            const char* aRelativePath,
                            TInt& aBitmapHandle);

#endif
