#ifndef EVENTMANAGER_HPP_INCLUDED
#define EVENTMANAGER_HPP_INCLUDED

#include "commons.hpp"
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

    List<sf::Event> getPendingEvents() const;
    List<sf::Event::MouseButtonEvent> getMouseClicks() const;
    bool isMouseButtonPressed(sf::Mouse::Button button) const;
    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isCloseRequested() const;
    bool hasLostFocus() const;
  
  private:
    sf::Window *m_window;
    List<sf::Event> m_events;
    struct Cache
    {
        bool closed = false;
        bool lostFocus = false;
    }m_cache;
};

}

#endif // EVENTMANAGER_HPP_INCLUDED