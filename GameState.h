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

class GameState {
private:
    std::string player_name;
    std::map<std::string, std::array<int, 3>> rating;

    vector<Process> processes;
    vector<Resource> resources;
    vector<vector<int>> allocation;
    vector<int> available;
    int level;
    int session_wins, session_losses; // ИЗМЕНЕНО: переименованы для ясности
    bool level_switching; // ДОБАВЛЕНО: флаг переключения уровня

public:
    GameState(vector<Process>& pr, vector<string>& res_names, int pr_cnt[], int res_cnt[],
        vector<float>& coeff, int l = 1) : level(l), session_wins(0), session_losses(0), level_switching(false)
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

    void add_wins() { session_wins++; } // ИЗМЕНЕНО: увеличиваем только сессионные
    void add_losses() { session_losses++; } // ИЗМЕНЕНО: увеличиваем только сессионные

    const vector<Process>& get_vec_pr() const { return processes; }
    const vector<Resource>& get_vec_res() const { return resources; }
    int get_lvl() const { return level; }

    // ИЗМЕНЕНО: теперь возвращаем глобальную статистику игрока
    int get_wins() const {
        if (!player_name.empty() && rating.find(player_name) != rating.end()) {
            return rating.at(player_name)[0]; // Глобальные победы
        }
        return session_wins; // Если игрок не установлен, показываем сессионные
    }

    int get_losses() const {
        if (!player_name.empty() && rating.find(player_name) != rating.end()) {
            return rating.at(player_name)[1]; // Глобальные поражения
        }
        return session_losses; // Если игрок не установлен, показываем сессионные
    }

    // ДОБАВЛЕНО: методы для получения сессионной статистики
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

    // ДОБАВЛЕНО: методы для работы с флагом переключения
    bool is_switching_level() const { return level_switching; }
    void set_switching(bool state) { level_switching = state; }
};