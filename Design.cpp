#include "Design.h"
#include "Constants.h"

Frame::Frame(const Font& font, const String& tite, float s)
{
    border.setFillColor(yellow);
    border.setOutlineThickness(2.f);
    border.setOutlineColor(black);
    text.setFont(font);
    text.setString(tite);
    text.setCharacterSize(s);
    text.setFillColor(black);
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
}

Button::Button(const Font& font, const String& tite, float s) : isHovered(false), isActive(false)
{
    border.setFillColor(yellow);
    border.setOutlineThickness(2.f);
    border.setOutlineColor(black);
    text.setFont(font);
    text.setString(tite);
    text.setCharacterSize(s);
    text.setFillColor(black);
}
void Button::hover(bool x, bool t) {
    isHovered = x;
    if (isHovered) {
        if (t) {
            border.setFillColor(darkGreen);
            text.setFillColor(white);
        }
        else {
            border.setFillColor(darkRed);
            text.setFillColor(white);
        }
    }
    else {
        border.setFillColor(yellow);
        text.setFillColor(black);
    }
}
void Button::hover(bool x, Color self) {
    isHovered = x;
    if (isHovered) {
        border.setFillColor(darkGreen);
        text.setFillColor(white);
    }
    else {
        border.setFillColor(self);
        text.setFillColor(black);
    }
}
void Button::activate(bool x) { isActive = x; }
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
}

