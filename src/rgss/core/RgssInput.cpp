#include "RgssInput.h"

#include <cmath>

namespace rgss {

namespace {

/* Opposite direction pairs cancel each other (Enterbrain behavior). */
bool Opposite(int a, int b)
{
    return (a == 2 && b == 8) || (a == 8 && b == 2) ||
           (a == 4 && b == 6) || (a == 6 && b == 4);
}

/* Numpad-layout dir8 from held Up/Down/Left/Right. */
int CombineDir8(bool aDown, bool aLeft, bool aRight, bool aUp)
{
    if (Opposite(aDown ? 2 : 0, aUp ? 8 : 0) && aDown && aUp)
        return 0;
    if (aDown && aUp)
        return 0;
    if (aLeft && aRight)
        return 0;

    if (aUp)
        return aLeft ? 7 : (aRight ? 9 : 8);
    if (aDown)
        return aLeft ? 1 : (aRight ? 3 : 2);
    if (aLeft)
        return 4;
    if (aRight)
        return 6;
    return 0;
}

int CombineDir4(bool aDown, bool aLeft, bool aRight, bool aUp,
                int aNewestHeld /* one of 2/4/6/8 or 0 */)
{
    if ((aDown && aUp) || (aLeft && aRight))
        return 0;
    return aNewestHeld;
}

} // namespace

RgssInputState::RgssInputState()
    : current(0), previous(0), dir4(0), dir8(0),
      repeatDelayFrames(16), repeatIntervalFrames(4)
{
    for (int i = 0; i < 32; ++i)
        heldFrames[i] = 0;
}

void RgssInputState::SetFrameRate(int aFramesPerSecond)
{
    if (aFramesPerSecond < 1)
        aFramesPerSecond = 40;
    repeatDelayFrames = static_cast<int>(std::ceil(0.4 * aFramesPerSecond));
    repeatIntervalFrames = static_cast<int>(std::ceil(0.1 * aFramesPerSecond));
    if (repeatIntervalFrames < 1)
        repeatIntervalFrames = 1;
}

void RgssInputState::Update(unsigned int aRawBitmask)
{
    previous = current;
    current = aRawBitmask;

    for (int code = 0; code < 32; ++code)
    {
        const unsigned int bit = 1u << code;
        if (current & bit)
            ++heldFrames[code];
        else
            heldFrames[code] = 0;
    }

    const bool down = Press(2), left = Press(4), right = Press(6), up = Press(8);
    dir8 = CombineDir8(down, left, right, up);

    /* dir4: newest direction still held wins (0 when all released or
     * opposite pair held). Find the highest-age direction: the one whose
     * held counter reset most recently has the smallest count among held. */
    int newest = 0;
    int newestFrames = 1 << 30;
    const int dirs[4] = {2, 4, 6, 8};
    for (int i = 0; i < 4; ++i)
    {
        const int code = dirs[i];
        if ((current & (1u << code)) && heldFrames[code] < newestFrames)
        {
            newestFrames = heldFrames[code];
            newest = code;
        }
    }
    dir4 = CombineDir4(down, left, right, up, newest);
}

bool RgssInputState::Press(int aButton) const
{
    if (aButton < 0 || aButton >= 32)
        return false;
    return (current & (1u << aButton)) != 0;
}

bool RgssInputState::Trigger(int aButton) const
{
    if (aButton < 0 || aButton >= 32)
        return false;
    return (current & (1u << aButton)) != 0 &&
           (previous & (1u << aButton)) == 0;
}

bool RgssInputState::Release(int aButton) const
{
    if (aButton < 0 || aButton >= 32)
        return false;
    return (current & (1u << aButton)) == 0 &&
           (previous & (1u << aButton)) != 0;
}

bool RgssInputState::Repeat(int aButton) const
{
    if (aButton < 0 || aButton >= 32)
        return false;
    const int frames = heldFrames[aButton];
    if (frames <= 0)
        return false;
    return frames == 1 ||
           (frames >= repeatDelayFrames &&
            (frames - repeatDelayFrames) % repeatIntervalFrames == 0);
}

} // namespace rgss
