#include "GameState.h"
void GameState::initLevel1() {
    resources = {
        Resource("Resource 1", 6),
        Resource("Resource 2", 3)
    };
    processes = {
        Process("Process 1", {5, 2}),
        Process("Process 2", {2, 3}),
        Process("Process 3", {6, 3}),
        Process("Process 4", {1, 1})
    };
}
void GameState::reset_res() {
    for (auto& res : resources) {
        res.reset();
    }
}
void GameState::reset_proc() {
    for (auto& proc : processes) {
        proc.reset();
    }
}
