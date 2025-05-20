// Design.h
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Constants.h"
#include "GameState.h"

using namespace std;
using namespace sf;

// ────────── Цвета ──────────
const Color white(255, 255, 255);
const Color black(0, 0, 0);
const Color blue(50, 150, 250);
const Color red(227, 38, 54);
const Color green(100, 200, 50);
const Color yellow(255, 250, 205);
const Color orange(255, 69, 0);

const Color lightOrange(255, 140, 0);
const Color lightRed(240, 128, 128);
const Color lightBlue(131, 185, 229);
const Color lightGreen(71, 167, 106);

const Color darkGreen(0, 120, 0);
const Color darkRed(120, 0, 0);


// ────────── Frame ──────────
class Frame : public Drawable {
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


// ────────── Button ──────────
class Button : public Drawable {
private:
    RectangleShape back, border;
    Text text;
    bool isHovered, isActive;
    Vector2f size, pos;
public:
    Button() : isHovered(false), isActive(false) {}
    Button(const Font& font, const String& mes, float s);
    void setColor(Color col, Color col2) { border.setFillColor(col); border.setOutlineColor(col2); }
    void hover(bool x, Color self);
    void hover(bool x, bool t);
    void activate(bool x) { isActive = x; }
    void draw(RenderTarget& target, RenderStates states) const override;
    bool contains(Vector2f point) const { return back.getGlobalBounds().contains(point); }
    void setSize(const Vector2f& size);
    void setPosition(const Vector2f& p);
    bool get_hovered() const { return isHovered; }
    bool get_active() const { return isActive; }
    Color getColor() const { return border.getFillColor(); }
    void setTextPos(float x, float y) { text.setPosition(x, y); }
};


// ────────── Input ──────────
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
    wstring getText() const { return content; }
    bool is_empty() const { return content.empty(); }
    void clear() { content.clear(); text.setString(L""); }
};


// ────────── LevelDesign ──────────
class LevelDesign {
private:
    Frame main, resources, request, processes;
    Button grant, deny;
    RectangleShape background;
public:
    LevelDesign(const Font& font);
    void draw(RenderWindow& w);
    void reconstruct(int l = 0);
    void setElemColor(const string& name, const Color& Col1, const Color& Col2);
    int interactive(const Vector2f& mousePos, bool cl);
};


// ────────── Animation ──────────
class Animation : public Drawable {
private:
    RectangleShape overlay;
    Text title_text;
    Text description;
    RectangleShape progress_bar;
    RectangleShape progress_background;
    enum class State { FadeIn, ShowInfo, FadeOut, Done } state;
    float fade_in_time = 0.5f;
    float show_info = 2.0f;
    float fade_out_time = 0.5f;

    Clock anima_clock;
    float currentTime = 0.f;

    int cur_level = 1;
    bool isActive = false;

    struct LevelColors { Color background, main; };
    vector<LevelColors> levelColorSchemes;
    vector<string> levelDescriptions;

    void set_description();
public:
    Animation(const Font& font);

    // Запустить анимацию перехода
    void start_animation(int newLevel);
    bool update();
    bool is_transition_active() const { return isActive; }
    int get_curr_lvl() const { return cur_level; }
    // Цветовые схемы для текущего уровня
    pair<Color, Color> get_lvl_colors() const;
    void draw(RenderTarget& target, RenderStates states) const override;
};

// ────────── Menu ──────────
class Menu {
private:
    Frame main, info;
    Button stats, l1, l2, l3, l4;
    Input input;
    RectangleShape background;
    Animation anime;
public:
    Menu(const Font& font);
    void draw(RenderWindow& w);
    // При клике: mousePos + флаг клика
    int interactive(const Vector2f& mousePos, bool click);
    // Обновление анимации перехода (вызывает GameState при окончании)
    void update_animation(float dt, GameState& gameState);
    void draw_animation(RenderWindow& w) const;
    bool is_animation_active() const { return anime.is_transition_active(); }
};
