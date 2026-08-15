#include "rgss/core/RgssGraphicsCore.h"

namespace rgss {

RgssGraphicsCore::RgssGraphicsCore(RgssScene& aScene)
    : scene(aScene), mode(KTransNone), step(0), total(0), brightness(255)
{
    frameCount = 0;
    frameRate = 40;
    back.Create(640, 480);
    scene.SetScreenSize(back.Width(), back.Height());
}

RgssGraphicsCore::~RgssGraphicsCore()
{
}

bool RgssGraphicsCore::SetSize(int aWidth, int aHeight)
{
    if (!back.Create(aWidth, aHeight))
        return false;
    scene.SetScreenSize(aWidth, aHeight);
    frozen.Create(0, 0); /* any old frozen frame is invalid after resize */
    return true;
}

void RgssGraphicsCore::Freeze()
{
    if (!back.IsValid())
        return;
    frozen.Create(back.Width(), back.Height());
    if (!frozen.IsValid())
        return;
    Rect full(0, 0, back.Width(), back.Height());
    frozen.Blit(&back, full, 0, 0, 255);
}

void RgssGraphicsCore::Transition(int aDuration, int aVague)
{
    (void)aVague; /* MVP: crossfade ignores the vague parameter */
    if (aDuration <= 0) {
        mode = KTransNone;
        step = total = 0;
        return;
    }
    mode = KTransCrossfade;
    step = 0;
    total = aDuration;
}

void RgssGraphicsCore::FadeOut(int aCount)
{
    if (aCount <= 0) {
        brightness = 0;
        mode = KTransNone;
        return;
    }
    mode = KTransFadeOut;
    step = 0;
    total = aCount;
}

void RgssGraphicsCore::FadeIn(int aCount)
{
    if (aCount <= 0) {
        brightness = 255;
        mode = KTransNone;
        return;
    }
    mode = KTransFadeIn;
    step = 0;
    total = aCount;
}

void RgssGraphicsCore::Update()
{
    if (!back.IsValid()) {
        ++frameCount;
        return;
    }

    // ponytail: full-frame compose (dirty-rect optimization is M-R5).
    back.Clear();
    scene.Compose(back);

    if (mode == KTransCrossfade) {
        /* Crossfade = frozen bitmap fading out over the live frame:
         * live + frozen OVER at (1 - progress) opacity. */
        if (frozen.IsValid() && frozen.Width() == back.Width() &&
            frozen.Height() == back.Height() && step < total) {
            int frozenOpacity = 255 * (total - step) / total;
            Rect full(0, 0, back.Width(), back.Height());
            back.Blit(&frozen, full, 0, 0, frozenOpacity);
        }
        ++step;
        if (step >= total)
            mode = KTransNone;
    } else if (mode == KTransFadeOut || mode == KTransFadeIn) {
        /* Brightness steps toward 0 (FadeOut) or 255 (FadeIn). */
        ++step;
        if (step >= total) {
            brightness = mode == KTransFadeOut ? 0 : 255;
            mode = KTransNone;
        } else {
            int target = mode == KTransFadeOut ? 0 : 255;
            brightness = 255 - (255 - target) * step / total;
        }
        if (brightness < 255)
            back.MultiplyBrightness(brightness);
    }

    ++frameCount;
}

} // namespace rgss
