#ifndef RGSS_FONT_CORE_H
#define RGSS_FONT_CORE_H

#include <stddef.h>

#include "rgss/core/RgssTypes.h"

namespace rgss {

class RgssBitmapCore;

struct RgssFontStyle
{
    RgssFontStyle()
        : size(22), bold(false), italic(false), color(255, 255, 255, 255),
          shadow(false), outline(true), outColor(0, 0, 0, 128)
    {
    }

    int size;
    bool bold;
    bool italic;
    Color color;
    bool shadow;
    bool outline;
    Color outColor;
};

Rect RgssMeasureTextUtf8(const char* aText, size_t aLength,
                         const RgssFontStyle& aStyle);

void RgssDrawTextUtf8(RgssBitmapCore& aBitmap, const Rect& aBounds,
                      const char* aText, size_t aLength,
                      const RgssFontStyle& aStyle, int aAlign);

} // namespace rgss

#endif
