#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <string>
#include <iostream>
using namespace sf;

class Frame {
protected:
    RectangleShape back, border/*, highlight*/;
    Text text;
public:
    Frame() = default;
    Frame(float x, float y, const Font& font, const String& title, float size = 24);
    void draw(RenderTarget& target, RenderStates states) const;
};

class CyberButton : public Drawable {
private:
    RectangleShape back;
    RectangleShape border;
    Text buttonText;
    bool isHovered;
    bool isActive;

public:
    CyberButton(float x, float y, float width, float height,
        const Font& font, const String& text, float textSize = 20)
        : isHovered(false), isActive(false) {

        // Настройка фона
        back.setPosition(x, y);
        back.setSize(Vector2f(width, height));
        back.setFillColor(Color(20, 40, 60));

        // Настройка рамки
        border.setPosition(x, y);
        border.setSize(Vector2f(width, height));
        border.setFillColor(Color::Transparent);
        border.setOutlineThickness(2.0f);
        border.setOutlineColor(Color(100, 200, 220));

        // Настройка текста - используем String напрямую
        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(textSize);
        buttonText.setFillColor(Color(100, 200, 220));

        // Центрирование текста
        FloatRect textBounds = buttonText.getLocalBounds();
        buttonText.setPosition(
            x + (width - textBounds.width) / 2,
            y + (height - textBounds.height) / 2 - 5
        );
    }

    bool contains(Vector2f point) const {
        return back.getGlobalBounds().contains(point);
    }

    void setHovered(bool hovered) {
        isHovered = hovered;
        if (isHovered) {
            back.setFillColor(Color(40, 80, 120));
        }
        else {
            back.setFillColor(Color(20, 40, 60));
        }
    }

    void setActive(bool active) {
        isActive = active;
        if (isActive) {
            border.setOutlineColor(Color(150, 255, 150));
            buttonText.setFillColor(Color(150, 255, 150));
        }
        else {
            border.setOutlineColor(Color(100, 200, 220));
            buttonText.setFillColor(Color(100, 200, 220));
        }
    }

    void draw(RenderTarget& target, RenderStates states) const override {
        target.draw(back, states);
        target.draw(border, states);
        target.draw(buttonText, states);
    }
};

class LevelDesign : public Frame {
private:
    Frame mainFrame, resourcesFrame, requestFrame, grantButton, denyButton, processesFrame;
    RectangleShape background;
public:
    LevelDesign(const Font& font);
    RectangleShape& get_background() { return background; }
};