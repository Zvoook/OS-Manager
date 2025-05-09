#include "GameState.h"
#include <iostream>

void GameState::init_level(int lvl) {
    level = lvl;
    processes.clear();
    resources.clear();
    allocation.clear();
    available.clear();  
}
bool GameState::is_lvl_passed() const {
    for (int i = 0; i < processes.size(); ++i) {
        if (!processes[i].is_done()) {
            return false;
        }
    }
    return true;
}


bool GameState::make_request(int proc_id, int res_id, int k) {
    Process& proc = processes[proc_id];
    int need = proc.get_max_require()[res_id] - proc.get_alloc()[res_id];

    if (k > need) {
        std::cerr << "Запрос превышает потребности процесса.\n";
        return false;
    }
    if (k > available[res_id]) {
        std::cerr << "Недостаточно доступных ресурсов.\n";
        return false;
    }
    available[res_id] -= k;
    allocation[proc_id][res_id] += k;
    proc.add_res(res_id, k);
    resources[res_id].take(k);

    if (deadlock_detect()) {

        std::cout << "Запрос принят";
        if (proc.is_done()) {
            // Освобождаем ресурсы процесса
            for (int i = 0; i < resources.size(); ++i) {
                int allocated = allocation[proc_id][i];
                available[i] += allocated;
                resources[i].back(allocated);
                allocation[proc_id][i] = 0;
            }
            std::cout << "Процесс " << proc_id << " завершен и освободил свои ресурсы.\n";
        }

        return true;
    }
    else {
        available[res_id] += k;
        allocation[proc_id][res_id] -= k;
        proc.add_res(res_id, -k);
        resources[res_id].back(k);
        std::cerr << "Запрос отклонен\n";
        return false;
    }
}

//банкир
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
        if (!f) {
            return false;
        }
    }
    return true; 
}

