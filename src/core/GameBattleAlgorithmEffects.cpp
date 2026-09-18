#include <algorithm>

#include "game_battlealgorithm.h"

void Game_BattleAlgorithm::AlgorithmBase::SetRepeat(int repeat_count)
{
    repeat = std::max(1, repeat_count);
    current_repeat = 0;
}

void Game_BattleAlgorithm::AlgorithmBase::ApplyComboHitsMultiplier(int hits)
{
    repeat *= std::max(1, hits);
}

void Game_BattleAlgorithm::AlgorithmBase::PrepareRepeatAnimation()
{
}

bool Game_BattleAlgorithm::AlgorithmBase::RepeatNext()
{
    ++current_repeat;
    if (current_repeat >= repeat)
        return false;
    current_target = targets.begin();
    while (current_target != targets.end() && !IsTargetValid())
        ++current_target;
    first_attack = false;
    return current_target != targets.end();
}
