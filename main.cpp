#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include "Constants.h"
#include "GameState.h"
#include "Design.h"
using namespace sf;
string fix_str(const string& text, int width)
{
    if (text.length() >= static_cast<size_t>(width))
        return text.substr(0, width - 1) + "~";          
    return text + string(width - text.length(), ' ');   
}
void updateStatistic(Text& t, const GameState& g)
{
    t.setString("Level: "  + to_string(g.get_lvl()) +"   Wins: "   + to_string(g.get_wins()) + "   Losses: " + to_string(g.get_losses()));
}
Text create_res_text(int row, const Resource& r, Font& f)
{
    Text t("", f, 24);
    t.setFillColor(black);
    string line = fix_str(r.get_name(), RES_NAME_WIDTH) + SEP +
                  fix_str(to_string(r.get_available()) + "/" +
                                   to_string(r.get_total()),
                                   CELL_WIDTH);
    t.setString(line);
    t.setPosition(FRAME_X_RES + 20,
                  FRAME_Y_RES + RES_OFFSET + row * RES_STEP);
    return t;
}
Text create_proc_text(int row, int resCnt, const Process& p,
                  Font& f)
{
    Text t("", f, 24);
    t.setFillColor(black);
    string line = fix_str(p.get_name(), PROC_NAME_WIDTH);
    const auto& alloc = p.get_alloc();
    const auto& max   = p.get_max_require();

    for (int i = 0; i < resCnt; ++i)
        line += fix_str(to_string(alloc[i]) + "/" +
                                 to_string(max[i]), CELL_WIDTH);
    t.setString(line);
    t.setPosition(FRAME_X_PROC + 20,
                  FRAME_Y_PROC + PROC_OFFSET + row * PROC_STEP);
    return t;
}
Text makeHeader(const vector<string>& names, Font& f)
{
    Text h("", f, 22);
    h.setFillColor(black);
    string line = fix_str("", PROC_NAME_WIDTH);
    for (const auto& n : names)
        line += fix_str(n, CELL_WIDTH);
    h.setString(line);
    h.setPosition(FRAME_X_PROC + 48.5, FRAME_Y_PROC + PROC_OFFSET - 25);
    return h;
}

int main()
{
    srand(time(0));
    Font font;
    if (!font.loadFromFile("JetBrainsMono-Bold.ttf")) return 1;
    Image icon;
    if (!icon.loadFromFile("man.jpg")) return -1;
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Resource Manager", Style::Close);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    GameState   game(2);
    LevelDesign ui(font);
    ui.reconstruct(game.get_lvl());
    vector<string> res_names = { "CPU", "RAM", "DISK", "NET", "GPU" };
    vector<Resource> resources = {
        {0, "CPU",  8},
        {1, "RAM", 16},
        {2, "DISK", 6},
        {3, "NET",  4},
        {4, "GPU",  8}
    };
    vector<string> proc_names = { 
        "Notepad", 
        "Explorer", 
        "Compiler",                
        "Discord", 
        "Minecraft", 
        "Chrome" 
    };
    vector<Process> processes = {
        {0, proc_names[0], {0, 2, 1, 1, 0}},
        {1, proc_names[1], {2, 3, 2, 0, 0}},
        {2, proc_names[2], {2, 3, 5, 3, 0}},
        {3, proc_names[3], {5, 3, 3, 2, 0}},
        {4, proc_names[4], {4, 5, 4, 3, 5}},
        {5, proc_names[5], {3, 5, 6, 4, 1}}
    };

    int shownProcCnt = START_PROC_COUNT + game.get_lvl() - 1;
    game.set_manual(resources, processes, shownProcCnt);
    Text stat("", font, 20);  stat.setFillColor(black); stat.setPosition(57, 70);
    Text info("", font, 21);  info.setFillColor(black); info.setPosition(430, 160);
    Text header = makeHeader(res_names, font);

    Vector2f mouse;
    bool waiting = false;
    tuple<int,int,int> req{-1,-1,-1};
    Clock clock;
    float  last = 0;  int interval = 5 + rand()%5;

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

            if (event.type == Event::MouseButtonPressed)
            {
                int state = ui.interactive(mouse, true);
                if (state && waiting)
                {
                    auto [pid, rid, amt] = req;
                    info.setString(state == 1
                        ? game.action_result(pid, rid, amt)
                        : "Request denied!");
                    waiting = false;
                }
            }
            else ui.interactive(mouse, false);
        }
        if (clock.getElapsedTime().asSeconds() - last >= interval)
        {
            last = clock.getElapsedTime().asSeconds();
            interval = 3 + rand()%7;

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
        window.clear(white);
        updateStatistic(stat, game);
        ui.draw(window);
        window.draw(stat);
        window.draw(info);
        window.draw(header);
        int row = 0;
        for (const auto& r : game.get_vec_res())
            window.draw(create_res_text(row++, r, font));
        row = 0;
        for (const auto& p : game.get_vec_pr())
            window.draw(create_proc_text(row++, shownProcCnt, p, font));
        window.display();
    }
    return 0;
}
