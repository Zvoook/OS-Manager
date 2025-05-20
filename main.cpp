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
    text.setCharacterSize(20);
    text.setFillColor(black);
    text.setString(res.get_name() + ": " + to_string(res.get_available()) + "/" + to_string(res.get_total()));
    text.setPosition(20, RES_OFFSET + i * RES_STEP);
    ++i;
    return text;
}

Text create_proc_text(int& i, const Process& proc, Font& font) {
    Text text;
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(Color::Green);
    string line = proc.get_name() + "  ";
    for (int require : proc.get_max_require()) line += "(" + to_string(require) + ") ";
    text.setString(line);
    text.setPosition(20, PROC_OFFSET + i * PROC_STEP);
    ++i;
    return text;
}


int main() {
    Font font;
    if (!font.loadFromFile("arial.ttf")) return 1;
    GameState game;
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Resource Manager", Style::Close);
    LevelDesign ui(font);
    bool waitingAnswer = false;
    //tuple<int, int, int> req;
    string message;

    Text statistic;
    statistic.setFont(font);
    statistic.setCharacterSize(22);
    statistic.setFillColor(black);
    statistic.setPosition(70, 70);
    updatestatistic(statistic, game);
    
    /*map<wstring, Color> resourceColors;
    resourceColors["CPU"] = red;
    resourceColors["Энергия"] = blue;
    resourceColors["Вода"] = biruza;
    resourceColors["Топливо"] = orange;
    resourceColors["Пища"] = green;*/

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
                window.close();
            }
            //if (/*Resource generating request*/) {
            //    req = game.get_request();
            //    int proc_id = req[0];
            //    int res_id = req[1];
            //    int amount = req[2];
            //    if (req[0] >= 0) {
            //        waitingAnswer = true;

            //        wstring proc_name = String(game.get_vec_pr()[proc_id].get_name()).toWideString();
            //        wstring res_name = String(game.get_vec_res()[res_id].get_name()).toWideString();

            //        message = String(proc_name + " request " +
            //            to_wstring(amount) + res_name + " resources ");
            //    }
            //}
            else if (event.type == Event::MouseMoved) {
                Vector2f mousePos((float)(event.mouseMove.x), (float)(event.mouseMove.y));
                bool state = ui.interactive(mousePos);
                if (state == -1) {
                    message = "Request denyed";
                    waitingAnswer = false;
                }
                else if (state == 1) {
                    message = "Request accepted";
                    waitingAnswer = false;
                }
            }
        }

        window.clear(Color::Blue);
        updatestatistic(statistic, game);
        window.draw(statistic);
        ui.draw(window);

        int i = 0, j = 0;
        for (const auto& res: game.get_vec_res()) {
            Text text = create_res_text(i, res, font);
            window.draw(text);
        }
        for (const auto& proc : game.get_vec_pr()) {
            Text text = create_proc_text(j, proc, font);
            window.draw(text);
        }
        window.display();
    }
    return 0;
}