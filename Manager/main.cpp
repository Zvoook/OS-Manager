#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Design.h"
#include <iostream>
using namespace sf;

int main() {
    GameState game;
    RenderWindow window(VideoMode(width, height), "Resource Manager", Style::Close);
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Font load error!\n";
        return 1;
    }
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear(Color::Blue);
        Text title("Resource Status", font, 24);
        title.setPosition(20, 10);
        title.setFillColor(Color::White);
        window.draw(title);
        int res_offset = 50;
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
        }
        Text procTitle("Processes", font, 24);
        procTitle.setPosition(20, 120);
        procTitle.setFillColor(Color::White);
        window.draw(procTitle);

        int proc_offset = 160;
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
        }
        window.display();
    }

    return 0;
}