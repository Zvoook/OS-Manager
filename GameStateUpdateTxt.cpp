#include "GameState.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

void GameState::init_level(int v) {
    level = v;
    processes.clear();
    resources.clear();
    allocation.clear();
    available.clear();
}

bool GameState::is_lvl_passed() const {
    for (const auto& proc : processes) {
        if (!proc.is_done()) return false;
    }
    return true;
}

std::string GameState::action_result(int proc_id, int res_id, int k) {
    std::string mes;
    Process& proc = processes[proc_id];
    int need = proc.get_max_require()[res_id] - proc.get_alloc()[res_id];

    if (k > need || k > available[res_id]) {
        mes += "Invalid request!";
        return mes;
    }

    available[res_id] -= k;
    allocation[proc_id][res_id] += k;
    proc.add_res(res_id, k);
    resources[res_id].take(k);

    if (deadlock_detect()) {
        mes += "Resource successfully granted!";
        if (proc.is_done()) {
            for (int i = 0; i < resources.size(); ++i) {
                int allocated = allocation[proc_id][i];
                available[i] += allocated;
                resources[i].back(allocated);
                allocation[proc_id][i] = 0;
            }
            mes += "\n\nProcess completed!";
        }

        if (check_win()) {
            increment_wins();
            mes += "\n\n🎉 Congratulations! You've completed the game!";
        }

    } else {
        // Откатываем изменения
        available[res_id] += k;
        allocation[proc_id][res_id] -= k;
        proc.add_res(res_id, -k);
        resources[res_id].back(k);
        mes += "You face a deadlock!";
        
        if (check_loss()) {
            increment_losses();
            mes += "\n\n💀 Game over! Deadlock detected and no safe path found.";
        }
    }

    return mes;
}

bool GameState::deadlock_detect() {
    int num_processes = processes.size();
    int num_resources = resources.size();

    std::vector<int> work = available;
    std::vector<bool> finish(num_processes, false);
    std::vector<std::vector<int>> need(num_processes, std::vector<int>(num_resources));

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
        if (!f) return false;
    }
    return true;
}

std::tuple<int, int, int> GameState::create_random_request() {
    for (int i = 0; i < processes.size(); ++i) {
        if (!processes[i].is_done()) {
            auto rest = processes[i].get_rest();
            for (int j = 0; j < rest.size(); ++j) {
                if (rest[j] > 0 && available[j] > 0) {
                    int amount = std::min({1, rest[j], available[j]});
                    return {i, j, amount};
                }
            }
        }
    }
    return {-1, -1, -1};
}

void GameState::set_manual(const std::vector<Resource>& res, const std::vector<Process>& proc, int count) {
    int pc = std::min(count, (int)proc.size()), rc = std::min(count, (int)res.size());
    for (int i = 0; i < rc; ++i) resources.push_back(res[i]);
    for (int i = 0; i < pc; ++i) processes.push_back(proc[i]);
    available.resize(rc);
    allocation.assign(pc, std::vector<int>(rc, 0));
    for (int i = 0; i < rc; ++i) available[i] = resources[i].get_available();
}

// Сохранение состояния в .txt
void GameState::save_to_txt(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }

    // level, wins, losses
    file << "level: " << level << "\n";
    file << "wins: " << wins << "\n";
    file << "losses: " << losses << "\n\n";

    // available resources
    file << "available: ";
    for (int val : available) file << val << " ";
    file << "\n\n";

    // allocation matrix
    file << "allocation:\n";
    for (const auto& row : allocation) {
        for (int val : row) file << val << " ";
        file << "\n";
    }
    file << "\n";

    // resources
    file << "resources:\n";
    for (const auto& res : resources) {
        file << res.get_id() << " "
             << res.get_name() << " "
             << res.get_total() << " "
             << res.get_available() << "\n";
    }
    file << "\n";

    // processes
    file << "processes:\n";
    for (const auto& proc : processes) {
        file << proc.get_id() << " "
             << proc.get_name() << " ";
        const auto& req = proc.get_max_require();
        for (int val : req) file << val << " ";
        file << "\n";
    }

    file.close();
}

// Загрузка состояния из .txt
bool GameState::load_from_txt(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    processes.clear();
    resources.clear();
    allocation.clear();
    available.clear();

    std::string line;

    // level, wins, losses
    std::getline(file, line); sscanf(line.c_str(), "level: %d", &level);
    std::getline(file, line); sscanf(line.c_str(), "wins: %d", &wins);
    std::getline(file, line); sscanf(line.c_str(), "losses: %d", &losses);

    // empty line
    std::getline(file, line);

    // available
    std::getline(file, line);
    std::istringstream avail_ss(line.substr(9));
    int val;
    while (avail_ss >> val) available.push_back(val);

    // empty line
    std::getline(file, line);

    // allocation
    std::getline(file, line); // skip header
    while (true) {
        std::getline(file, line);
        if (line.empty()) break;
        std::istringstream alloc_ss(line);
        std::vector<int> row;
        while (alloc_ss >> val) row.push_back(val);
        allocation.push_back(row);
    }

    // resources
    std::getline(file, line); // skip header
    while (true) {
        std::getline(file, line);
        if (line.empty()) break;
        std::istringstream res_ss(line);
        int id, total, available_res;
        std::string name;
        res_ss >> id >> name >> total >> available_res;
        Resource res(id, name, total);
        res.set_available(available_res);
        resources.push_back(res);
    }

    // processes
    std::getline(file, line); // skip header
    while (std::getline(file, line) && !line.empty()) {
        std::istringstream proc_ss(line);
        int id;
        std::string name;
        std::vector<int> max_require;
        proc_ss >> id >> name;
        while (proc_ss >> val) max_require.push_back(val);
        Process proc(id, name, max_require);
        processes.push_back(proc);
    }

    file.close();
    return true;
}

// Проверка на победу
bool GameState::check_win() {
    return is_lvl_passed();
}

// Проверка на проигрыш: если все процессы заблокированы
bool GameState::check_loss() {
    for (int i = 0; i < processes.size(); ++i) {
        if (!processes[i].is_done()) {
            auto rest = processes[i].get_rest();
            for (int j = 0; j < rest.size(); ++j) {
                if (rest[j] <= available[j]) return false; // Есть возможность выделить ресурсы
            }
        }
    }
    return true; // Все процессы не могут продолжиться — deadlock
}

// Статистика
void GameState::increment_wins() {
    wins++;
}

void GameState::increment_losses() {
    losses++;
}
