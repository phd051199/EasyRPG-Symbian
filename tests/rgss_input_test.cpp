/* Host unit test for the RGSS1 input state machine (plain asserts). */

#include "rgss/core/RgssInput.h"

#include <cassert>
#include <cstdio>

using rgss::RgssInputState;

static const unsigned int kDown = 1u << 2;
static const unsigned int kLeft = 1u << 4;
static const unsigned int kRight = 1u << 6;
static const unsigned int kUp = 1u << 8;
static const unsigned int kC = 1u << 13;

static void TestPressTriggerRelease()
{
    RgssInputState input;
    input.SetFrameRate(40);

    input.Update(0);
    input.Update(kC);
    assert(input.Press(13));
    assert(input.Trigger(13));
    assert(!input.Release(13));
    assert(input.Repeat(13)); /* first frame */

    input.Update(kC);
    assert(input.Press(13));
    assert(!input.Trigger(13));
    assert(!input.Repeat(13)); /* inside start delay */

    input.Update(0);
    assert(!input.Press(13));
    assert(input.Release(13));
    assert(!input.Repeat(13));
    printf("press/trigger/release ok\n");
}

static void TestRepeatTiming()
{
    RgssInputState input;
    input.SetFrameRate(40); /* delay 16, interval 4 */

    int fired = 0;
    for (int frame = 1; frame <= 40; ++frame)
    {
        input.Update(kC);
        if (input.Repeat(13))
            ++fired;
    }
    /* fires at frames 1, 16, 20, 24, 28, 32, 36, 40 = 8 times */
    assert(fired == 8);
    printf("repeat timing ok (%d fires)\n", fired);
}

static void TestDir4NewestWins()
{
    RgssInputState input;

    input.Update(kDown);
    assert(input.Dir4() == 2);
    input.Update(kDown | kLeft);
    assert(input.Dir4() == 4); /* left is newer */
    input.Update(kDown | kLeft);
    assert(input.Dir4() == 4); /* stable while held */
    input.Update(kDown | kLeft | kRight);
    assert(input.Dir4() == 0); /* left+right opposite pair cancels */
    input.Update(kDown); /* left/right released, down is older but only one */
    assert(input.Dir4() == 2);

    /* Opposite pair cancels. */
    input.Update(0);
    input.Update(kDown | kUp);
    assert(input.Dir4() == 0);
    printf("dir4 ok\n");
}

static void TestDir8()
{
    RgssInputState input;
    input.Update(kDown | kLeft);
    assert(input.Dir8() == 1);
    input.Update(kUp | kRight);
    assert(input.Dir8() == 9);
    input.Update(kLeft);
    assert(input.Dir8() == 4);
    input.Update(kLeft | kRight);
    assert(input.Dir8() == 0); /* opposite pair */
    printf("dir8 ok\n");
}

int main()
{
    TestPressTriggerRelease();
    TestRepeatTiming();
    TestDir4NewestWins();
    TestDir8();
    printf("rgss_input_test: all ok\n");
    return 0;
}
