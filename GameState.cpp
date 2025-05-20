#include "GameState.h"
#include <iostream>
#include <fstream>



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
    ofstream file(filename);
    if (!file) return;

    file << level << " " << wins << " " << losses << "\n";

    file << available.size() << "\n";
    for (int a : available) file << a << " ";
    file << "\n";


    file << allocation.size() << " " << (allocation.empty() ? 0 : allocation[0].size()) << "\n";
    for (const auto& row : allocation) {
        for (int val : row) file << val << " ";
        file << "\n";
    }


    file << resources.size() << "\n";
    for (const auto& res : resources) {
        file << res.get_id() << "\n";
        file << res.get_name() << "\n";
        file << res.get_total() << " " << res.get_available() << "\n";
    }


    file << processes.size() << "\n";
    for (const auto& proc : processes) {
        file << proc.get_id() << "\n";
        file << proc.get_name() << "\n";

        const auto& max = proc.get_max_require();
        const auto& alloc = proc.get_alloc();
        file << max.size() << "\n";
        for (int val : max) file << val << " ";
        file << "\n";
        for (int val : alloc) file << val << " ";
        file << "\n";
    }
}

bool GameState::load_from_file(const std::string& filename) {
    ifstream file(filename);
    if (!file) return false;

    processes.clear();
    resources.clear();
    allocation.clear();
    available.clear();

    file >> level >> wins >> losses;

    int available_size;
    file >> available_size;
    available.resize(available_size);
    for (int i = 0; i < available_size; ++i)
        file >> available[i];

    int alloc_rows, alloc_cols;
    file >> alloc_rows >> alloc_cols;
    allocation.assign(alloc_rows, std::vector<int>(alloc_cols));
    for (int i = 0; i < alloc_rows; ++i)
        for (int j = 0; j < alloc_cols; ++j)
            file >> allocation[i][j];

    int res_count;
    file >> res_count;
    for (int i = 0; i < res_count; ++i) {
        int id, total, available_val;
        std::string name;
        file >> id;
        file.ignore(); // съесть \n
        getline(file, name);
        file >> total >> available_val;

        Resource res(id, name, total);
        res.set_available(available_val);
        resources.push_back(res);
    }

    int proc_count;
    file >> proc_count;
    for (int i = 0; i < proc_count; ++i) {
        int id, size;
        std::string name;
        file >> id;
        file.ignore();
        getline(file, name);
        file >> size;

        std::vector<int> max(size), alloc(size);
        for (int j = 0; j < size; ++j) file >> max[j];
        for (int j = 0; j < size; ++j) file >> alloc[j];

        Process proc(id, name, max);
        proc.set_alloc(alloc);
        processes.push_back(proc);
    }

    return true;
}


