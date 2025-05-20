// Design.cpp
#include "Design.h"
#include "Constants.h"

////////////////////////////////////////////////////////////////////////////////
// LevelTransition

void Animation::set_description() {
    levelDescriptions = {
        "EASY",
        "MIDDLE",
        "HARD",
        "IMPOSSIBLE"
    };
}

Animation::Animation(const Font& font)
    : state(State::Done), cur_level(1), isActive(false)
{
    overlay.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
    overlay.setFillColor(Color(0, 0, 0, 0));

    title_text.setFont(font);
    title_text.setCharacterSize(48);
    title_text.setFillColor(Color::White);
    title_text.setStyle(Text::Bold);

    description.setFont(font);
    description.setCharacterSize(24);
    description.setFillColor(Color::White);

    progress_background.setSize({ 600.f, 10.f });
    progress_background.setFillColor(Color(100, 100, 100, 200));

    progress_bar.setSize({ 0.f, 10.f });
    progress_bar.setFillColor(Color::White);

    levelColorSchemes = {
        { lightBlue, blue },
        { lightGreen, green },
        { lightOrange, orange },
        { lightRed, red }
    };

    set_description();
}

void Animation::start_animation(int newLevel) {
    cur_level = newLevel;

    isActive = true;
    state = State::FadeIn;
    anima_clock.restart();
    currentTime = 0.f;

    title_text.setString("LEVEL " + to_string(cur_level));
    description.setString(
        (cur_level >= 1 && cur_level <= (int)levelDescriptions.size())
        ? levelDescriptions[cur_level - 1]
        : "Unknown"
    );

    float w1 = title_text.getLocalBounds().width;
    title_text.setPosition(SCREEN_WIDTH / 2 - w1 / 2, SCREEN_HEIGHT / 2 - 60.f);
    float w2 = description.getLocalBounds().width;
    description.setPosition(SCREEN_WIDTH / 2 - w2 / 2, SCREEN_HEIGHT / 2 + 10.f);
}

bool Animation::update() {
    if (!isActive) return false;
    float dt = anima_clock.restart().asSeconds();
    currentTime += dt;

    float total = fade_in_time + show_info + fade_out_time;
    float prog = currentTime / total;
    progress_bar.setSize({ prog * 600.f, 10.f });

    switch (state) {
    case State::FadeIn:
        if (currentTime <= fade_in_time) {
            auto alpha = Uint8((currentTime / fade_in_time) * 255);
            overlay.setFillColor(Color(0, 0, 0, alpha));
        }
        else {
            state = State::ShowInfo;
            currentTime = fade_in_time;
        }
        break;
    case State::ShowInfo:
        if (currentTime <= fade_in_time + show_info) {
            overlay.setFillColor(Color(0, 0, 0, 200));
        }
        else {
            state = State::FadeOut;
        }
        break;
    case State::FadeOut:
        if (currentTime <= total) {
            auto alpha = Uint8(255 - ((currentTime - fade_in_time - show_info) / fade_out_time) * 255);
            overlay.setFillColor(Color(0, 0, 0, alpha));
        }
        else {
            state = State::Done;
            isActive = false;
            return true;
        }
        break;
    case State::Done:
        isActive = false;
        return true;
    }
    return false;
}

pair<Color, Color> Animation::get_lvl_colors() const {
    if (cur_level >= 1 && cur_level <= (int)levelColorSchemes.size())
        return { levelColorSchemes[cur_level - 1].background,
                 levelColorSchemes[cur_level - 1].main };
    return { white, black };
}

