#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <string>
#include <iostream>
using namespace std;
using namespace sf;

const Color white(255, 255, 255);
const Color black(0, 0, 0);
const Color blue(100, 200, 220);
const Color white_red(10, 10, 10);
const Color gray_blue(20, 40, 60);
const Color dark_blue(10, 20, 40, 230);
const Color beauty_red(227, 38, 54);
const Color bordo(176, 0, 0);

class Frame:public Drawable {
protected:
    RectangleShape back, border;
    Text text;
    Vector2f size, pos;
public:
    Frame() = default;
    Frame(const Font& font, const String& title, float s);
    void setColor(sf::Color col) { back.setFillColor(col); }
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
    void setColor(const sf::Color& col) { back.setFillColor(col); }
    void hover(bool x);
    void activate(bool x);
    void draw(RenderTarget& target, RenderStates states) const override;
    bool contains(Vector2f point) const { return back.getGlobalBounds().contains(point); }
    void setSize(const Vector2f& size);
    void setPosition(const Vector2f& p);

    float getBottom() const { return pos.y + size.y; }
    float getRight() const { return pos.x + size.x; }
};

class LevelDesign {
private:
    Frame main, resources, request, processes;
    Button allow, deny;
    RectangleShape background;
public:
    LevelDesign(const Font& font);
    void draw(RenderWindow& w);
    void reconstruct(int level);
    void setFrameColor(const string& name, const sf::Color& color);
    RectangleShape& get_background() { return background; }
};