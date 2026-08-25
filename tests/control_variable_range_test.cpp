#include <cassert>
#include <vector>

#include "compat/ControlVariableRange.h"

namespace {

struct TestVariables {
	explicit TestVariables(std::vector<int> initial) : values(initial) {}
	int& operator[](int id) { return values[id - 1]; }
	std::vector<int> values;
};

const int kMax = 9999999;
const int kMin = -9999999;

int g_notified = 0;

void CountingNotify(int) {
	++g_notified;
}

}

int main() {
	{
		// Snapshot value assigned across a range (mode 1).
		TestVariables vars{{10, 20, 30, 40}};
		EasyRpgCompat::ApplyControlVariableRange(vars,
			1, 3, 0, 1, 2, 0, 20, kMax, kMin,
			[](int, int) { return -1; });
		assert(vars.values[0] == 20 && vars.values[1] == 20 && vars.values[2] == 20);
	}

	{
		// Mode 2 re-reads the indirect variable per element.
		TestVariables vars{{2, 4, 30, 40}};
		EasyRpgCompat::ApplyControlVariableRange(vars,
			1, 2, 0, 2, 1, 0, 4, kMax, kMin,
			[](int, int) { return -1; });
		assert(vars.values[0] == 4);
		assert(vars.values[1] == 40);
	}

	{
		// Mode 3 draws per element within [low, high].
		TestVariables vars{{0, 0, 0}};
		int next = 0;
		EasyRpgCompat::ApplyControlVariableRange(vars,
			1, 3, 0, 3, 7, 5, 6, kMax, kMin,
			[&](int low, int high) {
				assert(low == 5 && high == 7);
				return 5 + (next++ % 3);
			});
		assert(vars.values[0] == 5 && vars.values[1] == 6 && vars.values[2] == 7);
		assert(next == 3);
	}

	{
		// Single variable never consults the RNG (mode 3 ignored).
		TestVariables vars{{0}};
		int rng_calls = 0;
		EasyRpgCompat::ApplyControlVariableRange(vars,
			1, 1, 0, 3, 9, 7, 7, kMax, kMin,
			[&](int, int) { ++rng_calls; return 9; });
		assert(vars.values[0] == 7);
		assert(rng_calls == 0);
	}

	{
		// Operations: add, mod-by-zero reset, div-by-zero guard, clamp.
		TestVariables vars{{5, 7, -8, 9999999}};
		EasyRpgCompat::ApplyControlVariableRange(vars,
			1, 1, 1, 1, 0, 0, 3, kMax, kMin,
			[](int, int) { return -1; });
		assert(vars.values[0] == 8);
		EasyRpgCompat::ApplyControlVariableRange(vars,
			2, 2, 5, 1, 0, 0, 0, kMax, kMin,
			[](int, int) { return -1; });
		assert(vars.values[1] == 0);
		EasyRpgCompat::ApplyControlVariableRange(vars,
			3, 3, 4, 1, 0, 0, 0, kMax, kMin,
			[](int, int) { return -1; });
		assert(vars.values[2] == -8);
		EasyRpgCompat::ApplyControlVariableRange(vars,
			4, 4, 1, 1, 0, 0, 1, kMax, kMin,
			[](int, int) { return -1; });
		assert(vars.values[3] == 9999999);
	}

	{
		// Notification fires per changed variable.
		TestVariables vars{{1, 2}};
		g_notified = 0;
		EasyRpgCompat::ApplyControlVariableRange(vars,
			1, 2, 0, 1, 0, 0, 5, kMax, kMin,
			[](int, int) { return -1; }, &CountingNotify);
		assert(vars.values[0] == 5 && vars.values[1] == 5);
		assert(g_notified == 2);
	}

	{
		// A clamped write back to the original value does not count as changed.
		TestVariables vars{{9999999}};
		g_notified = 0;
		EasyRpgCompat::ApplyControlVariableRange(vars,
			1, 1, 1, 1, 0, 0, 1, kMax, kMin,
			[](int, int) { return -1; }, &CountingNotify);
		assert(vars.values[0] == 9999999);
		assert(g_notified == 0);
	}

	return 0;
}
