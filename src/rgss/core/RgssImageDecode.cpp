#include "rgss/core/RgssBitmapCore.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace ImageXYZ {
bool ReadXYZ(const uint8_t* data, unsigned len, bool transparent,
             int& width, int& height, void*& pixels);
}

namespace ImageBMP {
bool ReadBMP(const uint8_t* data, unsigned len, bool transparent,
             int& width, int& height, void*& pixels);
}

namespace ImagePNG {
bool ReadPNGBytes(const uint8_t* data, unsigned len, bool transparent,
                  int& width, int& height, void*& pixels);
}

namespace rgss {

RgssBitmapCore* RgssDecodeImageToRgb565(const unsigned char* aData, long aSize)
{
    if (!aData || aSize <= 0 || static_cast<unsigned long>(aSize) > 0xfffffffful)
        return 0;

    const unsigned size = static_cast<unsigned>(aSize);
    int width = 0;
    int height = 0;
    void* rgba = 0;
    bool decoded = false;

    static const unsigned char kPngSignature[8] =
        {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    if (size >= sizeof(kPngSignature) &&
        memcmp(aData, kPngSignature, sizeof(kPngSignature)) == 0)
    {
        decoded = ImagePNG::ReadPNGBytes(aData, size, false,
                                         width, height, rgba);
    }
    else if (size >= 4 && memcmp(aData, "XYZ1", 4) == 0)
    {
        decoded = ImageXYZ::ReadXYZ(aData, size, false,
                                    width, height, rgba);
    }
    else if (size >= 2 && aData[0] == 'B' && aData[1] == 'M')
    {
        decoded = ImageBMP::ReadBMP(aData, size, false,
                                    width, height, rgba);
    }

    if (!decoded || !rgba)
    {
        free(rgba);
        return 0;
    }

    RgssBitmapCore* bitmap = new RgssBitmapCore;
    if (!bitmap->CreateFromRgba8(width, height,
                                 static_cast<const unsigned char*>(rgba)))
    {
        delete bitmap;
        bitmap = 0;
    }
    free(rgba);
    return bitmap;
}

} // namespace rgss
