#include "fix_for_mac.h"
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Design.h"
#include <iostream>
using namespace sf;

void updatestatistic(Text& statistic, const GameState& game) {
    statistic.setString("Level: " + to_string(game.get_lvl()) +
        "   Wins: " + to_string(game.get_wins()) + "   Losses: " + to_string(game.get_losses()));
}

Text create_res_text(int& i, const Resource& res, Font& font) {
    Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(black);
    text.setString(res.get_name() + "   " + to_string(res.get_available()) + "/" + to_string(res.get_total()));
    text.setPosition(FRAME_X_RES + 20, FRAME_Y_RES + RES_OFFSET + i * RES_STEP);
    ++i;
    return text;
}

Text create_proc_text(int& i, int c, const Process& proc, Font& font) {
    Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(black);
    int k = 0;
    string line = proc.get_name() + "       ";
    for (int require : proc.get_max_require()) {
        if (k < c) {
            line += to_string(require) + "      ";
            ++k;
        }
    }
    text.setString(line);
    text.setPosition(FRAME_X_PROC + 20, FRAME_Y_PROC - 100 + 20 + PROC_OFFSET + i * PROC_STEP);
    ++i;
    return text;
}

int main() {
    srand(time(0));
    Font font;
    if (!font.loadFromFile("Nihonium1.ttf")) return 1;
    Image icon;
    if (!icon.loadFromFile("man.jpg")) return -1;
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Resource Manager", Style::Close);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    GameState game(2);
    LevelDesign ui(font);
    ui.reconstruct(game.get_lvl());


    int cur_res_count = START_PROC_COUNT + game.get_lvl() - 1;
    bool waitingAnswer = false;
    tuple<int, int, int> req = { -1, -1, -1 };
    string message;


    vector<string> res_names = {
    string("CPU"),
    string("RAM"),
    string("DISK"),
    string("NET"),
    string("GPU")
    };

    vector<Resource> resources = {
    Resource(0, res_names[0] + "     ", 8),
    Resource(1, res_names[1] + "     ", 16),
    Resource(2, res_names[2] + "    ", 6),
    Resource(3, res_names[3] + "     ", 4),
    Resource(4, res_names[4] + "     ", 8)
    };

    vector<string> names = {
    string("Notepad"),
    string("Explorer"),
    string("Compiler"),
    string("Discord"),
    string("Minecraft"),
    string("Chrome"),
    string()
    };

    vector<Process> processes = {
        Process(0, names[0] + "    ", {0, 2, 0, 0, 0}),
        Process(1, names[1] + "   ", {2, 3, 2, 0, 0}),
        Process(2, names[2] + "   ", {2, 3, 5, 3, 0}),
        Process(3, names[3] + "    ", {5, 3, 3, 2, 0}),
        Process(4, names[4] + "  ", {4, 5, 4, 3, 5}),
        Process(5, names[5] + "     ", {3, 5, 6, 4, 1}),
        Process(6, names[6] + "     ", {3, 5, 6, 4, 1})
    };

    game.set_manual(resources, processes, cur_res_count);

    Text statistic;
    statistic.setFont(font);
    statistic.setCharacterSize(20);
    statistic.setFillColor(black);
    statistic.setPosition(57, 70);
    updatestatistic(statistic, game);

    Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(21);
    infoText.setFillColor(black);
    infoText.setPosition(430, 160);

    Text column;
    string col;
    for (int i = 0; i < game.get_vec_res().size(); ++i) col = col + res_names[i] + "    ";
    column.setString(col);
    column.setFont(font);
    column.setCharacterSize(22);
    column.setFillColor(black);
    column.setPosition(360, 465);

    Vector2f mousePos(0, 0);

    Clock timer;
    float last = 0;
    int interval = rand() % 5 + 5;


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
                window.close();
            }
            if (event.type == Event::MouseMoved) mousePos = { (float)event.mouseMove.x, (float)event.mouseMove.y };
            if (event.type == Event::MouseButtonPressed) {
                int state = ui.interactive(mousePos, 1);
                if (state != 0 && waitingAnswer) {
                    int proc_id = get<0>(req);
                    int res_id = get<1>(req);
                    int amount = get<2>(req);
                    if (state == 1) message = game.action_result(proc_id, res_id, amount);
                    else message = "Request denied!";
                    waitingAnswer = false;
                }
            }
            else ui.interactive(mousePos, 0);
        }
        if (timer.getElapsedTime().asSeconds() - last >= interval) {
            last = timer.getElapsedTime().asSeconds();
            interval = rand() % 7 + 3;
            if (!waitingAnswer) {
                req = game.create_random_request();
                int proc_id = get<0>(req);
                int res_id = get<1>(req);
                int amount = get<2>(req);
                if (proc_id >= 0) {
                    waitingAnswer = true;
                    message = names[proc_id] + " requests " +
                        to_string(amount) + " of " + game.get_vec_res()[res_id].get_name();
                }
            }
        }
        infoText.setString(message);

        window.clear(white);
        updatestatistic(statistic, game);
        ui.draw(window);
        window.draw(statistic);
        window.draw(infoText);
        window.draw(column);

        int i = 0, j = 0;
        for (const auto& res : game.get_vec_res()) {
            Text text = create_res_text(i, res, font);
            window.draw(text);
        }
        for (const auto& proc : game.get_vec_pr()) {
            Text text = create_proc_text(j, cur_res_count, proc, font);
            window.draw(text);
        }
        window.display();
    }
    return 0;
}
