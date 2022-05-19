#ifndef EVENTMANAGER_HPP_INCLUDED
#define EVENTMANAGER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace LaserWave
{


class EventManager
{
  //TODO
  public:
    EventManager();
    ~EventManager();

    EventManager(sf::Window *window);

    std::vector<sf::Event> getPendingEvents() const;
    std::vector<sf::Event::MouseButtonEvent> getMouseClicks() const;
    bool isMouseButtonPressed(sf::Mouse::Button button) const;
    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isCloseRequested() const;
    bool hasLostFocus() const;
  
  private:
    sf::Window *m_window;
    std::vector<sf::Event> m_events;
    struct Cache
    {
        bool closed = false;
        bool lostFocus = false;
    }m_events_cache;
};

}

#endif // EVENTMANAGER_HPP_INCLUDED