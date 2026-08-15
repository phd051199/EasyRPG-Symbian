#ifndef RGSS_GRAPHICS_CORE_H
#define RGSS_GRAPHICS_CORE_H

/*
 * Frame composition driver (RGSS1 Graphics). Owns the native-resolution
 * RGB565 back buffer; the binding reads GetFrame() each frame and hands it
 * to RgssHostVtable::Present. Frozen-bitmap Transition is an MVP crossfade;
 * FadeOut/FadeIn are brightness multiplies.
 */

#include "rgss/core/RgssBitmapCore.h"
#include "rgss/core/RgssScene.h"

namespace rgss {

class RgssGraphicsCore
{
public:
    explicit RgssGraphicsCore(RgssScene& aScene);
    ~RgssGraphicsCore();

    /* (Re)creates the back buffer and retargets the default viewport. */
    bool SetSize(int aWidth, int aHeight);
    int Width() const { return back.Width(); }
    int Height() const { return back.Height(); }

    int frameCount;
    int frameRate; /* default 40 (RPG Maker XP) */

    /* One frame: compose the scene, then apply transition/fade. */
    void Update();
    void Freeze();
    /* MVP crossfade from the frozen bitmap over aDuration frames;
     * aVague is accepted but unused. */
    void Transition(int aDuration, int aVague);
    void FadeOut(int aCount);
    void FadeIn(int aCount);

    /* Frame pixels for host->Present(ctx, frame, width, height). */
    const unsigned short* GetFrame() const { return back.Pixels(); }

    RgssBitmapCore& BackBuffer() { return back; }
    RgssScene& Scene() { return scene; }

    bool IsFrozen() const { return frozen.IsValid(); }
    int Brightness() const { return brightness; }

private:
    RgssGraphicsCore(const RgssGraphicsCore&);
    RgssGraphicsCore& operator=(const RgssGraphicsCore&);

    enum TransitionMode
    {
        KTransNone = 0,
        KTransCrossfade,
        KTransFadeOut,
        KTransFadeIn
    };

    RgssScene& scene;
    RgssBitmapCore back;
    RgssBitmapCore frozen;
    TransitionMode mode;
    int step;
    int total;
    int brightness; /* 0-255, driven by FadeOut/FadeIn */
};

} // namespace rgss

#endif
