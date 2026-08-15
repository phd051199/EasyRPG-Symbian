#ifndef RGSS_INPUT_H
#define RGSS_INPUT_H

/*
 * Portable RGSS1 input state machine. The host feeds a raw button bitmask
 * (bit i = RGSS button code i held) once per Graphics.update; queries are
 * answered from the internal state. No SDL/Symbian headers -> host-testable.
 *
 * Button codes: 2=Down 4=Left 6=Right 8=Up, 11..18=A..R, 21=Shift 22=Ctrl
 * 23=Alt, 25..29=F5..F9.
 */

namespace rgss {

class RgssInputState
{
public:
    RgssInputState();

    /* Repeats: fires on the first held frame, then after a 0.4 s start
     * delay, then every 0.1 s (frame counts computed from the frame rate). */
    void SetFrameRate(int aFramesPerSecond);
    void Update(unsigned int aRawBitmask);

    bool Press(int aButton) const;
    bool Trigger(int aButton) const;
    bool Release(int aButton) const;
    bool Repeat(int aButton) const;

    /* 0 or one of 2/4/6/8; opposite pair cancels, else newest press wins. */
    int Dir4() const { return dir4; }
    /* 0 or 1..9 (numpad layout); opposite pair cancels. */
    int Dir8() const { return dir8; }

private:
    unsigned int current;  /* held now */
    unsigned int previous; /* held in the last update */
    int heldFrames[32];    /* consecutive held frames per button code */
    int dir4, dir8;
    int repeatDelayFrames; /* ceil(0.4 * fps) */
    int repeatIntervalFrames; /* ceil(0.1 * fps) */
};

} // namespace rgss

#endif
