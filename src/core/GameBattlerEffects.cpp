#include <algorithm>

#include "game_battler.h"
#include "player.h"

void Game_Battler::ShakeOnce(int strength, int speed, int frames)
{
    shake_strength = std::max(0, strength);
    shake_speed = std::max(0, speed);
    shake_frames = std::max(0, frames);
    shake_start_frame = Player::GetFrames();
}

void Game_Battler::SetBattleCombo(int command_id, int times)
{
    battle_combo_command_id = command_id;
    battle_combo_times = std::max(1, times);
}

int Game_Battler::GetBattleComboCommand() const
{
    return battle_combo_command_id;
}

int Game_Battler::GetBattleComboTimes() const
{
    return std::max(1, battle_combo_times);
}

int Game_Battler::GetBattleShakeX() const
{
    if (shake_strength <= 0 || shake_speed <= 0 || shake_frames <= 0)
        return 0;
    int elapsed = Player::GetFrames() - shake_start_frame;
    if (elapsed < 0 || elapsed >= shake_frames)
        return 0;
    const int amplitude = shake_strength * 2;
    if (amplitude <= 0)
        return 0;
    int step = shake_strength * shake_speed / 10;
    if (step <= 0)
        step = 1;
    const int cycle = amplitude * 4;
    const int phase = (elapsed * step) % cycle;
    if (phase <= amplitude)
        return phase;
    if (phase <= amplitude * 3)
        return amplitude * 2 - phase;
    return phase - cycle;
}
