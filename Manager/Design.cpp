#include "Design.h"

Frame::Frame(const Font& font, const String& title, float s)
{
    back.setFillColor(white);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2.0f);
    border.setOutlineColor(blue);
    text.setFont(font);
    text.setString(title);
    text.setCharacterSize(s);
    text.setFillColor(blue);
}

Button::Button(const Font& font, const String& title, float s) : isHovered(false), isActive(false)
{
    back.setFillColor(gray_blue);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2.0f);
    border.setOutlineColor(sf::Color(100, 200, 220));
    text.setFont(font);
    text.setString(title);
    text.setCharacterSize(s);
    text.setFillColor(sf::Color(100, 200, 220));
}

void Frame::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(back, states);
    target.draw(border, states);
    target.draw(text, states);
}

void Frame::setSize(const Vector2f& s)
{
    size = s;
    back.setSize(s);
    border.setSize(s);
}

void Frame::setPosition(const Vector2f& p)
{
    pos = p;
    back.setPosition(p);
    border.setPosition(p);
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition(pos.x + (size.x - textBounds.width) / 2 + xFrameOffset, pos.y + yFrameOffset);
}

void Button::hover(bool x)
{
    isHovered = x;
    if (isHovered) { back.setFillColor(sf::Color(40, 80, 120)); }
    else { back.setFillColor(sf::Color(20, 40, 60)); }
}

void Button::activate(bool x)
{
    isActive = x;
    if (isActive) {
        border.setOutlineColor(sf::Color(150, 255, 150));
        text.setFillColor(sf::Color(150, 255, 150));
    }
    else {
        border.setOutlineColor(sf::Color(100, 200, 220));
        text.setFillColor(sf::Color(100, 200, 220));
    }
}

void Button::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(back, states);
    target.draw(border, states);
    target.draw(text, states);
}

void Button::setSize(const Vector2f& s)
{
    size = s;
    back.setSize(s);
    border.setSize(s);
}

void Button::setPosition(const Vector2f& p)
{
    pos = p;
    back.setPosition(p);
    border.setPosition(p);
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition(pos.x + (size.x - textBounds.width) / 2 + xButtonOffset, pos.y + yButtonOffset);
}

LevelDesign::LevelDesign(const Font& font)
{
    background.setFillColor(Color(10, 20, 30));
    background.setSize(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    main = Frame(font, L"Симуляция", SCREEN_HEIGHT * 0.045f);
    main.setPosition(Vector2f(MARGIN, MARGIN));
    main.setSize(Vector2f(SCREEN_WIDTH - 2 * MARGIN, FRAME_SCREEN_HEIGHT));

    resources = Frame(font, L"Ресурсы", SCREEN_HEIGHT * 0.035f);
    resources.setPosition(Vector2f(MARGIN, main.getBottom() + MARGIN));
    resources.setSize(Vector2f(FRAME_SCREEN_WIDTH, SMALL_FRAME_SCREEN_HEIGHT));

    request = Frame(font, L"Запрос", SCREEN_HEIGHT * 0.035f);
    request.setPosition(Vector2f(resources.getRight() + MARGIN, main.getBottom() + MARGIN));
    request.setSize(Vector2f(FRAME_SCREEN_WIDTH, SMALL_FRAME_SCREEN_HEIGHT));

    processes = Frame(font, L"Процессы", SCREEN_HEIGHT * 0.035f);
    processes.setPosition(Vector2f(MARGIN, resources.getBottom() + MARGIN));
    processes.setSize(Vector2f(SCREEN_WIDTH - 2 * MARGIN, 0.25f * SCREEN_HEIGHT));

    allow = Button(font, L"Разрешить", SCREEN_HEIGHT * 0.035f);
    allow.setPosition(Vector2f(MARGIN, processes.getBottom() + MARGIN));
    allow.setSize(Vector2f((SCREEN_WIDTH - 3 * MARGIN) / 2.f, SMALL_FRAME_SCREEN_HEIGHT));

    deny = Button(font, L"Отклонить", SCREEN_HEIGHT * 0.035f);
    deny.setPosition(Vector2f(allow.getRight() + MARGIN, processes.getBottom() + MARGIN));
    deny.setSize(Vector2f((SCREEN_WIDTH - 3 * MARGIN) / 2.f, SMALL_FRAME_SCREEN_HEIGHT));
}

void LevelDesign::draw(RenderWindow& w) {
    w.draw(background);
    main.draw(w, RenderStates::Default);
    resources.draw(w, RenderStates::Default);
    request.draw(w, RenderStates::Default);
    processes.draw(w, RenderStates::Default);
    allow.draw(w, RenderStates::Default);
    deny.draw(w, RenderStates::Default);
}


void LevelDesign::setFrameColor(const string& name, const sf::Color& color)
{
    if (name == "main") main.setColor(color);
    else if (name == "resources") resources.setColor(color);
    else if (name == "request") request.setColor(color);
    else if (name == "processes") processes.setColor(color);
    else if (name == "allow") allow.setColor(color);
    else if (name == "deny") deny.setColor(color);
}

void LevelDesign::reconstruct(int level)
{
    setFrameColor("main", sf::Color(0, 0, 0));
    setFrameColor("main", sf::Color(0, 0, 0));
}