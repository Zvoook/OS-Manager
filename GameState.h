#pragma once
#include "Process.h"
#include "Resource.h"
#include <vector>
#include <string>
#include <random>
#include <map>
#include <array>
#include <fstream>
#include <algorithm>
using namespace std;
class GameState {
private:
    string player_name;
    map<string, array<int, 3>> rating;
    vector<Process> processes;
    vector<Resource> resources;
    vector<vector<int>> allocation;
    vector<int> available;
    int level, wins, losses;
    int session_wins, session_losses;
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
    void release_finished();
    bool make_request(int proc_id, int res_id, int k);

    void add_wins() { wins++; }
    void add_losses() { losses++; }

    const vector<Process>& get_vec_pr() const { return processes; }
    const vector<Resource>& get_vec_res() const { return resources; }
    int get_lvl() const { return level; }
    int get_wins() const;
    int get_losses() const;
    int get_session_wins() const { return session_wins; }
    int get_session_losses() const { return session_losses; }

    void save_to_file(const string& filename) const;
    bool load_from_file(const string& filename);

    void set_player(const string& name);
    void save_rating(const string& path) const;
    void load_rating(const string& path);
    vector<tuple<string, int, int, int>> get_all_stats() const;
    void add_win();
    void add_loss();
    void update_max_level(int lvl);

    int get_max_available_level() const;
    bool has_player() const { return !player_name.empty(); }
    string get_player_name() const { return player_name; }
};