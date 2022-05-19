#include "SimpleGUI.hpp"

namespace LaserWave
{

void SimpleGUI::addButton(SimpleButton button)
{
    m_buttons.push_back(std::move(button));
}

int SimpleGUI::respondToEvent(EventManager &event, 
                               bool break_on_first_response)
{
    int responses = 0;
    for (auto &click : event.getMouseClicks())
    for (auto &button : m_buttons) if (click.button == sf::Mouse::Left) {
      if (button.getRect().contains({click.x, click.y})) {
        button.onClick();
        ++responses;
        if (break_on_first_response)
          return responses;
      }
    }
    return responses;
}

void SimpleGUI::drawTo(sf::RenderWindow &window, sf::RenderStates states) const
{
    for (auto &button : m_buttons)
      window.draw(button, states);
}

} // namespace LaserWave