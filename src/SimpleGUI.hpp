#ifndef SIMPLEGUI_HPP_INCLUDED
#define SIMPLEGUI_HPP_INCLUDED

#include "SimpleButton.hpp"
#include <vector>
#include "EventManager.hpp"

namespace LaserWave
{

class SimpleGUI
{
  public:
    SimpleGUI() = default;
    SimpleGUI(const SimpleGUI&) = delete;

    void addButton(SimpleButton button);

    // return: the number of responses
    int respondToEvent(EventManager &event, 
                        bool break_on_first_response = false);

    void drawTo(sf::RenderWindow &window, sf::RenderStates = {}) const;

  private:
    std::vector<SimpleButton> m_buttons;
};

} // namespace LaserWave

#endif // SIMPLEGUI_HPP_INCLUDED