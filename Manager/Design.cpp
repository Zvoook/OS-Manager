#include "Design.h"

Frame::Frame(float x, float y, const Font& font, const String& title, float size)
{
    // Настройка фона
    back.setPosition(x, y);
    back.setSize(Vector2f(width, height));
    back.setFillColor(Color(10, 20, 40, 230));

    // Настройка рамки
    border.setPosition(x, y);
    border.setSize(Vector2f(width, height));
    border.setFillColor(Color::Transparent);
    border.setOutlineThickness(2.0f);
    border.setOutlineColor(Color(100, 200, 220));

    //// Настройка подсветки
    //highlight.setPosition(x + 2, y + 2);
    //highlight.setSize(Vector2f(width - 4, height - 4));
    //highlight.setFillColor(Color::Transparent);
    //highlight.setOutlineThickness(1.0f);
    //highlight.setOutlineColor(Color(50, 100, 150));

    // Настройка текста заголовка
    text.setFont(font);
    text.setString(title);
    text.setCharacterSize(size);
    text.setFillColor(Color(100, 200, 220));

    // Центрирование текста по горизонтали
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition(x + (width - textBounds.width) / 2, y + 5);
}

void Frame::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(back, states);
    target.draw(border, states);
    //target.draw(highlight, states);
    target.draw(text, states);
}

LevelDesign::LevelDesign(const Font& font) {
    Frame mainFrame(30, 30, /*964, 708,*/ font, L"Simulation", 28);
    Frame resourcesFrame(60, 100, /*300, 300,*/ font, L"Resources", 22);
    Frame requestFrame(400, 100, /*560, 200,*/ font, L"Request", 22);
    Frame grantButton(400, 320, /*270, 60,*/ font, L"Allow", 22);
    Frame denyButton(690, 320, /*270, 60,*/ font, L"Deny", 22);
    Frame processesFrame(60, 420, /*900, 280,*/ font, L"Processes", 22);
    background.setFillColor(Color(10, 20, 30));
}