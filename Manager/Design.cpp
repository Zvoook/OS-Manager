#include "Design.h"
#include "Constants.h"

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

void Frame::draw(RenderTarget& target, RenderStates states) const {
    target.draw(back, states);
    target.draw(border, states);
    target.draw(text, states);
}

void Frame::setSize(const Vector2f& s) {
    size = s;
    back.setSize(s);
    border.setSize(s);
}

void Frame::setPosition(const Vector2f& p) {
    pos = p;
    back.setPosition(p);
    border.setPosition(p);
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition(pos.x + (size.x - textBounds.width) / 2 + FRAME_TEXT_OFFSET_X, pos.y + FRAME_TEXT_OFFSET_Y);
}

void Button::hover(bool x) {
    isHovered = x;
    if (isHovered) { back.setFillColor(sf::Color(40, 80, 120)); }
    else { back.setFillColor(sf::Color(20, 40, 60)); }
}

void Button::activate(bool x) {
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

void Button::draw(RenderTarget& target, RenderStates states) const {
    target.draw(back, states);
    target.draw(border, states);
    target.draw(text, states);
}

void Button::setSize(const Vector2f& s) {
    size = s;
    back.setSize(s);
    border.setSize(s);
}

void Button::setPosition(const Vector2f& p) {
    pos = p;
    back.setPosition(p);
    border.setPosition(p);
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition(pos.x + (size.x - textBounds.width) / 2 + BUTTON_TEXT_OFFSET_X, pos.y + BUTTON_TEXT_OFFSET_Y);
}

LevelDesign::LevelDesign(const Font& font) {
    background.setFillColor(Color(10, 20, 30));
    background.setSize(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    main = Frame(font, L"Симуляция", 32);
    main.setPosition(Vector2f(MAIN_X, MAIN_Y));
    main.setSize(Vector2f(MAIN_WIDTH, MAIN_HEIGHT));

    resources = Frame(font, L"Ресурсы", 26);
    resources.setPosition(Vector2f(FRAME_X_RES, FRAME_Y_RES));
    resources.setSize(Vector2f(FRAME_WIDTH_RES, FRAME_HEIGHT_RES));

    request = Frame(font, L"Запрос", 26);
    request.setPosition(Vector2f(FRAME_X_REQ, FRAME_Y_REQ));
    request.setSize(Vector2f(FRAME_WIDTH_REQ, FRAME_HEIGHT_REQ));

    processes = Frame(font, L"Процессы", 26);
    processes.setPosition(Vector2f(FRAME_X_PROC, FRAME_Y_PROC));
    processes.setSize(Vector2f(FRAME_WIDTH_PROC, FRAME_HEIGHT_PROC));

    allow = Button(font, L"Разрешить", 22);
    allow.setPosition(Vector2f(BUTTON_X_GRANT, BUTTON_Y));
    allow.setSize(Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));

    deny = Button(font, L"Отклонить", 22);
    deny.setPosition(Vector2f(BUTTON_X_DENY, BUTTON_Y));
    deny.setSize(Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
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

void LevelDesign::setFrameColor(const string& name, const sf::Color& color) {
    if (name == "main") main.setColor(color);
    else if (name == "resources") resources.setColor(color);
    else if (name == "request") request.setColor(color);
    else if (name == "processes") processes.setColor(color);
    else if (name == "allow") allow.setColor(color);
    else if (name == "deny") deny.setColor(color);
}

void LevelDesign::interactive(const Vector2f& mousePos)
{
    allow.hover(allow.contains(mousePos));
    allow.activate(allow.get_hovered());
    deny.hover(deny.contains(mousePos));
    deny.activate(deny.get_hovered());
}

void LevelDesign::reconstruct(int level) {
    setFrameColor("main", sf::Color(0, 0, 0));
}