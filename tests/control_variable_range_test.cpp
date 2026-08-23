#include <cassert>
#include <vector>

#include "compat/ControlVariableRange.h"

namespace {

int Read(const std::vector<int>& vars, int id) {
	return id > 0 && id <= static_cast<int>(vars.size()) ? vars[id - 1] : 0;
}

}

int main() {
	{
		std::vector<int> vars{10, 20, 30, 40};
		const int snapshot = Read(vars, 2);
		EasyRpgCompat::ApplyControlVariableRange(
			1, 3, 1, 2, 0, snapshot,
			[&](int id) { return Read(vars, id); },
			[](int, int) { return -1; },
			[&](int id, int value) { vars[id - 1] = value; });
		assert(vars[0] == 20 && vars[1] == 20 && vars[2] == 20);
	}

	{
		std::vector<int> vars{2, 4, 30, 40};
		const int snapshot = Read(vars, Read(vars, 1));
		EasyRpgCompat::ApplyControlVariableRange(
			1, 2, 2, 1, 0, snapshot,
			[&](int id) { return Read(vars, id); },
			[](int, int) { return -1; },
			[&](int id, int value) { vars[id - 1] = value; });
		assert(vars[0] == 4);
		assert(vars[1] == 40);
	}

	{
		std::vector<int> vars(3, 0);
		int next = 0;
		EasyRpgCompat::ApplyControlVariableRange(
			1, 3, 3, 5, 7, 6,
			[&](int id) { return Read(vars, id); },
			[&](int low, int high) {
				assert(low == 5 && high == 7);
				return 5 + (next++ % 3);
			},
			[&](int id, int value) { vars[id - 1] = value; });
		assert(vars[0] == 5 && vars[1] == 6 && vars[2] == 7);
		assert(next == 3);
	}

	{
		std::vector<int> vars(1, 0);
		int rng_calls = 0;
		EasyRpgCompat::ApplyControlVariableRange(
			1, 1, 3, 1, 9, 7,
			[&](int id) { return Read(vars, id); },
			[&](int, int) { ++rng_calls; return 9; },
			[&](int id, int value) { vars[id - 1] = value; });
		assert(vars[0] == 7);
		assert(rng_calls == 0);
	}

	return 0;
}
