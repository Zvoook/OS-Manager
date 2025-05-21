#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Constants.h"
#include "GameState.h"
using namespace std;
using namespace sf;

const Color white(255, 255, 255);
const Color black(0, 0, 0);
const Color blue(50, 150, 250);
const Color red(227, 38, 54);
const Color green(100, 200, 50);
const Color yellow(255, 250, 205);
const Color orange(255, 69, 0);
const Color gray(240, 240, 240);

const Color lightOrange(255, 140, 0);
const Color lightRed(240, 128, 128);
const Color lightBlue(131, 185, 229);
const Color lightGreen(71, 167, 106);

const Color darkGreen(0, 120, 0);
const Color darkRed(120, 0, 0);
const Color darkGray(100, 100, 100);


class Frame :public Drawable {
protected:
    RectangleShape back, border;
    Text text;
    Vector2f size, pos;
public:
    Frame() = default;
    Frame(const Font& font, const String& tite, float s);
    void setColor(Color col, Color col2) { border.setFillColor(col); border.setOutlineColor(col2); }
    void draw(RenderTarget& target, RenderStates states) const override;
    void setSize(const Vector2f& size);
    void setPosition(const Vector2f& p);
    void setTextPos(float x, float y) { text.setPosition(x, y); }
};

class Button :public Drawable {
private:
    RectangleShape back, border;
    Text text;
    bool isHovered, isActive, can_activate;
    Vector2f size, pos;
public:
    Button() :isHovered(false), isActive(false), can_activate(true) {};
    Button(const Font& font, const String& mes, float s);
    void setColor(Color col, Color col2) { border.setFillColor(col); border.setOutlineColor(col2); }
    void hover(bool x);
    bool hover(bool x, int a) { isHovered = x; return x; };
    void hover(bool x, bool t);
    void activate(bool x);
    void draw(RenderTarget& target, RenderStates states) const override;
    bool contains(Vector2f point) const { return back.getGlobalBounds().contains(point); }
    void setSize(const Vector2f& size);
    void setPosition(const Vector2f& p);
    void set_can_activate(bool a) { can_activate = a; }
    void setTextPos(float x, float y) { text.setPosition(x, y); }

    bool get_hovered() const { return isHovered; }
    bool get_active() const { return isActive; }
    sf::Color getColor() const { return border.getFillColor(); }
    bool get_can_activate() const { return can_activate; }
};

class Input {
private:
    wstring content;
    Text text;
    Button accept;
    RectangleShape texture;
    bool active;
public:
    Input() = default;
    Input(const Font& font, Vector2f position, Vector2f size);

    void handleEvent(const Event& event);
    void draw(RenderWindow& window) const;
    void hide_help();
    void clear() { content.clear(); text.setString(""); }

    bool is_empty() const { return content == ""; }
    wstring getText() const { return content; }
};

class LevelDesign {
private:
    Frame main, resources, request, processes;
    Button grant, deny, go_menu;
    RectangleShape background;
public:
    LevelDesign() = default;
    LevelDesign(const Font& font);
    void draw(RenderWindow& w);
    void reconstruct(int l = 0);
    void setElemColor(const string& name, const Color& Col1, const Color& Col2);
    int interactive(const Vector2f& mousePos, bool cl);
};

class Menu {
private:
    Frame main, info;
    Button stats, l1, l2, l3, l4;
    Input input;
    RectangleShape background;
public:
    Menu() = default;
    Menu(const Font& font);
    void draw(RenderWindow& w);
    void upd_buttons(int i);
    int interactive(const Vector2f& mousePos, bool click);
    void handle(const Event& event);
    bool text_entered() const { return !input.is_empty(); }
};

class Statistics {
private:
    Frame main, info;
    Button go_menu;
    RectangleShape background;
public:
    Statistics() = default;
    Statistics(const Font& font);
    void draw(RenderWindow& w);
    int interactive(const Vector2f& mousePos, bool click);
};


//class Interface {
//public:
//    Interface(const Font& font);
//    Menu menu;
//    Statistics statistics;
//    LevelDesign level;
//    Text stats, info, menu_info;
//
//    void level_reconstrust(int l) { level.reconstruct(l); }
//    void draw(wins window, RenderWindow& w);
//    int interactive(wins window, const Vector2f& mousePos, bool click);
//    void handle(const Event& event) { menu.handle(event); }
//    void upd_buttons(int i) { menu.upd_buttons(i); }
//    void updateStatistic(const GameState& g);
//
//    void set_stats(string str) { stats.setString(str); }
//    void set_info(string str) { info.setString(str);}
//
//    Text get_stats() const { return stats; }
//    Text get_info() const { return info; }
//    Text get_menu_info() const { return menu_info; }
//    bool text_entered() const { return menu.text_entered(); }
//};