LevelDesign::LevelDesign(const Font& font) {
    background.setFillColor(white);
    background.setSize(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    main = Frame(font, "RESOURCE OPERATOR", 26);
    main.setPosition(Vector2f(MAIN_X, MAIN_Y));
    main.setSize(Vector2f(MAIN_WIDTH, MAIN_HEIGHT));
    main.setTextPos(355, 37);

    resources = Frame(font, "RESOURCES", 22);
    resources.setPosition(Vector2f(FRAME_X_RES, FRAME_Y_RES));
    resources.setSize(Vector2f(FRAME_WIDTH_RES, FRAME_HEIGHT_RES));
    resources.setTextPos(135, 110);

    request = Frame(font, "REQUEST", 22);
    request.setPosition(Vector2f(FRAME_X_REQ, FRAME_Y_REQ));
    request.setSize(Vector2f(FRAME_WIDTH_REQ, FRAME_HEIGHT_REQ));
    request.setTextPos(630, 110);

    processes = Frame(font, "PROCESSES", 22);
    processes.setPosition(Vector2f(FRAME_X_PROC, FRAME_Y_PROC));
    processes.setSize(Vector2f(FRAME_WIDTH_PROC, FRAME_HEIGHT_PROC));
    processes.setTextPos(430, 430);

    grant = Button(font, "GRANT", 22);
    grant.setPosition(Vector2f(BUTTON_X_GRANT, BUTTON_Y));
    grant.setSize(Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    grant.setTextPos(495, 335);

    deny = Button(font, "DENY", 22);
    deny.setPosition(Vector2f(BUTTON_X_DENY, BUTTON_Y));
    deny.setSize(Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    deny.setTextPos(790, 335);
}
void LevelDesign::draw(RenderWindow& w) {
    w.draw(background);
    main.draw(w, RenderStates::Default);
    resources.draw(w, RenderStates::Default);
    request.draw(w, RenderStates::Default);
    processes.draw(w, RenderStates::Default);
    grant.draw(w, RenderStates::Default);
    deny.draw(w, RenderStates::Default);
}
void LevelDesign::setElemColor(const string& name, const Color& Col1, const Color& Col2) {
    if (name == "main") main.setColor(Col1, Col2);
    else if (name == "frames") {
        resources.setColor(Col1, Col2);
        request.setColor(Col1, Col2);
        processes.setColor(Col1, Col2);
    }
    else if (name == "back") background.setFillColor(Col1);
}
int LevelDesign::interactive(const Vector2f& mousePos, bool cl)
{
    grant.hover(grant.contains(mousePos), 1);
    if (cl) grant.activate(grant.get_hovered());

    deny.hover(deny.contains(mousePos), 0);
    if (cl) deny.activate(deny.get_hovered());

    return grant.get_hovered() - deny.get_hovered();
}
void LevelDesign::reconstruct(int l) {
    if (l == 1) {
        setElemColor("back", lightBlue, black);
        setElemColor("main", blue, black);
    }
    else if (l == 2) {
        setElemColor("back", lightGreen, lightRed);
        setElemColor("main", green, black);
    }
    else if (l == 3) {
        setElemColor("back", lightOrange, lightOrange);
        setElemColor("main", orange, black);
    }
    else if (l == 4) {
        setElemColor("back", lightRed, lightRed);
        setElemColor("main", red, black);
    }
}

Input::Input(const Font& font, Vector2f position, Vector2f size) : active(false)
{
    texture.setPosition(position);
    texture.setSize(size);
    texture.setFillColor(Color(240, 240, 240));
    texture.setOutlineThickness(2.f);
    texture.setOutlineColor(Color::Black);

    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(Color::Black);
    text.setPosition(position.x + 5, position.y + 5);

    accept = Button(font, "OK", 40);
    accept.setPosition(Vector2f(OK_FIELD_X + OK_FIELD_WIDTH + 40, OK_Y));
    accept.setSize(Vector2f(OK_BUTTION_WIDTH, OK_HEIGHT));
    accept.setTextPos(OK_FIELD_X + OK_FIELD_WIDTH + 45, OK_Y + 5);
}
void Input::handleEvent(const Event& event)
{
    if (event.type == Event::MouseButtonPressed) {
        Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
        active = texture.getGlobalBounds().contains(mouse);
        texture.setOutlineColor(active ? Color::Blue : Color::Black);
    }
    else if (active && event.type == Event::TextEntered) {
        if (event.text.unicode == '\b') {
            if (!content.empty()) content.pop_back();
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 128) {
            content += (wchar_t)(event.text.unicode);
        }
        text.setString(content);
    }
}
void Input::draw(RenderWindow& window) const
{
    window.draw(texture);
    window.draw(accept);
    window.draw(text);
}

Menu::Menu(const Font& font)
{
    background.setFillColor(lightBlue);
    background.setSize(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    main = Frame(font, "MAIN MENU", 80);
    main.setPosition(Vector2f(MAIN_X, MAIN_Y));
    main.setSize(Vector2f(MAIN_WIDTH, MAIN_HEIGHT));
    main.setTextPos(SCREEN_WIDTH / 3.4, MAIN_Y + 10);
    main.setColor(white, black);

    info = Frame(font, "ABOUT GAME", 30);
    info.setPosition(Vector2f(INFO_X, INFO_Y));
    info.setSize(Vector2f(INFO_WIDTH, INFO_HEIGHT));
    info.setTextPos(INFO_X + INFO_WIDTH / 2.5, INFO_Y + INFO_HEIGHT / 12);
    info.setColor(yellow, black);

    input = Input(font, { OK_FIELD_X, OK_Y }, { OK_FIELD_WIDTH, OK_HEIGHT });

    stats = Button(font, "CHECK STATS", 26);
    stats.setPosition(Vector2f(STATS_X, STATS_Y));
    stats.setSize(Vector2f(STATS_WIDTH, STATS_HEIGHT));
    stats.setTextPos(STATS_X + STATS_WIDTH / 8, STATS_Y + STATS_HEIGHT / 3);
    stats.setColor(yellow, black);

    l1 = Button(font, "LEVEL 1", 22);
    l1.setPosition(Vector2f(L1_X, L_Y));
    l1.setSize(Vector2f(LEVEL_WIDTH, LEVEL_HEIGHT));
    l1.setTextPos(L1_X + LEVEL_WIDTH / 4, L_Y + LEVEL_HEIGHT / 3);
    l1.setColor(lightBlue, black);

    l2 = Button(font, "LEVEL 2", 22);
    l2.setPosition(Vector2f(L2_X, L_Y));
    l2.setSize(Vector2f(LEVEL_WIDTH, LEVEL_HEIGHT));
    l2.setTextPos(L2_X + LEVEL_WIDTH / 4, L_Y + LEVEL_HEIGHT / 3);
    l2.setColor(lightGreen, black);

    l3 = Button(font, "LEVEL 3", 22);
    l3.setPosition(Vector2f(L3_X, L_Y));
    l3.setSize(Vector2f(LEVEL_WIDTH, LEVEL_HEIGHT));
    l3.setTextPos(L3_X + LEVEL_WIDTH / 4, L_Y + LEVEL_HEIGHT / 3);
    l3.setColor(lightOrange, black);

    l4 = Button(font, "LEVEL 4", 22);
    l4.setPosition(Vector2f(L4_X, L_Y));
    l4.setSize(Vector2f(LEVEL_WIDTH, LEVEL_HEIGHT));
    l4.setTextPos(L4_X + LEVEL_WIDTH / 4, L_Y + LEVEL_HEIGHT / 3);
    l4.setColor(lightRed, black);
}

void Menu::draw(RenderWindow& w) {
    w.draw(background);
    main.draw(w, RenderStates::Default);
    info.draw(w, RenderStates::Default);
    stats.draw(w, RenderStates::Default);
    l1.draw(w, RenderStates::Default);
    l2.draw(w, RenderStates::Default);
    l3.draw(w, RenderStates::Default);
    l4.draw(w, RenderStates::Default);
    input.draw(w);
}

int Menu::interactive(const Vector2f& mousePos)
{
    l1.hover(l1.contains(mousePos), l1.getColor());
    l2.hover(l2.contains(mousePos), l2.getColor());
    l3.hover(l3.contains(mousePos), l3.getColor());
    l4.hover(l4.contains(mousePos), l4.getColor());

    if (!input.is_empty()) {
        l1.activate(l1.get_hovered());
        l1.activate(l1.get_hovered());
        l1.activate(l1.get_hovered());
        l1.activate(l1.get_hovered());
    }
    return l1.get_active() + l2.get_active() * 2 + l3.get_active() * 3 + l4.get_active() + 4;
}