void Animation::draw(RenderTarget& t, RenderStates s) const {
    if (!isActive) return;
    t.draw(overlay, s);
    if (state == State::ShowInfo || state == State::FadeOut) {
        t.draw(title_text, s);
        t.draw(description, s);
        t.draw(progress_background, s);
        t.draw(progress_bar, s);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Frame

Frame::Frame(const Font& font, const String& tite, float s) {
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
void Frame::setSize(const Vector2f& s) { size = s; back.setSize(s); border.setSize(s); }
void Frame::setPosition(const Vector2f& p) { pos = p; back.setPosition(p); border.setPosition(p); }

////////////////////////////////////////////////////////////////////////////////
// Button

Button::Button(const Font& font, const String& tite, float s)
    : isHovered(false), isActive(false)
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
        if (t) { border.setFillColor(darkGreen); text.setFillColor(white); }
        else { border.setFillColor(darkRed); text.setFillColor(white); }
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
void Button::draw(RenderTarget& target, RenderStates states) const {
    target.draw(back, states);
    target.draw(border, states);
    target.draw(text, states);
}
void Button::setSize(const Vector2f& s) { size = s; back.setSize(s); border.setSize(s); }
void Button::setPosition(const Vector2f& p) { pos = p; back.setPosition(p); border.setPosition(p); }

////////////////////////////////////////////////////////////////////////////////
// LevelDesign

LevelDesign::LevelDesign(const Font& font) {
    background.setFillColor(white);
    background.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });

    main = Frame(font, "RESOURCE OPERATOR", 26);
    main.setPosition({ MAIN_X, MAIN_Y });
    main.setSize({ MAIN_WIDTH, MAIN_HEIGHT });
    main.setTextPos(355, 37);

    resources = Frame(font, "RESOURCES", 22);
    resources.setPosition({ FRAME_X_RES, FRAME_Y_RES });
    resources.setSize({ FRAME_WIDTH_RES, FRAME_HEIGHT_RES });
    resources.setTextPos(135, 110);

    request = Frame(font, "REQUEST", 22);
    request.setPosition({ FRAME_X_REQ, FRAME_Y_REQ });
    request.setSize({ FRAME_WIDTH_REQ, FRAME_HEIGHT_REQ });
    request.setTextPos(630, 110);

    processes = Frame(font, "PROCESSES", 22);
    processes.setPosition({ FRAME_X_PROC, FRAME_Y_PROC });
    processes.setSize({ FRAME_WIDTH_PROC, FRAME_HEIGHT_PROC });
    processes.setTextPos(430, 430);

    grant = Button(font, "GRANT", 22);
    grant.setPosition({ BUTTON_X_GRANT, BUTTON_Y });
    grant.setSize({ BUTTON_WIDTH, BUTTON_HEIGHT });
    grant.setTextPos(495, 335);

    deny = Button(font, "DENY", 22);
    deny.setPosition({ BUTTON_X_DENY, BUTTON_Y });
    deny.setSize({ BUTTON_WIDTH, BUTTON_HEIGHT });
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
    if (name == "main")     main.setColor(Col1, Col2);
    else if (name == "frames") {
        resources.setColor(Col1, Col2);
        request.setColor(Col1, Col2);
        processes.setColor(Col1, Col2);
    }
    else if (name == "back") background.setFillColor(Col1);
}
int LevelDesign::interactive(const Vector2f& mousePos, bool cl) {
    grant.hover(grant.contains(mousePos), 1);
    if (cl) grant.activate(grant.get_hovered());
    deny.hover(deny.contains(mousePos), 0);
    if (cl) deny.activate(deny.get_hovered());
    return grant.get_hovered() - deny.get_hovered();
}
void LevelDesign::reconstruct(int l) {
    if (l == 1) { setElemColor("back", lightBlue, black);  setElemColor("main", blue, black); }
    if (l == 2) { setElemColor("back", lightGreen, lightRed); setElemColor("main", green, black); }
    if (l == 3) { setElemColor("back", lightOrange, lightOrange); setElemColor("main", orange, black); }
    if (l == 4) { setElemColor("back", lightRed, lightRed);  setElemColor("main", red, black); }
}

////////////////////////////////////////////////////////////////////////////////
// Input

Input::Input(const Font& font, Vector2f position, Vector2f size) : active(false) {
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
    accept.setPosition({ OK_FIELD_X + OK_FIELD_WIDTH + 40, OK_Y });
    accept.setSize({ OK_BUTTION_WIDTH, OK_HEIGHT });
    accept.setTextPos(OK_FIELD_X + OK_FIELD_WIDTH + 45, OK_Y + 5);
}
void Input::handleEvent(const Event& event) {
    if (event.type == Event::MouseButtonPressed) {
        Vector2f m(event.mouseButton.x, event.mouseButton.y);
        active = texture.getGlobalBounds().contains(m);
        texture.setOutlineColor(active ? Color::Blue : Color::Black);
    }
    else if (active && event.type == Event::TextEntered) {
        if (event.text.unicode == '\b') {
            if (!content.empty()) content.pop_back();
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 128) {
            content += (wchar_t)event.text.unicode;
        }
        text.setString(content);
    }
}
void Input::draw(RenderWindow& w) const {
    w.draw(texture);
    w.draw(accept);
    w.draw(text);
}

////////////////////////////////////////////////////////////////////////////////
// Menu

Menu::Menu(const Font& font)
    : anime(font)   // инициализация анимации
{
    background.setFillColor(lightBlue);
    background.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });

    main = Frame(font, "MAIN MENU", 80);
    main.setPosition({ MAIN_X, MAIN_Y });
    main.setSize({ MAIN_WIDTH, MAIN_HEIGHT });
    main.setTextPos(SCREEN_WIDTH / 3.4f, MAIN_Y + 10);
    main.setColor(white, black);

    info = Frame(font, "ABOUT GAME", 30);
    info.setPosition({ INFO_X, INFO_Y });
    info.setSize({ INFO_WIDTH, INFO_HEIGHT });
    info.setTextPos(INFO_X + INFO_WIDTH / 2.5f, INFO_Y + INFO_HEIGHT / 12);
    info.setColor(yellow, black);

    input = Input(font, { OK_FIELD_X, OK_Y }, { OK_FIELD_WIDTH, OK_HEIGHT });

    stats = Button(font, "CHECK STATS", 26);
    stats.setPosition({ STATS_X, STATS_Y });
    stats.setSize({ STATS_WIDTH, STATS_HEIGHT });
    stats.setTextPos(STATS_X + STATS_WIDTH / 8, STATS_Y + STATS_HEIGHT / 3);
    stats.setColor(yellow, black);

    l1 = Button(font, "LEVEL 1", 22);
    l1.setPosition({ L1_X, L_Y });
    l1.setSize({ LEVEL_WIDTH, LEVEL_HEIGHT });
    l1.setTextPos(L1_X + LEVEL_WIDTH / 4, L_Y + LEVEL_HEIGHT / 3);
    l1.setColor(lightBlue, black);

    l2 = Button(font, "LEVEL 2", 22);
    l2.setPosition({ L2_X, L_Y });
    l2.setSize({ LEVEL_WIDTH, LEVEL_HEIGHT });
    l2.setTextPos(L2_X + LEVEL_WIDTH / 4, L_Y + LEVEL_HEIGHT / 3);
    l2.setColor(lightGreen, black);

    l3 = Button(font, "LEVEL 3", 22);
    l3.setPosition({ L3_X, L_Y });
    l3.setSize({ LEVEL_WIDTH, LEVEL_HEIGHT });
    l3.setTextPos(L3_X + LEVEL_WIDTH / 4, L_Y + LEVEL_HEIGHT / 3);
    l3.setColor(lightOrange, black);

    l4 = Button(font, "LEVEL 4", 22);
    l4.setPosition({ L4_X, L_Y });
    l4.setSize({ LEVEL_WIDTH, LEVEL_HEIGHT });
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

int Menu::interactive(const Vector2f& mousePos, bool click) {
    l1.hover(l1.contains(mousePos), l1.getColor());
    l2.hover(l2.contains(mousePos), l2.getColor());
    l3.hover(l3.contains(mousePos), l3.getColor());
    l4.hover(l4.contains(mousePos), l4.getColor());
    if (click) {
        if (l1.get_hovered()) { anime.start_animation(1); return 1; }
        if (l2.get_hovered()) { anime.start_animation(2); return 2; }
        if (l3.get_hovered()) { anime.start_animation(3); return 3; }
        if (l4.get_hovered()) { anime.start_animation(4); return 4; }
    }
    return 0;
}

void Menu::update_animation(float dt, GameState& gameState) {
    if (anime.is_transition_active()) {
        if (anime.update()) {
            gameState.init_level(anime.get_curr_lvl());
        }
    }
}

void Menu::draw_animation(RenderWindow& w) const {
    anime.draw(w, RenderStates::Default);
}
