#ifndef EASYRPG_SYMBIAN_CONTROL_VARIABLE_RANGE_H
#define EASYRPG_SYMBIAN_CONTROL_VARIABLE_RANGE_H

#include <algorithm>

namespace EasyRpgCompat
{

// Applies one Control Variables operation over a variable range. Variables is
// an indexable container (int& operator[](int)), random_value draws in
// [low, high], notify_changed (optional) fires once per changed variable.
template <typename Variables, typename Random>
inline void ApplyControlVariableRange(Variables &variables,
                                      int start,
                                      int end,
                                      int operation,
                                      int operand_mode,
                                      int operand_a,
                                      int operand_b,
                                      int snapshot_value,
                                      int max_value,
                                      int min_value,
                                      Random random_value,
                                      void (*notify_changed)(int) = 0)
{
    const bool is_range = start != end;
    for (int variable_id = start; variable_id <= end; ++variable_id)
    {
        int value = snapshot_value;
        if (is_range && operand_mode == 2)
        {
            value = variables[variables[operand_a]];
        }
        else if (is_range && operand_mode == 3)
        {
            const int high = std::max(operand_a, operand_b);
            const int low = std::min(operand_a, operand_b);
            value = random_value(low, high);
        }
        const int old_value = variables[variable_id];
        switch (operation)
        {
        case 0:
            // Assignement
            variables[variable_id] = value;
            break;
        case 1:
            // Addition
            variables[variable_id] += value;
            break;
        case 2:
            // Subtraction
            variables[variable_id] -= value;
            break;
        case 3:
            // Multiplication
            variables[variable_id] *= value;
            break;
        case 4:
            // Division
            if (value != 0)
            {
                variables[variable_id] /= value;
            }
            break;
        case 5:
            // Module
            if (value != 0)
            {
                variables[variable_id] %= value;
            }
            else
            {
                variables[variable_id] = 0;
            }
            break;
        }
        if (variables[variable_id] > max_value)
        {
            variables[variable_id] = max_value;
        }
        if (variables[variable_id] < min_value)
        {
            variables[variable_id] = min_value;
        }
        if (notify_changed && variables[variable_id] != old_value)
            notify_changed(variable_id);
    }
}

} // namespace EasyRpgCompat

#endif
