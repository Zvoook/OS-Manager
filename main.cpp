#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include "Constants.h"
#include "GameState.h"
#include "Design.h"

using namespace sf;
using namespace std;

// Обрезка строки до ширины width
string fix_str(const string& text, int width) {
    if (text.length() >= size_t(width))
        return text.substr(0, width - 1) + "~";
    return text + string(width - text.length(), ' ');
}

// Обновление строки статистики
void updateStatistic(Text& t, const GameState& g) {
    t.setString("Level: " + to_string(g.get_lvl()) +
        "   Wins: " + to_string(g.get_wins()) +
        "   Losses: " + to_string(g.get_losses()));
}

// Создание текста для ресурса
Text create_res_text(int row, const Resource& r, Font& f) {
    Text t("", f, 24);
    t.setFillColor(black);
    string line = fix_str(r.get_name(), RES_NAME_WIDTH) + SEP +
        fix_str(to_string(r.get_available()) + "/" +
            to_string(r.get_total()), CELL_WIDTH);
    t.setString(line);
    t.setPosition(FRAME_X_RES + 20,
        FRAME_Y_RES + RES_OFFSET + row * RES_STEP);
    return t;
}

// Создание текста для процесса
Text create_proc_text(int row, int resCnt, const Process& p, Font& f) {
    Text t("", f, 24);
    t.setFillColor(black);
    string line = fix_str(p.get_name(), PROC_NAME_WIDTH);
    const auto& alloc = p.get_alloc();
    const auto& max = p.get_max_require();
    for (int i = 0; i < resCnt; ++i) {
        int need = max[i] - alloc[i];
        line += need
            ? fix_str(to_string(need), CELL_WIDTH)
            : fix_str("-", CELL_WIDTH);
    }
    t.setString(line);
    t.setPosition(FRAME_X_PROC + 20,
        FRAME_Y_PROC + PROC_OFFSET + row * PROC_STEP);
    return t;
}

// Заголовок таблицы процессов
Text makeHeader(const vector<string>& names, Font& f, int c) {
    Text h("", f, 22);
    h.setFillColor(black);
    string line = fix_str("", PROC_NAME_WIDTH);
    for (int i = 0; i < c && i < (int)names.size(); ++i)
        line += fix_str(names[i], CELL_WIDTH);
    h.setString(line);
    h.setPosition(FRAME_X_PROC + 48.5f,
        FRAME_Y_PROC + PROC_OFFSET - 25);
    return h;
}

