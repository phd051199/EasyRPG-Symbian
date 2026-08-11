#include "ModernCoreProbe.h"

extern "C" int EasyRpgModernCoreProbe(int aValue)
{
    const auto transform = [aValue](int aInput) -> int
    {
        return aInput * 3 + aValue;
    };

    const int values[] = { 1, 2, 3 };
    int total = 0;
    for (const int value : values)
        total += transform(value);

    return total;
}
