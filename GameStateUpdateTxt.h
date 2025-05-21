#pragma once
#include "Process.h"
#include "Resource.h"
#include <vector>
#include <string>

class GameState {
private:
    std::vector<Process> processes;
    std::vector<Resource> resources;
    std::vector<std::vector<int>> allocation;
    std::vector<int> available;
    int level;
    int wins;
    int losses;

public:
    GameState();
    void init_level(int v);

    //запрос ресурса
    std::string action_result(int proc_id, int res_id, int k);
    void set_manual(const std::vector<Resource>& res, const std::vector<Process>& proc, int count);

    // Проверки
    bool deadlock_detect();
    bool is_lvl_passed() const;

    // Сохранение и загрузка
    void save_to_txt(const std::string& filename) const;
    bool load_from_txt(const std::string& filename);

    // Статистика
    bool check_win();
    bool check_loss();
    void increment_wins();
    void increment_losses();

    // гетте(о)ры
    const std::vector<Process>& get_vec_pr() const { return processes; }
    const std::vector<Resource>& get_vec_res() const { return resources; }
    int get_lvl() const { return level; }
    int get_wins() const { return wins; }
    int get_losses() const { return losses; }

    // Доп. действия
    void next_lvl() { init_level(level + 1); }
    void reset() { init_level(level); }
};