int main() {
    srand(time(0));
    // Загрузка шрифта и иконки
    Font font;
    Image icon;
    if (!font.loadFromFile("Font.ttf") || !icon.loadFromFile("man.jpg"))
        return 1;

    // Окно
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
        "Resource Manager", Style::Close);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Состояние игры и UI
    GameState game;
    LevelDesign ui(font);
    Menu menu(font);

    // Тексты
    Text stat("", font, 20);
    stat.setFillColor(black);
    stat.setPosition(57, 70);

    Text info("", font, 21);
    info.setFillColor(black);
    info.setPosition(430, 160);

    Text menu_info(
        "It's the OS operator game. This game simulate actual OS working.\n"
        "You should grant or deny requests from processes and avoid deadlock.",
        font, 19);
    menu_info.setFillColor(black);
    menu_info.setPosition(INFO_X + 30, INFO_Y + 70);

    // Режим: меню или игра
    bool in_menu = true;
    bool levelSelected = false;

    // Источники ресурсов и процессов (по умолчанию)
    vector<string> res_names = { "CPU", "RAM", "DISK", "NET", "GPU" };
    vector<Resource> resources = {
        {0, "CPU",  8}, {1, "RAM", 16}, {2, "DISK", 6},
        {3, "NET",  4}, {4, "GPU",  8}
    };
    vector<string> proc_names = {
        "Notepad", "Explorer", "Compiler", "Discord", "Minecraft", "Chrome"
    };
    vector<Process> processes = {
        {0, proc_names[0], {0,2,1,1,0}},
        {1, proc_names[1], {2,3,2,0,0}},
        {2, proc_names[2], {2,3,5,3,0}},
        {3, proc_names[3], {5,3,3,2,0}},
        {4, proc_names[4], {4,5,4,3,5}},
        {5, proc_names[5], {3,5,6,4,1}}
    };

    // Инициализация уровня
    int proc_count = START_PROC_COUNT + game.get_lvl() - 1;
    game.set_manual(resources, processes, proc_count);
    ui.reconstruct(game.get_lvl());
    Text header = makeHeader(res_names, font, proc_count);

    // Для случайных запросов
    Vector2f mouse;
    bool waiting = false;
    tuple<int, int, int> req{ -1,-1,-1 };
    Clock randClock;
    float lastRandTime = 0;
    int interval = 5 + rand() % 5;

    // Для дельта-времени анимации перехода
    Clock deltaClock;

    // Главный цикл
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        // Обработка событий
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
                window.close();
            }
            if (event.type == Event::MouseMoved) {
                mouse = { float(event.mouseMove.x), float(event.mouseMove.y) };
            }

            if (in_menu && !menu.is_animation_active()) {
                // В меню — только клики по кнопкам
                if (event.type == Event::MouseButtonPressed) {
                    int lvl = menu.interactive(mouse, true);
                    if (lvl != 0) {
                        levelSelected = true;
                    }
                }
            }
            else if (!in_menu && !menu.is_animation_active()) {
                // В игре — обработка UI процессов
                if (event.type == Event::MouseButtonPressed) {
                    int state = ui.interactive(mouse, true);
                    if (state && waiting) {
                        auto [pid, rid, amt] = req;
                        info.setString(state == 1
                            ? game.action_result(pid, rid, amt)
                            : "Request denied!");
                        waiting = false;
                    }
                }
                else {
                    ui.interactive(mouse, false);
                }
            }
        }

        // Обновление
        if (levelSelected) {
            // Анимация перехода внутри Menu
            menu.update_animation(dt, game);
            if (!menu.is_animation_active()) {
                // Переход завершён — стартуем игру
                levelSelected = false;
                in_menu = false;

                // Инициализируем новый уровень
                int lvl = game.get_lvl();
                proc_count = START_PROC_COUNT + lvl - 1;
                game.set_manual(resources, processes, proc_count);
                ui.reconstruct(lvl);
                header = makeHeader(res_names, font, proc_count);
            }
        }
        else if (!in_menu) {
            // Только в игровом режиме — генерация случайных запросов
            float t = randClock.getElapsedTime().asSeconds();
            if (t - lastRandTime >= interval) {
                lastRandTime = t;
                interval = 3 + rand() % 7;
                if (!waiting) {
                    req = game.create_random_request();
                    auto [pid, rid, amt] = req;
                    if (pid >= 0) {
                        waiting = true;
                        info.setString(proc_names[pid] + " requests " +
                            to_string(amt) + " of " + res_names[rid]);
                    }
                }
            }
        }

        // Отрисовка
        window.clear(white);

        if (in_menu) {
            menu.draw(window);
            window.draw(menu_info);
        }
        else {
            // Игровой экран
            menu.draw(window);  // фон меню остаётся
            updateStatistic(stat, game);
            ui.draw(window);
            window.draw(stat);
            window.draw(info);
            window.draw(header);

            // Таблица ресурсов
            int row = 0;
            for (const auto& r : game.get_vec_res())
                window.draw(create_res_text(row++, r, font));

            // Таблица процессов
            row = 0;
            for (const auto& p : game.get_vec_pr())
                window.draw(create_proc_text(row++, game.get_vec_res().size(), p, font));
        }

        // Поверх всего — анимация перехода
        menu.draw_animation(window);

        window.display();
    }

    return 0;
}
