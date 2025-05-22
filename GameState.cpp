#include "GameState.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <tuple>

void GameState::init_level(vector<Process>& pr, vector<string>& res_names, int pr_cnt[], int res_cnt[], vector<float>& coeff, int l) {
    level = l;
    processes.clear();
    resources.clear();
    allocation.clear();
    available.clear();

    int p_count = pr_cnt[level - 1];
    int r_count = res_cnt[level - 1];

    int total_procs = pr.size();
    int safe_count = min(p_count, total_procs);
    processes.assign(pr.begin(), pr.begin() + safe_count);

    vector<vector<int>> max_require;
    for (int i = 0; i < safe_count; ++i) {
        max_require.push_back(processes[i].get_max_require());
    }

    vector<int> min_need(r_count, 0);
    for (int i = 0; i < r_count; ++i) {
        for (int j = 0; j < p_count; ++j) {
            if (j < safe_count && min_need[i] < max_require[j][i])
                min_need[i] = max_require[j][i];
        }
    }

    vector<int> res_sums(r_count, 0);
    for (const auto& m : max_require) {
        for (int j = 0; j < r_count; ++j) {
            res_sums[j] += m[j];
        }
    }

    for (int j = 0; j < r_count; ++j) {
        bool used = false;
        for (int i = 0; i < safe_count; ++i) {
            if (processes[i].get_max_require()[j] > 0) {
                used = true;
                break;
            }
        }
        if (used && res_sums[j] == 0) res_sums[j] = 1;
    }

    for (int i = 0; i < r_count; ++i) {
        int base = static_cast<int>(res_sums[i] * coeff[level - 1]);
        int variation = (rand() % 3) - 1;
        int total = max(1, base + variation);
        total = max(total, min_need[i]);
        resources.emplace_back(i, res_names[i], total);
        resources[i].set_available(total);
        available.push_back(total);
    }
    allocation.assign(safe_count, vector<int>(r_count, 0));
}

bool GameState::is_lvl_passed() const {
    for (int i = 0; i < processes.size(); ++i) {
        if (!processes[i].is_done()) return false;
    }
    return true;
}

string GameState::action_result(int proc_id, int res_id, int k) {
    string mes;
    if (proc_id < 0 || proc_id >= processes.size() ||
        res_id < 0 || res_id >= resources.size() || k <= 0) {
        return "Invalid request parameters";
    }

    Process& proc = processes[proc_id];
    int need = proc.get_max_require()[res_id] - proc.get_alloc()[res_id];

    if (k > need || k > available[res_id]) return "";

    available[res_id] -= k;
    allocation[proc_id][res_id] += k;
    proc.add_res(res_id, k);
    resources[res_id].take(k);

    if (deadlock_detect()) {
        mes += "Resource successful granted";
        if (proc.is_done()) {
            for (int i = 0; i < resources.size(); ++i) {
                int allocated = allocation[proc_id][i];
                available[i] += allocated;
                resources[i].back(allocated);
                allocation[proc_id][i] = 0;
            }
            mes += "\n\nPROCESS COMPLETED";
        }
    }
    else {
        available[res_id] += k;
        allocation[proc_id][res_id] -= k;
        proc.add_res(res_id, -k);
        resources[res_id].back(k);
        mes += "You face to deadlock\n\nGAME OVER";
    }
    return mes;
}

bool GameState::deadlock_detect() {
    int num_processes = processes.size();
    int num_resources = resources.size();

    vector<int> work = available;
    vector<bool> finish(num_processes, false);
    vector<vector<int>> need(num_processes, vector<int>(num_resources));

    for (int i = 0; i < num_processes; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            need[i][j] = processes[i].get_max_require()[j] - allocation[i][j];
        }
    }

    bool progress = true;
    while (progress) {
        progress = false;
        for (int i = 0; i < num_processes; ++i) {
            if (!finish[i]) {
                bool can_finish = true;
                for (int j = 0; j < num_resources; ++j) {
                    if (need[i][j] > work[j]) {
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish) {
                    for (int j = 0; j < num_resources; ++j) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    progress = true;
                }
            }
        }
    }

    for (bool f : finish) {
        if (!f) {
            return false;
        }
    }
    return true;
}

