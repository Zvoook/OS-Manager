#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Design.h"
#include <iostream>
using namespace sf;

int main() {
    Font font;
    if (!font.loadFromFile("arial.ttf")) return 1;
    GameState game;
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Resource Manager", Style::Close);
    LevelDesign ui(font);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || (event.type=Event::KeyPressed) && (event.type == Keyboard::Escape)) window.close();
        }

        window.clear(Color::Blue);
        ui.draw(window);
        //window.draw(ui.get_background());
        Text info;
        info.setFont(font);
        info.setString(L"Уровень: " + to_wstring(game.get_lvl()) +
            L"   Успехов: " + to_wstring(game.get_wins()) +
            L"   Провалов: " + to_wstring(game.get_losses()));
        info.setCharacterSize(18);
        info.setFillColor(Color(200, 200, 200));
        info.setPosition(70, 70);
        window.draw(info);
        /*Text title("Resource Status", font, 24);
        title.setPosition(20, 10);
        title.setFillColor(Color::White);
        window.draw(title);*/
        /*int res_offset = 50;
        int res_step = 30;
        for (size_t i = 0; i < game.get_vec_res().size(); ++i) {
            const auto& res = game.get_vec_res()[i];
            Text text;
            text.setFont(font);
            text.setCharacterSize(20);
            text.setFillColor(Color::Cyan);
            text.setString(res.get_name() + ": " + to_string(res.get_available()) + "/" + to_string(res.get_total()));
            text.setPosition(20, res_offset + i * res_step);
            window.draw(text);
        }*/
        /*Text procTitle("Processes", font, 24);
        procTitle.setPosition(20, 120);
        procTitle.setFillColor(Color::White);
        window.draw(procTitle);*/

        /*int proc_offset = 160;
        int proc_step = 25;
        for (size_t i = 0; i < game.get_vec_pr().size(); ++i) {
            const auto& proc = game.get_vec_pr()[i];
            Text text;
            text.setFont(font);
            text.setCharacterSize(18);
            text.setFillColor(Color::Green);
            string line = proc.get_name() + "  ";
            for (int require : proc.get_max_require()) line += "(" + to_string(require) + ") ";
            text.setString(line);
            text.setPosition(20, proc_offset + i * proc_step);
            window.draw(text);
        }*/
        window.display();
    }

    return 0;
}