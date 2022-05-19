#include "MenuState.hpp"
#include "assets.hpp"
#include "constants.hpp"
#include "PlayingState.hpp"

namespace LaserWave
{

sf::Color calcClickMessageColor(sf::Time time);

MenuState::MenuState(GameDataRef data)
: State(data),
  m_title(TITLE, this->getAssets().getFont(TITLE_FONT), TITLE_FONT_SIZE),
  m_click_to_play_message(CLICK_MESSAGE, this->getAssets().getFont(CLICK_FONT), CLICK_FONT_SIZE),
  m_cursor(&this->getAssets().getCursor(DEFAULT_CURSOR))
{
    this->m_title.setPosition(this->getWindow().getSize().x / 2
                            - this->m_title.getGlobalBounds().width / 2,
                              TITLE_Y);
    this->m_title.setColor(TITLE_COLOR);

    this->m_click_to_play_message.setPosition(
        this->getWindow().getSize().x / 2
      - this->m_click_to_play_message.getGlobalBounds().width / 2,
        CLICK_Y);
    this->m_click_to_play_message.setColor(
        calcClickMessageColor(this->m_message_timer));


    const sf::Texture *exit_texture = &this->getAssets().getTexture(EXIT_BUTTON);
    SimpleButton exit_button = makeButton(exit_texture, EXIT_BUTTON_POS,
        [this]() {
            this->m_requests.push_back(StackRequest::QUIT);
        }
    );
    this->m_GUI.addButton(std::move(exit_button));

}
MenuState::~MenuState() = default;
 
std::vector<StackRequest> MenuState::update(sf::Time dt, EventManager &event)
{ 
    this->m_message_timer += dt;
    this->m_click_to_play_message.setColor(
        calcClickMessageColor(this->m_message_timer));

    if (this->m_GUI.respondToEvent(event)) {}
    else if (event.isMouseButtonPressed(sf::Mouse::Left)) {
      this->m_requests.push_back({StackRequest::PUSH, PlayingState::ID});
      this->pause();
    }
    return std::move(this->m_requests);
}

void MenuState::render() const
{
    this->getWindow().clear(BACKGROUND_COLOR);
    this->getWindow().draw(this->m_title);
    this->getWindow().draw(this->m_click_to_play_message);
    this->m_GUI.drawTo(this->getWindow());
}

void MenuState::asTopState()
{
    this->resume();
    this->getWindow().setMouseCursor(*this->m_cursor);
    this->getWindow().setMouseCursorVisible(true);
    this->m_message_timer = sf::Time::Zero;
}


sf::Color calcClickMessageColor(sf::Time time)
{
    sf::Color color = CLICK_COLOR;
    color.a = 255 * (0.4 + 0.2 * cos(time.asSeconds() * 2));
    return color;
}

} // namespace LaserWave