#pragma once
#include "Process.h"
#include "Resource.h"
#include <vector>
class GameState {
private:
    std::vector<Process> processes;
    std::vector<Resource> resources;
    std::vector<std::vector<int>> allocation;
    std::vector<int> available;               
    int level;
    int wins;
    int losses;
public:
    GameState() : level(1), wins(0), losses(0) { init_level(level); }
    const std::vector<Process>& get_vec_pr() const { return processes; }
    const std::vector<Resource>& get_vec_res() const { return resources; }
    int get_lvl() const { return level; }
    int get_wins() const { return wins; }
    int get_losses() const { return losses; }
    void init_level(int lvl);
    //void design_level();
    bool make_request(int proc_id, int res_id, int k);
    bool deadlock_detect();
    bool is_lvl_passed() const;
    void next_lvl() { init_level(level + 1); }
    void reset() { init_level(level); }
    void add_wins() { wins++; }
    void add_losses() { losses++; }
};