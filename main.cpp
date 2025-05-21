#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include "Constants.h"
#include "GameState.h"
#include "Design.h"
using namespace sf;


string fix_str(const string& text, int width)
{
    if (text.length() >= size_t(width))
        return text.substr(0, width - 1) + "~";
    return text + string(width - text.length(), ' ');
}
void updateStatistic(Text& t, const GameState& g)
{
    t.setString("Level: " + to_string(g.get_lvl()) + "   Wins: " + to_string(g.get_wins()) + "   Losses: " + to_string(g.get_losses()));
}
Text create_res_text(int row, const Resource& r, Font& f)
{
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
Text create_proc_text(int row, int resCnt, const Process& p, Font& f)
{

    Text t("", f, 24);
    t.setFillColor(black);
    string line = fix_str(p.get_name(), PROC_NAME_WIDTH);
    const auto& alloc = p.get_alloc();
    const auto& max = p.get_max_require();

    for (int i = 0; i < resCnt; ++i) {
        int need = max[i] - alloc[i];
        if (!need) line += fix_str("-", CELL_WIDTH);
        else line += fix_str(to_string(need), CELL_WIDTH);
    }
    t.setString(line);
    t.setPosition(FRAME_X_PROC + 20,
        FRAME_Y_PROC + PROC_OFFSET + row * PROC_STEP);
    return t;
}
Text makeHeader(const vector<string>& names, Font& f, int c)
{
    Text h("", f, 22);
    h.setFillColor(black);
    string line = fix_str("", PROC_NAME_WIDTH);
    int i = 0;
    for (const auto& n : names) {
        if (i < c) {
            line += fix_str(n, CELL_WIDTH);
            ++i;
        }
    }
    h.setString(line);
    h.setPosition(FRAME_X_PROC + 48.5, FRAME_Y_PROC + PROC_OFFSET - 25);
    return h;
}
void wait(int sec) {
    Clock time;
    while (time.getElapsedTime().asSeconds() < sec) {};
}

int main()
{
    srand(time(0));
    Font font;
    Image icon;
    if (!font.loadFromFile("Font.ttf") || !icon.loadFromFile("man.jpg")) return 1;
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Resource Manager", Style::Close);

    wins stat = menu_win;
    bool need_to_wait = false;

    vector<float> coeff = { 0.95f, 0.7f, 0.5f, 0.35f };
    int proc_cnt[] = { 3, 4, 5, 6 };
    int res_cnt[] = { 3, 4, 5, 6 };

    vector<string> res_names = { "CPU", "RAM", "HDD", "NET", "GPU", "VRAM" };
    vector<string> proc_names = { "Notepad", "Explorer", "Compiler", "Discord", "Minecraft", "Chrome" };
    vector<Process> all_proc = {
        {0, "Notepad",   {0, 2, 1, 0, 0, 0}}, {1, "Explorer",  {2, 3, 2, 0, 0, 0}},
        {2, "Compiler",  {2, 3, 5, 0, 0, 0}}, {3, "Discord",   {5, 3, 3, 2, 0, 0}},
        {4, "Minecraft", {4, 5, 4, 3, 5, 1}}, {5, "Chrome",    {3, 5, 6, 4, 1, 0}}
    };

    GameState game(all_proc, res_names, proc_cnt, res_cnt, coeff);
    LevelDesign ui(font);
    Menu menu(font);
    Statistics statistics(font);

    int opened_levels = 4;

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    ui.reconstruct(game.get_lvl());

    int proc_count = START_PROC_COUNT + game.get_lvl() - 1;

    Text header = makeHeader(res_names, font, proc_count);
    Text stats("", font, 20);  stats.setFillColor(black); stats.setPosition(57, 70);
    Text info("", font, 21);  info.setFillColor(black); info.setPosition(430, 160);
    Text menu_info("It's the OS operator game. This game simulate actual OS working.\nYou should grant or deny requests from processes and avoid deadlock.", font, 19); menu_info.setFillColor(black); menu_info.setPosition(Vector2f(INFO_X + 30, INFO_Y + 70));

    // Информация о сохранении/загрузке
    Text save_info("Press S to save game, L to load game", font, 16);
    save_info.setFillColor(black);
    save_info.setPosition(SCREEN_WIDTH - 300, SCREEN_HEIGHT - 30);

    Vector2f mouse;
    bool waiting = false;
    tuple<int, int, int> req{ -1,-1,-1 };
    Clock clock;
    float last = 0;  int interval = rand() % 4;


    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
                window.close();

            if (event.type == Event::MouseMoved)
                mouse = { float(event.mouseMove.x), float(event.mouseMove.y) };

            // Обработка клавиш S и L для сохранения и загрузки
            if (event.type == Event::KeyPressed) {
                if (stat == level_win) {  // Сохранение/загрузка работает только на экране уровня
                    if (event.key.code == Keyboard::S) {
                        // Сохранение игры
                        game.save_to_file("save.txt");
                        info.setString("Game saved to save.txt");
                    }
                    else if (event.key.code == Keyboard::L) {
                        // Загрузка игры
                        if (game.load_from_file("save.txt")) {
                            info.setString("Game loaded from save.txt");
                            ui.reconstruct(game.get_lvl());
                            int cur_res_count = game.get_vec_res().size();
                            header = makeHeader(res_names, font, cur_res_count);
                        }
                        else {
                            info.setString("Failed to load game!");
                        }
                    }
                }
            }

            if (stat == menu_win) {
                menu.handle(event);
                if (menu.text_entered()) menu.upd_buttons(opened_levels);
                if (event.type == Event::MouseButtonPressed)
                {
                    int state = menu.interactive(mouse, true);
                    if (state && state != 5) {
                        stat = level_win;
                        game.set_level(state);
                        game.init_level(all_proc, res_names, proc_cnt, res_cnt, coeff, state);
                        ui.reconstruct(game.get_lvl());
                        int cur_proc_count = START_PROC_COUNT + game.get_lvl() - 1;
                        header = makeHeader(res_names, font, cur_proc_count);
                    }
                    else if (state == 5) stat = statistic_win;
                }
                window.clear(white);
                menu.draw(window);
                window.draw(menu_info);
                window.display();
            }
            else if (stat == level_win) {
                if (clock.getElapsedTime().asSeconds() - last >= interval)
                {
                    last = clock.getElapsedTime().asSeconds();
                    interval = rand() % 4;
                    if (!waiting)
                    {
                        req = game.create_random_request();
                        auto [pid, rid, amt] = req;
                        if (pid >= 0)
                        {
                            waiting = true;
                            info.setString(proc_names[pid] + " requests " +
                                to_string(amt) + " of " + res_names[rid]);
                        }
                    }
                }
                if (event.type == Event::MouseButtonPressed)
                {
                    int state = ui.interactive(mouse, true);
                    if (state == 2) stat = menu_win;
                    else if (state && waiting)
                    {
                        auto [pid, rid, amt] = req;
                        info.setString(state == 1 ? game.action_result(pid, rid, amt) : "Request denied");
                        if (info.getString() == "Resource successful granted\n\nPROCESS COMPLETED") {
                            if (game.is_lvl_passed()) info.setString("LEVEL COMPLETED");
                            /*wait(3);*/
                        }
                        else if (info.getString() == "You face to deadlock\n\nGAME OVER") {
                            /*wait(3);*/
                            game.init_level(all_proc, res_names, proc_cnt, res_cnt, coeff, game.get_lvl());
                        }
                        // Сбрасываем флаг waiting после обработки запроса
                        waiting = false;
                    }
                }
                else ui.interactive(mouse, false);


                window.clear(white);
                ui.draw(window);
                updateStatistic(stats, game);
                window.draw(stats);
                window.draw(info);
                window.draw(header);
                window.draw(save_info);  // Отображаем информацию о сохранении/загрузке
                int row = 0;
                for (const auto& r : game.get_vec_res())
                    window.draw(create_res_text(row++, r, font));
                row = 0;
                for (const auto& p : game.get_vec_pr())
                    window.draw(create_proc_text(row++, game.get_vec_res().size(), p, font));
                window.display();
            }
            else if (stat == statistic_win) {
                if (event.type == Event::MouseButtonPressed)
                {
                    int state = statistics.interactive(mouse, true);
                    if (state) stat = menu_win;
                }
                window.clear(white);
                statistics.draw(window);
                window.display();
            }
        }
    }
    return 0;
}