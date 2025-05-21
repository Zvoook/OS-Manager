#include "GameState.h"
#include <iostream>
#include <fstream>
#include <algorithm>
//#include "json.hpp"
//using json = nlohmann::json;

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
    for (int i = 0; i < safe_count; ++i) max_require.push_back(processes[i].get_max_require());

    vector<int> min_need(r_count, 0);
    for (int i = 0; i < r_count; ++i) {
        for (int j = 0; j < p_count; ++j) {
            if (min_need[i] < max_require[j][i]) min_need[i] = max_require[j][i];
        }
    }

    vector<int> res_sums(r_count, 0);
    for (const auto& m : max_require) {
        for (int j = 0; j < r_count; ++j) res_sums[j] += m[j];
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
    vector<bool> Fillnish(num_processes, false);
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
            if (!Fillnish[i]) {
                bool can_Fillnish = true;
                for (int j = 0; j < num_resources; ++j) {
                    if (need[i][j] > work[j]) {
                        can_Fillnish = false;
                        break;
                    }
                }
                if (can_Fillnish) {
                    for (int j = 0; j < num_resources; ++j) {
                        work[j] += allocation[i][j];
                    }
                    Fillnish[i] = true;
                    progress = true;
                }
            }
        }
    }

    for (bool f : Fillnish) {
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


//void GameState::save_to_file(const std::string& filename) const {
//    json j;
//    j["level"] = level;
//    j["wins"] = wins;
//    j["losses"] = losses;
//
//    j["available"] = available;
//    j["allocation"] = allocation;
//
//    j["resources"] = json::array();
//    for (const auto& res : resources)
//        j["resources"].push_back({ {"id", res.get_id()}, {"name", res.get_name()}, {"total", res.get_total()}, {"available", res.get_available()} });
//
//    j["processes"] = json::array();
//    for (const auto& proc : processes)
//        j["processes"].push_back({ {"id", proc.get_id()}, {"name", proc.get_name()}, {"max_require", proc.get_max_require()}, {"alloc", proc.get_alloc()} });
//
//    std::ofstream file(filename);
//    if (file) file << j.dump(4);
//}
//
//bool GameState::load_from_file(const std::string& filename) {
//    std::ifstream file(filename);
//    if (!file) return false;
//
//    json j;
//    file >> j;
//
//    level = j["level"];
//    wins = j["wins"];
//    losses = j["losses"];
//    available = j["available"].get<std::vector<int>>();
//    allocation = j["allocation"].get<std::vector<std::vector<int>>>();
//
//    resources.clear();
//    for (const auto& resj : j["resources"]) {
//        Resource res(resj["id"], resj["name"], resj["total"]);
//        res.set_available(resj["available"]);
//        resources.push_back(res);
//    }
//
//    processes.clear();
//    for (const auto& procj : j["processes"]) {
//        Process proc(procj["id"], procj["name"], procj["max_require"]);
//        proc.set_alloc(procj["alloc"]);
//        processes.push_back(proc);
//    }
//
//    return true;
//}
