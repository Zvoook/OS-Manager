#pragma once
#include "Process.h"
#include "Resource.h"
#include <vector>

class GameState {
private:
    vector<Process> processes;
    vector<Resource> resources;
    vector<vector<int>> allocation;
    vector<int> available;
    int level;
    int wins;
    int losses;
public:
    GameState(int l = 1) : level(l), wins(0), losses(0) { init_level(level); }

    void init_level(int v);
    string action_result(int proc_id, int res_id, int k);
    bool deadlock_detect();
    bool is_lvl_passed() const;
    tuple<int, int, int> create_random_request();
    void set_manual(const vector<Resource>& res, const vector<Process>& procs, int count);

    void next_lvl() { init_level(level + 1); }
    void reset() { init_level(level); }
    void add_wins() { wins++; }
    void add_losses() { losses++; }

    const vector<Process>& get_vec_pr() const { return processes; }
    const vector<Resource>& get_vec_res() const { return resources; }
    int get_lvl() const { return level; }
    int get_wins() const { return wins; }
    int get_losses() const { return losses; }
};
