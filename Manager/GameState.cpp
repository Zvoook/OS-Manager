#include "GameState.h"
#include <random>
#include <ctime>
#include <algorithm>
void GameState::init_level(int lvl) {
    processes.clear();
    resources.clear();
    table.clear();
    available.clear();
    level = lvl;
    design_level();
}
bool GameState::is_lvl_passed() const {
    for (int i = 0; i < processes.size(); ++i) {
        if (!processes[i].is_done()) {
            return false;
        }
    }
    return true;
}