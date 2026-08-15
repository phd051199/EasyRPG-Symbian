/* Host unit test for the RGSS value types and their Marshal formats
 * (plain asserts, pattern of tests/render_damage_test.cpp). */

#include "rgss/core/RgssTypes.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace rgss;

static void TestRectRoundTrip()
{
    Rect rect(11, -22, 330, 440);
    unsigned char buffer[16];
    SerializeRect(buffer, rect);

    int expected[4] = {11, -22, 330, 440};
    assert(memcmp(buffer, expected, 16) == 0); /* little-endian host */

    Rect out;
    assert(DeserializeRect(buffer, sizeof(buffer), &out));
    assert(out.x == 11 && out.y == -22 && out.width == 330 && out.height == 440);

    assert(!DeserializeRect(buffer, 15, &out));
    printf("rect ok\n");
}

static void TestColorToneRoundTrip()
{
    Color color(1.5, -2.25, 255.0, 128.0);
    unsigned char buffer[32];
    SerializeColor(buffer, color);

    Color outColor(0, 0, 0, 0);
    assert(DeserializeColor(buffer, sizeof(buffer), &outColor));
    assert(outColor.red == 1.5 && outColor.green == -2.25);
    assert(outColor.blue == 255.0 && outColor.alpha == 128.0);
    assert(!DeserializeColor(buffer, 31, &outColor));

    Tone tone(-255.0, 0.0, 123.75, 200.0);
    SerializeTone(buffer, tone);
    Tone outTone;
    assert(DeserializeTone(buffer, sizeof(buffer), &outTone));
    assert(outTone.red == -255.0 && outTone.gray == 200.0);
    printf("color/tone ok\n");
}

static void TestTableRoundTrip()
{
    Table table(3, 4, 2);
    assert(table.Xsize() == 3 && table.Ysize() == 4 && table.Zsize() == 2);
    assert(table.SerializedSize() == 20 + 3 * 4 * 2 * 2);

    short counter = 0;
    for (int z = 0; z < 2; ++z)
        for (int y = 0; y < 4; ++y)
            for (int x = 0; x < 3; ++x)
                table.Set(x, y, z, counter++);

    std::vector<unsigned char> buffer(table.SerializedSize());
    table.Serialize(&buffer[0]);

    /* Header: dim=3, x=3, y=4, z=2, size=24. */
    assert(ReadLe32(&buffer[0]) == 3);
    assert(ReadLe32(&buffer[4]) == 3);
    assert(ReadLe32(&buffer[8]) == 4);
    assert(ReadLe32(&buffer[12]) == 2);
    assert(ReadLe32(&buffer[16]) == 24);

    Table loaded;
    assert(loaded.Deserialize(&buffer[0], buffer.size()));
    short value = -1;
    assert(loaded.Get(0, 0, 0, &value) && value == 0);
    assert(loaded.Get(2, 3, 1, &value) && value == 23);
    assert(loaded.Get(1, 1, 1, &value) && value == 1 * 12 + 1 * 3 + 1);
    assert(!loaded.Get(3, 0, 0, &value)); /* out of range */

    /* Malformed sizes are rejected. */
    assert(!loaded.Deserialize(&buffer[0], buffer.size() - 1));
    assert(!loaded.Deserialize(&buffer[0], 19));
    printf("table ok\n");
}

static void TestTableResizePreserves()
{
    Table table(2, 2, 1);
    table.Set(0, 0, 0, 100);
    table.Set(1, 1, 0, 111);
    table.Resize(3, 1, 1);
    short value = -1;
    assert(table.Get(0, 0, 0, &value) && value == 100);
    assert(!table.Get(1, 1, 0, &value)); /* y shrank */

    /* Dim in header: 1 when 1xNx1, 2 when y>1, 3 when z>1. */
    std::vector<unsigned char> buffer(table.SerializedSize());
    table.Serialize(&buffer[0]);
    assert(ReadLe32(&buffer[0]) == 1);

    Table three(1, 1, 2);
    three.Serialize(&buffer[0]);
    assert(ReadLe32(&buffer[0]) == 3);
    printf("table resize ok\n");
}

int main()
{
    TestRectRoundTrip();
    TestColorToneRoundTrip();
    TestTableRoundTrip();
    TestTableResizePreserves();
    printf("rgss_types_test: all ok\n");
    return 0;
}
