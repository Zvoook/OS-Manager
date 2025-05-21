#pragma once
#include "Process.h"
#include "Resource.h"
#include <vector>
#include <string>
#include <random>

class GameState {
private:
    vector<Process> processes;
    vector<Resource> resources;
    vector<vector<int>> allocation;
    vector<int> available;
    int level, wins, losses;
public:
    GameState(vector<Process>& pr, vector<string>& res_names, int pr_cnt[], int res_cnt[],
        vector<float>& coeff, int l = 1) : level(l), wins(0), losses(0)
    {
        init_level(pr, res_names, pr_cnt, res_cnt, coeff, level);
    }

    void init_level(vector<Process>& pr, vector<string>& res_names, int pr_cnt[], int res_cnt[], vector<float>& coeff, int l);
    string action_result(int proc_id, int res_id, int k);
    bool deadlock_detect();
    bool is_lvl_passed() const;
    tuple<int, int, int> create_random_request();
    void set_level(int i) { level = i; }
    //void set_manual(const vector<Resource>& res, const vector<Process>& procs, int count);
    void release_finished();
    bool make_request(int proc_id, int res_id, int k);

    void add_wins() { wins++; }
    void add_losses() { losses++; }

    const vector<Process>& get_vec_pr() const { return processes; }
    const vector<Resource>& get_vec_res() const { return resources; }
    int get_lvl() const { return level; }
    int get_wins() const { return wins; }
    int get_losses() const { return losses; }

    /*void save_to_file(const string& filename) const;
    bool load_from_file(const string& filename);*/
};
