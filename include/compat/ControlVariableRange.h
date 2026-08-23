#ifndef EASYRPG_SYMBIAN_CONTROL_VARIABLE_RANGE_H
#define EASYRPG_SYMBIAN_CONTROL_VARIABLE_RANGE_H

#include <algorithm>

namespace EasyRpgCompat {

template <typename ReadVariable, typename RandomValue, typename ApplyValue>
inline void ApplyControlVariableRange(int start,
		int end,
		int operand_mode,
		int operand_a,
		int operand_b,
		int snapshot_value,
		ReadVariable read_variable,
		RandomValue random_value,
		ApplyValue apply_value) {
	const bool is_range = start != end;
	for (int variable_id = start; variable_id <= end; ++variable_id) {
		int value = snapshot_value;
		if (is_range && operand_mode == 2) {
			value = read_variable(read_variable(operand_a));
		} else if (is_range && operand_mode == 3) {
			const int high = std::max(operand_a, operand_b);
			const int low = std::min(operand_a, operand_b);
			value = random_value(low, high);
		}
		apply_value(variable_id, value);
	}
}

}

#endif