tuple<int, int, int> GameState::create_random_request() {
    int p_size = processes.size();
    int tries = 20;

    while (tries--) {
        int pid = rand() % p_size;
        if (processes[pid].is_done()) continue;

        const auto& rest = processes[pid].get_rest();
        for (int j = 0; j < rest.size(); ++j) {
            if (rest[j] > 0 && available[j] > 0) {
                int max_req = min(rest[j], available[j]);
                int amount = 1 + rand() % max_req;
                return { pid, j, amount };
            }
        }
    }

    return { -1, -1, -1 };
}

bool GameState::make_request(int proc_id, int res_id, int k) {
    if (proc_id < 0 || proc_id >= processes.size() ||
        res_id < 0 || res_id >= resources.size() || k <= 0) {
        return false;
    }

    Process& proc = processes[proc_id];
    int need = proc.get_max_require()[res_id] - proc.get_alloc()[res_id];

    if (k > need || k > available[res_id])
        return false;

    available[res_id] -= k;
    allocation[proc_id][res_id] += k;
    proc.add_res(res_id, k);
    resources[res_id].take(k);

    if (deadlock_detect()) {
        if (proc.is_done()) {
            for (int i = 0; i < resources.size(); ++i) {
                int alloc = allocation[proc_id][i];
                available[i] += alloc;
                resources[i].back(alloc);
                allocation[proc_id][i] = 0;
            }
        }
        return true;
    }
    else {
        available[res_id] += k;
        allocation[proc_id][res_id] -= k;
        proc.add_res(res_id, -k);
        resources[res_id].back(k);
        return false;
    }
}

void GameState::release_finished() {
    for (int pid = 0; pid < processes.size(); ++pid) {
        if (processes[pid].is_done()) {
            for (int i = 0; i < resources.size(); ++i) {
                int alloc = allocation[pid][i];
                available[i] += alloc;
                resources[i].back(alloc);
                allocation[pid][i] = 0;
            }
        }
    }
}

void GameState::save_to_file(const string& filename) const {
    ofstream file(filename);
    if (!file) return;

    try {
        file << level << " " << wins << " " << losses << endl;
        file << available.size() << endl;
        for (size_t i = 0; i < available.size(); ++i) {
            file << available[i];
            if (i < available.size() - 1) file << " ";
        }
        file << endl;
        file << processes.size() << " " << resources.size() << endl;
        for (size_t i = 0; i < allocation.size(); ++i) {
            for (size_t j = 0; j < allocation[i].size(); ++j) {
                file << allocation[i][j];
                if (j < allocation[i].size() - 1) file << " ";
            }
            file << endl;
        }

        file << resources.size() << endl;
        for (const auto& res : resources) {
            file << res.get_id() << " ";
            file << res.get_name() << " ";
            file << res.get_total() << " ";
            file << res.get_available() << endl;
        }

        file << processes.size() << endl;
        for (const auto& proc : processes) {
            file << proc.get_id() << " ";
            file << proc.get_name() << " ";

            const auto& max_req = proc.get_max_require();
            file << max_req.size() << " ";
            for (size_t i = 0; i < max_req.size(); ++i) {
                file << max_req[i];
                if (i < max_req.size() - 1) file << " ";
            }
            file << " ";

            const auto& alloc = proc.get_alloc();
            file << alloc.size() << " ";
            for (size_t i = 0; i < alloc.size(); ++i) {
                file << alloc[i];
                if (i < alloc.size() - 1) file << " ";
            }
            file << " ";

            file << (proc.is_done() ? 1 : 0) << endl;
        }

        file.flush(); 
    }
    catch (...) {
        cout << "Error saving game to file: " << filename << endl;
    }
}

