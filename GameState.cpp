#include "GameState.h"
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


bool GameState::make_request(int proc_id, int res_id, int k) {
    Process& proc = processes[proc_id];
    int need = proc.get_max_require()[res_id] - proc.get_alloc()[res_id];

    if (k > need) {
        //cerr << "������ ��������� ����������� ��������.\n";
        return false;
    }
    if (k > available[res_id]) {
        //cerr << "������������ ��������� ��������.\n";
        return false;
    }
    available[res_id] -= k;
    allocation[proc_id][res_id] += k;
    proc.add_res(res_id, k);
    resources[res_id].take(k);

    if (deadlock_detect()) {
        //cout << "������ ������";
        if (proc.is_done()) {
            // ����������� ������� ��������
            for (int i = 0; i < resources.size(); ++i) {
                int allocated = allocation[proc_id][i];
                available[i] += allocated;
                resources[i].back(allocated);
                allocation[proc_id][i] = 0;
            }
            //cout << "������� " << proc_id << " �������� � ��������� ���� �������.\n";
        }

        return true;
    }
    else {
        available[res_id] += k;
        allocation[proc_id][res_id] -= k;
        proc.add_res(res_id, -k);
        resources[res_id].back(k);
        //cerr << "������ ��������\n";
        return false;
    }
}

//tupe<int, int, int> GameState::get_request(int proc_id, int res_id, int k)
//{
//    if (make_request(proc_id, res_id, k)) return tupe<int, int, int>(proc_id, res_id, k);
//    return tupe<int, int, int>(-1, 0, 0);
//}

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