#ifndef RGSS_PLANE_CORE_H
#define RGSS_PLANE_CORE_H

#include "rgss/core/RgssScene.h"

namespace rgss {

class RgssBitmapCore;

class RgssPlaneCore : public SceneElement
{
public:
    RgssPlaneCore();
    ~RgssPlaneCore();

    RgssBitmapCore* bitmap;
    int ox;
    int oy;
    double zoomX;
    double zoomY;
    int opacity;
    int blendType;
    Color color;
    Tone tone;

    void Draw(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
              const Rect& aClip);

private:
    RgssBitmapCore* Scratch(int aWidth, int aHeight);
    RgssBitmapCore* scratch;
};

} // namespace rgss

#endif
