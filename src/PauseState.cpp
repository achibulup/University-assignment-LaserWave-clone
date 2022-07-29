#include "State.hpp"
#include "SimpleGUI.hpp"

#include "assets.hpp"
#include "constants.hpp"
#include "stateRequests.hpp"
#include "draw.hpp"


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
    const sf::Cursor *m_cursor = {};
    SimpleGUI m_GUI = {};
    sf::Text m_pause_text = {};
};
const State::Id PauseState::ID ("PauseState");
const State::Id PAUSESTATE_ID = PauseState::ID;


template StateRequest makePushRequest<PauseState>();



PauseState::PauseState(GameDataRef data)
: State (data)
{
    this->m_cursor = &this->getAssets().getCursor(DEFAULT_CURSOR);

    this->m_pause_text.setString(PAUSE_TITLE);
    this->m_pause_text.setFont(this->getAssets().getFont(PAUSE_FONT));
    this->m_pause_text.setCharacterSize(PAUSE_TITLE_FONT_SIZE);
    this->m_pause_text.setPosition({
        getCenterX(this->getWindow(), this->m_pause_text),
        PAUSE_TITLE_Y
    });
    this->m_pause_text.setFillColor(PAUSE_TITLE_COLOR);

    auto &asset = this->getAssets();

    const sf::Texture *resume_texture = &asset.getTexture(RESUME_BUTTON);
    const sf::Texture *resume_hover_texture = &asset.getTexture(RESUME_BUTTON_HOVER);
    const sf::Texture *restart_texture = &asset.getTexture(RESTART_BUTTON);
    const sf::Texture *restart_hover_texture = &asset.getTexture(RESTART_BUTTON_HOVER);
    const sf::Texture *menu_texture = &asset.getTexture(MENU_BUTTON);
    const sf::Texture *menu_hover_texture = &asset.getTexture(MENU_BUTTON_HOVER);

    sf::Vector2f resume_pos = {      
        getCenterX(this->getWindow(), *resume_texture),
        RESUME_BUTTON_Y
    };
    sf::Vector2f restart_pos = {
        getCenterX(this->getWindow(), *restart_texture),
        RESTART_BUTTON_Y
    };
    sf::Vector2f menu_pos = {
        getCenterX(this->getWindow(), *menu_texture),
        MENU_BUTTON_Y
    };


    SimpleButton resume_button = makeButton(
        resume_texture, resume_pos,
        [this] {
            this->addStateRequest(makePopRequest());
        },
        resume_hover_texture
    );
    SimpleButton restart_button = makeButton(
        restart_texture, restart_pos,
        [this] {
            auto restart_request = makeRequests(
                makePopPastRequest(PLAYINGSTATE_ID),
                makePushRequest<PlayingState>()
            );
            this->addStateRequest(makePushRequest<WipeTransitionState>(
                std::move(restart_request)));
        },
        restart_hover_texture
    );
    SimpleButton menu_button = makeButton(
        menu_texture, menu_pos,
        [this] {
            this->addStateRequest(makePopPastRequest(PLAYINGSTATE_ID));
        },
        menu_hover_texture
    );

    this->m_GUI.addButton(std::move(resume_button))
               .addButton(std::move(restart_button))
               .addButton(std::move(menu_button));
}
PauseState::~PauseState() = default;

void PauseState::update(sf::Time dt, EventManager &event)
{
    if (this->m_GUI.respondToEvent(event)) {}
    else if (event.isKeyPressed(sf::Keyboard::P) 
          || event.isKeyPressed(sf::Keyboard::Escape)) {
      this->addStateRequest(makePopRequest());
    }
    else if (event.isMouseButtonPressed(sf::Mouse::Right))
      this->addStateRequest(makePopRequest());
}

void PauseState::render() const
{
    ///fill the screen
    Rect rect;
    rect.setSize({this->getWindow().getSize().x, this->getWindow().getSize().y});
    rect.setPosition({0, 0});
    rect.setFillColor(PAUSE_FILTER);
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