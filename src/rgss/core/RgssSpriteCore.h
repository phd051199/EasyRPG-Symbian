#ifndef RGSS_SPRITE_CORE_H
#define RGSS_SPRITE_CORE_H

/*
 * Sprite element state (RGSS1 Sprite). The bitmap reference is non-owning:
 * the binding owns bitmap lifetime and must clear it before freeing.
 * angle and wave are deliberately not implemented in M-R2; the binding
 * rejects them until a rotation path exists.
 */

#include "rgss/core/RgssScene.h"
#include "rgss/core/RgssTypes.h"

namespace rgss {

class RgssSpriteCore : public SceneElement
{
public:
    RgssSpriteCore();

    RgssBitmapCore* bitmap; /* non-owning; null draws nothing */
    Rect srcRect;
    int x, y, ox, oy;
    double zoomX, zoomY; /* 1.0 = unscaled; SLOW PATH when != 1.0 */
    int opacity;         /* 0-255 */
    int bushTop;         /* source rows at the bottom drawn with bushOpacity */
    int bushOpacity;     /* 0-255 */
    int waveAmp;         /* horizontal wave amplitude in source pixels */
    int waveLength;      /* wavelength in source pixels */
    double wavePhase;    /* degrees */
    int waveSpeed;       /* degrees advanced by update/20 */
    int blendType;       /* KBlendNormal / KBlendAdditive / KBlendSubtractive */
    bool mirror;         /* horizontal flip; SLOW PATH */

    int SortY() const { return y; }
    void Draw(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
              const Rect& aClip);
    void DrawRaw(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
                 const Rect& aClip);
};

} // namespace rgss

#endif
