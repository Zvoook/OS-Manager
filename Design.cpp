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

Button::Button(const Font& font, const String& tite, float s) : isHovered(false), isActive(false), can_activate(true)
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
    // Сохраняем предыдущее состояние
    bool prev_hovered = isHovered;

    // Hover работает только для активных кнопок
    if (!can_activate) {
        isHovered = false;
        // Не меняем цвет если кнопка неактивна
        return;
    }

    isHovered = x;
    // Меняем цвет только если состояние изменилось
    if (isHovered != prev_hovered) {
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
}
void Button::hover(bool x) {
    // Сохраняем предыдущее состояние
    bool prev_hovered = isHovered;

    // Hover работает только для активных кнопок
    if (!can_activate) {
        isHovered = false;
        // Не меняем цвет если кнопка неактивна
        return;
    }

    isHovered = x;
    // Меняем цвет только если состояние изменилось
    if (isHovered != prev_hovered) {
        if (isHovered) {
            border.setOutlineColor(white);
            text.setFillColor(white);
        }
        else {
            border.setOutlineColor(black);
            text.setFillColor(black);
        }
    }
}
void Button::activate(bool x) {
    if (can_activate) isActive = x;
    else isActive = false;
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


    go_menu = Button(font, "MENU", 24);
    go_menu.setPosition(Vector2f(MENU_BUTTON_X, MENU_BUTTON_Y));
    go_menu.setSize(Vector2f(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT));
    go_menu.setTextPos(MENU_BUTTON_X + MENU_BUTTON_WIDTH / 4, MENU_BUTTON_Y + MENU_BUTTON_HEIGHT / 4);
}
void LevelDesign::draw(RenderWindow& w) {
    w.draw(background);
    main.draw(w, RenderStates::Default);
    resources.draw(w, RenderStates::Default);
    request.draw(w, RenderStates::Default);
    processes.draw(w, RenderStates::Default);
    grant.draw(w, RenderStates::Default);
    deny.draw(w, RenderStates::Default);
    go_menu.draw(w, RenderStates::Default);
}
void LevelDesign::resetButtonsActive() {
    grant.release_activate();
    deny.release_activate();
    go_menu.release_activate();
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
    grant.hover(grant.contains(mousePos), true);
    if (cl) grant.activate(grant.get_hovered());

    deny.hover(deny.contains(mousePos), false);
    if (cl) deny.activate(deny.get_hovered());

    go_menu.hover(go_menu.contains(mousePos));
    if (cl) go_menu.activate(go_menu.get_hovered());

    return grant.get_hovered() - deny.get_hovered() + 2 * go_menu.get_hovered();
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

Input::Input(const Font& font, Vector2f position, Vector2f size) : active(false)/*, used(false)*/
{
    texture.setPosition(position);
    texture.setSize(size);
    texture.setFillColor(gray);
    texture.setOutlineThickness(2.f);
    texture.setOutlineColor(Color::Black);

    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(darkGray);
    text.setPosition(position.x + 10, position.y + 10);
    text.setString("Input nickname & press ENTER");
}
void Input::handleEvent(const Event& event)
{
    if (event.type == Event::MouseButtonPressed) {
        Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
        active = texture.getGlobalBounds().contains(mouse);
        texture.setOutlineColor(active ? Color::Blue : Color::Black);
    }
    else if (active && event.type == Event::TextEntered) {
        hide_help();
        if (event.text.unicode == '\b') {
            if (!content.empty()) content.pop_back();
        }
        else if (event.text.unicode >= 32) {
            content += static_cast<wchar_t>(event.text.unicode);
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
void Input::hide_help()
{
    text.setFillColor(black);
    text.setString("");
}
std::string Input::get_content() const {
    std::string result;
    for (size_t i = 0; i < content.size(); i++) {
        wchar_t ch = content[i];
        // Простое приведение типов
        result += static_cast<char>(ch % 256);
    }
    return result;
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

    input = Input(font, { INP_FIELD_X, INP_Y }, { INP_FIELD_WIDTH, INP_HEIGHT });


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
void Menu::upd_buttons(int i)
{
    // Сначала деактивируем все кнопки уровней
    l1.setColor(darkGray, black); l1.set_can_activate(false);
    l2.setColor(darkGray, black); l2.set_can_activate(false);
    l3.setColor(darkGray, black); l3.set_can_activate(false);
    l4.setColor(darkGray, black); l4.set_can_activate(false);
    stats.setColor(yellow, black); stats.set_can_activate(true);
    if (i >= 1) l1.setColor(lightBlue, black); l1.set_can_activate(true);
    if (i >= 2) l2.setColor(lightGreen, black); l2.set_can_activate(true);
    if (i >= 3) l3.setColor(lightOrange, black); l3.set_can_activate(true);
    if (i >= 4) l4.setColor(lightRed, black); l4.set_can_activate(true);
}
// В класс Menu  
void Menu::resetButtonsActive() {
    l1.release_activate();
    l2.release_activate();
    l3.release_activate();
    l4.release_activate();
    stats.release_activate();
}
int Menu::interactive(const Vector2f& mousePos, bool click)
{
    bool s1 = l1.contains(mousePos);
    bool s2 = l2.contains(mousePos);
    bool s3 = l3.contains(mousePos);
    bool s4 = l4.contains(mousePos);
    bool m = stats.contains(mousePos);

    // Hover только для активных кнопок
    l1.hover(s1 && l1.get_can_activate());
    l2.hover(s2 && l2.get_can_activate());
    l3.hover(s3 && l3.get_can_activate());
    l4.hover(s4 && l4.get_can_activate());
    stats.hover(m && stats.get_can_activate());

    if (click) {
        if (s1 && l1.get_can_activate()) return 1;
        else if (s2 && l2.get_can_activate()) return 2;
        else if (s3 && l3.get_can_activate()) return 3;
        else if (s4 && l4.get_can_activate()) return 4;
        else if (m && stats.get_can_activate()) return 5;
    }
    return 0;
}
void Menu::handle(const Event& event)
{
    input.handleEvent(event);
}


Statistics::Statistics(const Font& font)
{
    background.setFillColor(lightBlue);
    background.setSize(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    main = Frame(font, "STATISTICS", 80);
    main.setPosition(Vector2f(MAIN_X, MAIN_Y));
    main.setSize(Vector2f(MAIN_WIDTH, MAIN_HEIGHT));
    // Исправляем позицию заголовка - центрируем по горизонтали
    main.setTextPos(SCREEN_WIDTH / 4, MAIN_Y + 10);
    main.setColor(white, black);

    // Добавляем рамку для таблицы статистики
    stats_frame = Frame(font, "LEADERBOARD", 22);
    stats_frame.setPosition(Vector2f(60, 140));
    stats_frame.setSize(Vector2f(900, 480));
    // Центрируем заголовок "LEADERBOARD"
    stats_frame.setTextPos(SCREEN_WIDTH / 2 - 80, 150);
    stats_frame.setColor(yellow, black);

    // Рамка для заголовка таблицы
    header_frame = Frame(font, "", 18);
    header_frame.setPosition(Vector2f(80, 190));
    header_frame.setSize(Vector2f(860, 40));
    header_frame.setColor(lightGreen, black);

    go_menu = Button(font, "MENU", 24);
    go_menu.setPosition(Vector2f(MENU_BUTTON_X, MENU_BUTTON_Y));
    go_menu.setSize(Vector2f(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT));
    go_menu.setTextPos(MENU_BUTTON_X + MENU_BUTTON_WIDTH / 4, MENU_BUTTON_Y + MENU_BUTTON_HEIGHT / 4);
}
void Statistics::draw(RenderWindow& w) {
    w.draw(background);
    main.draw(w, RenderStates::Default);
    stats_frame.draw(w, RenderStates::Default);
    header_frame.draw(w, RenderStates::Default);
    go_menu.draw(w, RenderStates::Default);
}
int Statistics::interactive(const Vector2f& mousePos, bool cl)
{
    go_menu.hover(go_menu.contains(mousePos));
    if (cl) go_menu.activate(go_menu.get_hovered());
    return go_menu.get_hovered();
}
