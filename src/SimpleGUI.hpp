#ifndef SIMPLEGUI_HPP_INCLUDED
#define SIMPLEGUI_HPP_INCLUDED

#include "SimpleButton.hpp"
#include "EventManager.hpp"
#include <vector>
#include <SFML/Achibulup_dependencies/common_utils.hpp>

namespace LaserWave
{

class SimpleGUI
{
  public:
    SimpleGUI() = default;
    SimpleGUI(const SimpleGUI&) = delete;

    SimpleGUI& addButton(SimpleButton button)
    {
        this->do_addButton(std::move(button));
        return *this;
    }

    // return: the number of responses
    int respondToEvent(EventManager &event, 
                       bool break_on_first_response = false);

    void drawTo(sf::RenderWindow &window, sf::RenderStates = {}) const;

  private:
    void do_addButton(SimpleButton &&button);

    List<SimpleButton> m_buttons;
};



template<typename Component, 
         typename = decltype(std::declval<const Component&>().getSize())>
sf::Vector2f componentSize(const Component &component)
{
    return sf::Vector2f(component.getSize());
}
template<typename Component, 
         typename = decltype(std::declval<const Component&>().getGlobalBounds()),
         typename = void>
sf::Vector2f componentSize(const Component &component)
{
    auto rect = component.getGlobalBounds();
    return {rect.width, rect.height};
}

template<typename Component>
int getCenterX(sf::Window &window, const Component &component)
{
    return window.getSize().x / 2 - componentSize(component).x / 2;
}
template<typename Component>
int getCenterY(sf::Window &window, const Component &component)
{
    return window.getSize().y / 2 - componentSize(component).y / 2;
}

} // namespace LaserWave

#endif // SIMPLEGUI_HPP_INCLUDED