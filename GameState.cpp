#include "GameState.h"
#include <iostream>
#include "json.hpp"
#include <fstream>
using json = nlohmann::json;

void GameState::init_level(int v) {
    level = v;
    processes.clear();
    resources.clear();
    allocation.clear();
    available.clear();
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

    if (k > need || k > available[res_id]) return NULL;
    available[res_id] -= k;
    allocation[proc_id][res_id] += k;
    proc.add_res(res_id, k);
    resources[res_id].take(k);

    if (deadlock_detect()) {
        mes += "Resource successful granted!";
        if (proc.is_done()) {
            for (int i = 0; i < resources.size(); ++i) {
                int allocated = allocation[proc_id][i];
                available[i] += allocated;
                resources[i].back(allocated);
                allocation[proc_id][i] = 0;
            }
            mes += "\n\nProcess completed!";
        }
    }
    else {
        available[res_id] += k;
        allocation[proc_id][res_id] -= k;
        proc.add_res(res_id, -k);
        resources[res_id].back(k);
        mes += "You face to deadlock!";
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
    for (int i = 0; i < processes.size(); ++i) {
        if (!processes[i].is_done()) {
            auto rest = processes[i].get_rest();
            for (int j = 0; j < rest.size(); ++j) {
                if (rest[j] > 0 && available[j] > 0) {
                    int amount = min(1, min(rest[j], available[j]));
                    return { i, j, amount };
                }
            }
        }
    }
    return { -1, -1, -1 };
}

void GameState::set_manual(const vector<Resource>& res, const vector<Process>& proc, int count) {
    for (int i = 0; i < count && i < res.size(); ++i) resources.push_back(res[i]);
    for (int i = 0; i < count; ++i) processes.push_back(proc[i]);
    int r = resources.size(), p = processes.size();
    available.resize(r);
    allocation.assign(p, vector<int>(r, 0));
    for (int i = 0; i < r; ++i) available[i] = resources[i].get_available();
}

void GameState::save_to_file(const std::string& filename) const {
    json j;
    j["level"] = level;
    j["wins"] = wins;
    j["losses"] = losses;

    j["available"] = available;
    j["allocation"] = allocation;

    j["resources"] = json::array();
    for (const auto& res : resources)
        j["resources"].push_back({ {"id", res.get_id()}, {"name", res.get_name()}, {"total", res.get_total()}, {"available", res.get_available()} });

    j["processes"] = json::array();
    for (const auto& proc : processes)
        j["processes"].push_back({ {"id", proc.get_id()}, {"name", proc.get_name()}, {"max_require", proc.get_max_require()}, {"alloc", proc.get_alloc()} });

    std::ofstream file(filename);
    if (file) file << j.dump(4);
}

bool GameState::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;

    json j;
    file >> j;

    level = j["level"];
    wins = j["wins"];
    losses = j["losses"];
    available = j["available"].get<std::vector<int>>();
    allocation = j["allocation"].get<std::vector<std::vector<int>>>();

    resources.clear();
    for (const auto& resj : j["resources"]) {
        Resource res(resj["id"], resj["name"], resj["total"]);
        res.set_available(resj["available"]);
        resources.push_back(res);
    }

    processes.clear();
    for (const auto& procj : j["processes"]) {
        Process proc(procj["id"], procj["name"], procj["max_require"]);
        proc.set_alloc(procj["alloc"]);
        processes.push_back(proc);
    }

    return true;
}

