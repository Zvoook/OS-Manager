#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include "Constants.h"
#include "GameState.h"
#include "Design.h"
using namespace sf;
using namespace std;

string fix_str(const string& text, int width)
{
    if (text.length() >= size_t(width))
        return text.substr(0, width - 1) + "~";
    return text + string(width - text.length(), ' ');
}

void updateStatistic(Text& t, const GameState& g)
{
    string stat_text = "Level: " + to_string(g.get_lvl());
    if (g.has_player()) {
        stat_text += "   " "Wins: " + to_string(g.get_wins()) +
            "    Losses: " + to_string(g.get_losses());
    }
    else {
        stat_text += "   Wins: " + to_string(g.get_session_wins()) +
            "   Losses: " + to_string(g.get_session_losses());
    }
    t.setString(stat_text);
}

Text create_res_text(int row, const Resource& r, Font& f)
{
    Text t("", f, 24);
    t.setFillColor(black);
    string line = " " + fix_str(r.get_name(), RES_NAME_WIDTH) + SEP + " " +
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
    if (!font.loadFromFile("Font.ttf") || !icon.loadFromFile("image.jpg")) return 1;
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "OS Manager", Style::Close);

    wins stat = menu_win;
    bool need_to_wait = false;
    bool buttons_updated = false;

    vector<float> coeff = { 0.95f, 0.6f, 0.4f, 0.2f };
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

    game.load_rating("rating.txt");

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    ui.reconstruct(game.get_lvl());

    int proc_count = START_PROC_COUNT + game.get_lvl() - 1;

    Text header = makeHeader(res_names, font, proc_count);
    Text stats("", font, 20);  stats.setFillColor(black); stats.setPosition(57, 70);
    Text info("", font, 21);  info.setFillColor(black); info.setPosition(430, 160);
    Text menu_info("It's the OS operator game. This game simulate actual OS working.\nYou should grant or deny requests from processes and avoid deadlock.", font, 19); menu_info.setFillColor(black); menu_info.setPosition(Vector2f(INFO_X + 30, INFO_Y + 70));

    Text save_info("Press S to save game, L to load game", font, 16);
    save_info.setFillColor(black);
    save_info.setOrigin(save_info.getLocalBounds().width / 2, 0);
    save_info.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30);

    menu.upd_buttons(0);

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

            if (event.type == Event::MouseButtonReleased) {
                if (stat == level_win) {
                    ui.resetButtonsActive();
                }
                else if (stat == menu_win) {
                    menu.resetButtonsActive();
                }
                else if (stat == statistic_win) {
                    statistics.resetButtonsActive();
                }
            }

            if (event.type == Event::KeyPressed) {
                if (stat == level_win) {
                    if (event.key.code == Keyboard::S) {
                        game.save_to_file("save.txt");
                        info.setString("Game saved to save.txt");
                    }
                    else if (event.key.code == Keyboard::L) {
                        // Сохраняем текущее состояние на случай неудачной загрузки
                        int old_level = game.get_lvl();

                        if (game.load_from_file("save.txt")) {
                            info.setString("Game loaded from save.txt");
                            ui.reconstruct(game.get_lvl());
                            int cur_res_count = game.get_vec_res().size();
                            header = makeHeader(res_names, font, cur_res_count);
                        }
                        else {
                            info.setString("Failed to load game! File corrupted or incompatible.");
                            // Восстанавливаем состояние игры
                            game.init_level(all_proc, res_names, proc_cnt, res_cnt, coeff, old_level);
                            ui.reconstruct(game.get_lvl());
                            header = makeHeader(res_names, font, game.get_vec_res().size());
                        }
                    }
                }

                if (stat == menu_win && event.key.code == Keyboard::Enter) {
                    string name = menu.get_name();
                    if (!name.empty()) {
                        game.set_player(name);
                        game.save_rating("rating.txt");
                        menu.upd_buttons(game.get_max_available_level());
                        buttons_updated = true;
                    }
                }
            }

            if (stat == menu_win) {
                menu.handle(event);
                if (event.type == Event::MouseButtonPressed) {
                    int state = menu.interactive(mouse, true);
                    if (state > 0 && state < 5 && game.has_player()) {
                        if (state <= game.get_max_available_level()) {
                            stat = level_win;
                            game.set_level(state);
                            game.init_level(all_proc, res_names, proc_cnt, res_cnt, coeff, state);
                            ui.reconstruct(game.get_lvl());
                            header = makeHeader(res_names, font, game.get_vec_res().size());
                        }
                    }
                    else if (state == 5) stat = statistic_win;
                }
                else menu.interactive(mouse, false);
            }
            else if (stat == level_win) {
                if (clock.getElapsedTime().asSeconds() - last >= interval) {
                    last = clock.getElapsedTime().asSeconds();
                    interval = rand() % 4;
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
                if (event.type == Event::MouseButtonPressed) {
                    int state = ui.interactive(mouse, true);
                    if (state == 2) {
                        stat = menu_win;
                        buttons_updated = false;
                    }
                    else if (state && waiting) {
                        auto [pid, rid, amt] = req;
                        info.setString(state == 1 ? game.action_result(pid, rid, amt) : "Request denied");
                        if (info.getString() == "Resource successful granted\n\nPROCESS COMPLETED") {
                            if (game.is_lvl_passed()) {
                                info.setString("LEVEL COMPLETED");
                                game.add_win();
                                game.update_max_level(game.get_lvl());
                                game.save_rating("rating.txt");
                            }
                        }
                        else if (info.getString() == "You face to deadlock\n\nGAME OVER") {
                            game.add_loss();
                            game.save_rating("rating.txt");
                            game.init_level(all_proc, res_names, proc_cnt, res_cnt, coeff, game.get_lvl());
                        }
                        waiting = false;
                    }
                }
                else {
                    ui.interactive(mouse, false);
                }
            }
            else if (stat == statistic_win) {
                if (event.type == Event::MouseButtonPressed) {
                    int state = statistics.interactive(mouse, true);
                    if (state) {
                        stat = menu_win;
                        buttons_updated = false;
                    }
                }
                else {
                    statistics.interactive(mouse, false);
                }
            }
        }
        window.clear(white);

        if (stat == menu_win) {
            menu.draw(window);
            window.draw(menu_info);
            if (!buttons_updated) {
                if (game.has_player()) {
                    menu.upd_buttons(game.get_max_available_level());
                }
                else menu.upd_buttons(0);
                buttons_updated = true;
            }
        }
        else if (stat == level_win) {
            ui.draw(window);
            updateStatistic(stats, game);
            window.draw(stats);
            window.draw(info);
            window.draw(header);
            window.draw(save_info);

            int row = 0;
            for (const auto& r : game.get_vec_res())
                window.draw(create_res_text(row++, r, font));

            row = 0;
            for (const auto& p : game.get_vec_pr())
                window.draw(create_proc_text(row++, game.get_vec_res().size(), p, font));
        }
        else if (stat == statistic_win) {
            statistics.draw(window);

            auto all_stats = game.get_all_stats();

            Text stat_header("RANK    PLAYER        WINS      LOSSES    MAX LEVEL", font, 22);
            stat_header.setPosition(100, 200);
            stat_header.setFillColor(black);
            stat_header.setStyle(Text::Bold);
            window.draw(stat_header);

            RectangleShape line(Vector2f(820, 2));
            line.setPosition(100, 225);
            line.setFillColor(black);
            window.draw(line);
            float y_pos = 240.0f;
            int rank = 1;

            for (const auto& stat_item : all_stats) {
                string player_name = std::get<0>(stat_item);
                int wins = std::get<1>(stat_item);
                int losses = std::get<2>(stat_item);
                int level = std::get<3>(stat_item);
                Color row_color = black;
                if (rank == 1) row_color = Color(255, 215, 0);
                else if (rank == 2) row_color = Color(192, 192, 192);
                else if (rank == 3) row_color = Color(205, 127, 50);
                string rank_str = (rank < 10 ? "  " : "") + to_string(rank) + ".";
                string stat_line = fix_str(rank_str, 6) + "  " +
                    fix_str(player_name, 15) + " " +
                    fix_str(to_string(wins), 12) + " " +
                    fix_str(to_string(losses), 12) + " " +
                    to_string(level);
                Text player_stat(stat_line, font, 20);
                player_stat.setPosition(100, y_pos);
                player_stat.setFillColor(row_color);
                player_stat.setStyle(Text::Bold);
                if (rank <= 3) {
                    RectangleShape row_bg(Vector2f(820, 25));
                    row_bg.setPosition(90, y_pos - 2);
                    row_bg.setFillColor(Color(row_color.r, row_color.g, row_color.b, 30));
                    window.draw(row_bg);
                }
                window.draw(player_stat);
                y_pos += 30.0f;
                rank++;
                if (y_pos > SCREEN_HEIGHT - 120) break;
            }
            if (all_stats.empty()) {
                Text no_stats("No players yet! Start playing to see statistics.", font, 24);
                no_stats.setPosition(SCREEN_WIDTH / 6, SCREEN_HEIGHT / 2);
                no_stats.setFillColor(black);
                window.draw(no_stats);
            }
        }
        window.display();
    }
    return 0;
}