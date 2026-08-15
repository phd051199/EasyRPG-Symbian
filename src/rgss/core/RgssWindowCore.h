#ifndef RGSS_WINDOW_CORE_H
#define RGSS_WINDOW_CORE_H

#include "rgss/core/RgssScene.h"

namespace rgss {

class RgssBitmapCore;

class RgssWindowCore : public SceneElement
{
public:
    RgssWindowCore();
    ~RgssWindowCore();

    RgssBitmapCore* windowskin;
    RgssBitmapCore* contents;
    Rect cursorRect;
    int x;
    int y;
    int width;
    int height;
    int ox;
    int oy;
    int opacity;
    int backOpacity;
    int contentsOpacity;
    int openness;
    int padding;
    int paddingBottom;
    int generation;
    Tone tone;
    bool active;
    bool pause;
    bool stretch;
    bool arrowsVisible;

    int SortY() const { return y; }
    void Draw(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
              const Rect& aClip);

private:
    RgssBitmapCore* Scratch(int aWidth, int aHeight);
    RgssBitmapCore* scratch;
};

} // namespace rgss

#endif
