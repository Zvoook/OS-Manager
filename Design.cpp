#include "Design.h"
#include "Constants.h"

Frame::Frame(const Font& font, const String& tite, float s)
{
    //black.setFillColor(bordo);
    border.setFillColor(white);
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
    //black.setFillColor(white);
    border.setFillColor(white);
    border.setOutlineThickness(2.f);
    border.setOutlineColor(black);
    text.setFont(font);
    text.setString(tite);
    text.setCharacterSize(s);
    text.setFillColor(black);
}
void Button::hover(bool x) {
    isHovered = x;
    if (isHovered) {
        border.setOutlineColor(pale_green);
        text.setFillColor(pale_green);
    }
    else {
        border.setOutlineColor(black);
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

void LevelDesign::setFrameColor(const string& name, const sf::Color& Color) {
    if (name == "main") main.setColor(Color);
    else if (name == "resources") resources.setColor(Color);
    else if (name == "request") request.setColor(Color);
    else if (name == "processes") processes.setColor(Color);
    else if (name == "grant") grant.setColor(Color);
    else if (name == "deny") deny.setColor(Color);
}

int LevelDesign::interactive(const Vector2f& mousePos)
{
    grant.hover(grant.contains(mousePos));
    grant.activate(grant.get_hovered());
    if (grant.get_active()) return 1;


    deny.hover(deny.contains(mousePos));
    deny.activate(deny.get_hovered());
    if (deny.get_active()) return -1;
    return 0;
}

void LevelDesign::reconstruct(int l) {
    if (!l) return;
    if (l == 1) {
        setFrameColor("main", sf::Color(0, 0, 0));
    }
    else if (l == 2) {

    }
    else if (l == 3) {

    }
}
