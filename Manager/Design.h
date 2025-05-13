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
const Color red(200, 50, 50);
const Color green(100, 200, 50);

const Color bordo(176, 0, 0);
const Color biruza(50, 200, 150);
const Color orange(250, 150, 50);

const Color dark_blue(20, 40, 60);
const Color pale_blue(171, 205, 239);
const Color pale_green(71, 167, 106);
const Color white_red(10, 10, 10);
const Color beauty_red(227, 38, 54);

class Frame:public Drawable {
protected:
    RectangleShape back, border;
    Text text;
    Vector2f size, pos;
public:
    Frame() = default;
    Frame(const Font& font, const String& tite, float s);
    void setColor(sf::Color color) { back.setFillColor(color); }
    void draw(RenderTarget& target, RenderStates states) const override;
    void setSize(const Vector2f& size);
    void setPosition(const Vector2f& p);

    float getBottom() const {return pos.y + size.y;}
    float getRight() const {return pos.x + size.x;}
};

class Button:public Drawable {
private:
    RectangleShape back, border;
    Text text;
    bool isHovered, isActive;
    Vector2f size, pos;
public:
    Button():isHovered(false), isActive(false) {};
    Button(const Font& font, const String& mes, float s);
    void setColor(const sf::Color& co) { back.setFillColor(co); }
    void hover(bool x);
    void activate(bool x);
    void draw(RenderTarget& target, RenderStates states) const override;
    bool contains(Vector2f point) const { return back.getGlobalBounds().contains(point); }
    void setSize(const Vector2f& size);
    void setPosition(const Vector2f& p);

    float getBottom() const { return pos.y + size.y; }
    float getRight() const { return pos.x + size.x; }
    bool get_hovered() const { return isHovered; }
    bool get_active() const { return isActive; }
};

class LevelDesign {
private:
    Frame main, resources, request, processes;
    Button allow, deny;
    RectangleShape background;
public:
    LevelDesign(const Font& font);
    void draw(RenderWindow& w);
    void reconstruct(int eve);
    void setFrameColor(const string& name, const sf::Color& Color);
    int interactive(const Vector2f& mousePos);
    RectangleShape& get_background() { return background; }
};