#include "PauseState.hpp"
#include "assets.hpp"
#include "constants.hpp"
#include "PlayingState.hpp"
#include "stateRequests.hpp"

namespace LaserWave
{

const State::Id PauseState::ID = "Pause";

PauseState::PauseState(GameDataRef data)
: State(data), m_cursor(&this->getAssets().getCursor(DEFAULT_CURSOR)),
  m_pause_text(PAUSE_TITLE, this->getAssets().getFont(PAUSE_FONT),
               PAUSE_TITLE_FONT_SIZE)
{
    m_pause_text.setPosition({this->getWindow().getSize().x / 2 
                           -  m_pause_text.getGlobalBounds().width / 2,
                              PAUSE_TITLE_Y});
    m_pause_text.setColor(PAUSE_TITLE_COLOR);

    const sf::Texture *resume_texture = &this->getAssets().getTexture(RESUME_BUTTON);
    const sf::Texture *restart_texture = &this->getAssets().getTexture(RESTART_BUTTON);
    const sf::Texture *menu_texture = &this->getAssets().getTexture(MENU_BUTTON);

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
            this->m_requests.push_back(makePopRequest());
        }
    );
    SimpleButton restart_button = makeButton(restart_texture, restart_pos,
        [this]() {
            this->m_requests.push_back(makePopPastRequest(PlayingState::ID));
            this->m_requests.push_back(makePushRequest<PlayingState>());
        }
    );
    SimpleButton menu_button = makeButton(menu_texture, menu_pos,
        [this]() {
            this->m_requests.push_back(makePopPastRequest(PlayingState::ID));
        }
    );

    this->m_GUI.addButton(std::move(resume_button));
    this->m_GUI.addButton(std::move(restart_button));
    this->m_GUI.addButton(std::move(menu_button));
}
PauseState::~PauseState() = default;

std::vector<StateRequest> PauseState::update(sf::Time dt, EventManager &event)
{
    if (this->m_GUI.respondToEvent(event)) {}
    else if (event.isKeyPressed(sf::Keyboard::P) 
          || event.isKeyPressed(sf::Keyboard::Escape)) {
      this->m_requests.push_back(makePopRequest());
    }
    return std::move(this->m_requests);
}

void PauseState::render() const
{
    ///fill the screen
    sf::RectangleShape rect;
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