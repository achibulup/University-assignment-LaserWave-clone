#include "State.hpp"
#include "SimpleGUI.hpp"

#include "assets.hpp"
#include "constants.hpp"
#include "stateRequests.hpp"
#include "draw_convex.hpp"


namespace LaserWave
{

class PauseState : public State INIT_DEBUG_ID(PauseState)
{
  public:
    static const State::Id ID;

    explicit PauseState(GameDataRef data);
    PauseState(const PauseState&) = delete;
    void operator = (PauseState) = delete;
    ~PauseState();

    virtual State::Id getId() const noexcept override
    {
        return ID;
    }

    void update(sf::Time dt, EventManager &event) override;
    void render() const override;

    void asTopState() override;

  private:
    const sf::Cursor *m_cursor;
    SimpleGUI m_GUI;
    sf::Text m_pause_text;
};
const State::Id PauseState::ID("PauseState");
const State::Id PAUSESTATE_ID = PauseState::ID;


template StateRequest makePushRequest<PauseState>();



PauseState::PauseState(GameDataRef data)
: State(data), m_cursor(&this->getAssets().getCursor(DEFAULT_CURSOR)),
  m_pause_text(PAUSE_TITLE, this->getAssets().getFont(PAUSE_FONT),
               PAUSE_TITLE_FONT_SIZE)
{
    m_pause_text.setPosition({this->getWindow().getSize().x / 2 
                           -  m_pause_text.getGlobalBounds().width / 2,
                              PAUSE_TITLE_Y});
    m_pause_text.setFillColor(PAUSE_TITLE_COLOR);

    const sf::Texture *resume_texture = &this->getAssets().getTexture(RESUME_BUTTON);
    const sf::Texture *resume_hover_texture = &this->getAssets().getTexture(RESUME_BUTTON_HOVER);
    const sf::Texture *restart_texture = &this->getAssets().getTexture(RESTART_BUTTON);
    const sf::Texture *restart_hover_texture = &this->getAssets().getTexture(RESTART_BUTTON_HOVER);
    const sf::Texture *menu_texture = &this->getAssets().getTexture(MENU_BUTTON);
    const sf::Texture *menu_hover_texture = &this->getAssets().getTexture(MENU_BUTTON_HOVER);

    sf::Vector2f resume_pos = {
        this->getWindow().getSize().x / 2 - resume_texture->getSize().x / 2,
        RESUME_BUTTON_Y
    };
    sf::Vector2f restart_pos = {
        this->getWindow().getSize().x / 2 - restart_texture->getSize().x / 2,
        RESTART_BUTTON_Y
    };
    sf::Vector2f menu_pos = {
        this->getWindow().getSize().x / 2 - menu_texture->getSize().x / 2,
        MENU_BUTTON_Y
    };

    SimpleButton resume_button = makeButton(resume_texture, resume_pos,
        [this]() {
            this->addStateRequest(makePopRequest());
        },
        resume_hover_texture
    );
    SimpleButton restart_button = makeButton(restart_texture, restart_pos,
        [this]() {
            auto restart_request = makeRequests(
                makePopPastRequest(PLAYINGSTATE_ID),
                makePushRequest<PlayingState>()
            );
            this->addStateRequest(makePushRequest<WipeTransitionState>(
                std::move(restart_request)));
        },
        restart_hover_texture
    );
    SimpleButton menu_button = makeButton(menu_texture, menu_pos,
        [this]() {
            this->addStateRequest(makePopPastRequest(PLAYINGSTATE_ID));
        },
        menu_hover_texture
    );

    this->m_GUI.addButton(std::move(resume_button));
    this->m_GUI.addButton(std::move(restart_button));
    this->m_GUI.addButton(std::move(menu_button));
}
PauseState::~PauseState() = default;

void PauseState::update(sf::Time dt, EventManager &event)
{
    if (this->m_GUI.respondToEvent(event)) {}
    else if (event.isKeyPressed(sf::Keyboard::P) 
          || event.isKeyPressed(sf::Keyboard::Escape)) {
      this->addStateRequest(makePopRequest());
    }
}

void PauseState::render() const
{
    ///fill the screen
    Rect rect;
    rect.setSize({this->getWindow().getSize().x, this->getWindow().getSize().y});
    rect.setPosition({0, 0});
    rect.setColor(PAUSE_FILTER);
    this->getWindow().draw(rect);

    this->getWindow().draw(m_pause_text);
    this->m_GUI.drawTo(this->getWindow());
}

void PauseState::asTopState()
{
    this->getWindow().setMouseCursor(*this->m_cursor);
    this->getWindow().setMouseCursorVisible(true);
}

} // namespace LaserWave