#include "Process.h"

vector<int> Process::get_rest() const {
    vector<int> d;
    // ДОБАВЛЕНО: Проверка на соответствие размеров
    if (max_require.size() != alloc.size()) {
        // Возвращаем пустой вектор при несоответствии размеров
        return d;
    }

    for (int i = 0; i < max_require.size(); ++i) {
        // ДОБАВЛЕНО: Защита от отрицательных значений
        d.push_back(max(0, max_require[i] - alloc[i]));
    }
    return d;
}

void Process::set_alloc(const vector<int>& res) {
    if (res.size() == alloc.size()) {
        alloc = res;
        done = true;
        for (int i = 0; i < max_require.size(); ++i) {
            if (alloc[i] < max_require[i]) {
                done = false;
                break;
            }
        }
    }
}

void Process::add_res(int res_id, int amount) {
    if (res_id >= 0 && res_id < alloc.size()) {
        alloc[res_id] += amount;
        done = true;
        for (int i = 0; i < max_require.size(); ++i) {
            if (alloc[i] < max_require[i]) {
                done = false;
                break;
            }
        }
    }
}