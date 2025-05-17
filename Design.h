#pragma once
#include "fix_for_mac.h"
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

const Color lightOrange(255, 140, 0);
const Color lightRed(240, 128, 128);
const Color lightBlue(131, 185, 229);
const Color lightGreen(71, 167, 106);

const Color darkGreen(0, 120, 0);
const Color darkRed(120, 0, 0);


class Frame :public Drawable {
protected:
    RectangleShape back, border;
    Text text;
    Vector2f size, pos;
public:
    Frame() = default;
    Frame(const Font& font, const String& tite, float s);
    void setColor(sf::Color col, sf::Color col2) { border.setFillColor(col); border.setOutlineColor(col2); }
    void draw(RenderTarget& target, RenderStates states) const override;
    void setSize(const Vector2f& size);
    void setPosition(const Vector2f& p);
    void setTextPos(float x, float y) { text.setPosition(x, y); }
};

class Button :public Drawable {
private:
    RectangleShape back, border;
    Text text;
    bool isHovered, isActive;
    Vector2f size, pos;
public:
    Button() :isHovered(false), isActive(false) {};
    Button(const Font& font, const String& mes, float s);
    void setColor(sf::Color col, sf::Color col2) { border.setFillColor(col); border.setOutlineColor(col2); }
    void hover(bool x, bool t);
    void activate(bool x);
    void draw(RenderTarget& target, RenderStates states) const override;
    bool contains(Vector2f point) const { return back.getGlobalBounds().contains(point); }
    void setSize(const Vector2f& size);
    void setPosition(const Vector2f& p);

    bool get_hovered() const { return isHovered; }
    bool get_active() const { return isActive; }
    void setTextPos(float x, float y) { text.setPosition(x, y); }
};

class LevelDesign {
private:
    Frame main, resources, request, processes;
    Button grant, deny;
    RectangleShape background;
public:
    LevelDesign(const Font& font);
    void draw(RenderWindow& w);
    void reconstruct(int l = 0);
    void setElemColor(const string& name, const sf::Color& Col1, const sf::Color& Col2);
    int interactive(const Vector2f& mousePos, bool cl);
};