bool GameState::load_from_file(const string& filename) {
    ifstream file(filename);
    if (!file) return false;

    try {
        file >> level >> wins >> losses;

        int avail_size;
        file >> avail_size;
        if (avail_size <= 0 || avail_size > 10) { 
            return false;
        }

        available.clear();
        available.resize(avail_size);
        for (int i = 0; i < avail_size; ++i) {
            file >> available[i];
            if (available[i] < 0) return false; 
        }

        int proc_count, res_count;
        file >> proc_count >> res_count;

        if (proc_count <= 0 || proc_count > 10 || res_count <= 0 || res_count > 10) {
            return false;
        }
        if (res_count != avail_size) {
            return false;
        }

        allocation.clear();
        allocation.resize(proc_count, vector<int>(res_count, 0));

        for (int i = 0; i < proc_count; ++i) {
            for (int j = 0; j < res_count; ++j) {
                file >> allocation[i][j];
                if (allocation[i][j] < 0) return false;
            }
        }

        int res_size;
        file >> res_size;
        if (res_size != res_count) return false;

        resources.clear();
        resources.reserve(res_size);

        for (int i = 0; i < res_size; ++i) {
            int id, total, avail;
            string name;
            file >> id >> name >> total >> avail;
            if (total < 0 || avail < 0 || avail > total) {
                return false;
            }

            Resource res(id, name, total);
            res.set_available(avail);
            resources.push_back(res);
        }

        int proc_size;
        file >> proc_size;
        if (proc_size != proc_count) return false;

        processes.clear();
        processes.reserve(proc_size);

        for (int i = 0; i < proc_size; ++i) {
            int id, max_size, alloc_size;
            string name;
            bool done;

            file >> id >> name;
            file >> max_size;

            if (max_size != res_count) return false; 

            vector<int> max_req(max_size);
            for (int j = 0; j < max_size; ++j) {
                file >> max_req[j];
                if (max_req[j] < 0) return false;
            }

            file >> alloc_size;
            if (alloc_size != res_count) return false; 

            vector<int> alloc(alloc_size);
            for (int j = 0; j < alloc_size; ++j) {
                file >> alloc[j];
                if (alloc[j] < 0) return false;
            }

            file >> done;
            Process proc(id, name, max_req);
            proc.set_alloc(alloc);
            proc.set_done(done);
            processes.push_back(proc);
        }
        if (processes.size() != allocation.size()) {
            return false;
        }

        for (size_t i = 0; i < processes.size(); ++i) {
            if (processes[i].get_max_require().size() != resources.size() ||
                processes[i].get_alloc().size() != resources.size() ||
                allocation[i].size() != resources.size()) {
                return false;
            }
        }

        return true;
    }
    catch (...) {
        return false;
    }
}

void GameState::set_player(const string& name) {
    player_name = name;
    if (rating.find(name) == rating.end())
        rating[name] = { 0, 0, 1 };
}

void GameState::save_rating(const string& path) const {
    map<string, array<int, 3>> existing;
    ifstream infile(path);
    if (infile.is_open()) {
        string name;
        int w, l, m;
        while (infile >> name >> w >> l >> m) {
            existing[name] = { w, l, m };
        }
        infile.close();
    }
    if (!player_name.empty()) {
        existing[player_name] = rating.at(player_name);
    }
    ofstream out(path, ios::trunc);
    for (const auto& pair : existing) {
        const string& name = pair.first;
        const array<int, 3>& stats = pair.second;
        out << name << " " << stats[0] << " " << stats[1] << " " << stats[2] << endl;
    }
}

void GameState::load_rating(const string& path) {
    ifstream file(path);
    if (!file.is_open()) return;
    rating.clear();
    string name;
    int wins, losses, max_lvl;
    while (file >> name >> wins >> losses >> max_lvl) {
        rating[name] = { wins, losses, max_lvl };
    }
}

vector<tuple<string, int, int, int>> GameState::get_all_stats() const {
    vector<tuple<string, int, int, int>> stats;
    for (const auto& pair : rating) {
        const string& name = pair.first;
        const array<int, 3>& data = pair.second;
        stats.emplace_back(name, data[0], data[1], data[2]);
    }
    sort(stats.begin(), stats.end(),
        [](const auto& a, const auto& b) {
            int level_a = get<3>(a), level_b = get<3>(b);
            int wins_a = get<1>(a), wins_b = get<1>(b);
            if (level_a != level_b) return level_a > level_b;
            return wins_a > wins_b;
        });
    return stats;
}
int GameState::get_wins() const {
    if (!player_name.empty() && rating.find(player_name) != rating.end()) {
        return rating.at(player_name)[0]; 
    }
    return session_wins; 
}

int GameState::get_losses() const {
    if (!player_name.empty() && rating.find(player_name) != rating.end()) {
        return rating.at(player_name)[1]; 
    }
    return session_losses; 
}
void GameState::add_win() {
    session_wins++; 
    if (!player_name.empty()) {
        rating[player_name][0]++; 
    }
}

void GameState::add_loss() {
    session_losses++; 
    if (!player_name.empty()) {
        rating[player_name][1]++; 
    }
}

void GameState::update_max_level(int lvl) {
    if (!player_name.empty()) {
        rating[player_name][2] = max(rating[player_name][2], lvl + 1);
    }
}

int GameState::get_max_available_level() const {
    if (player_name.empty()) {
        return 0;
    }
    auto it = rating.find(player_name);
    if (it != rating.end()) {
        return min(4, it->second[2]);
    }
    return 1;
}