#include "EventManager.hpp"

namespace LaserWave
{

EventManager::EventManager() = default;
EventManager::~EventManager() = default;

EventManager::EventManager(sf::Window *window)
: m_window(window)
{
    if (!window->hasFocus()) {
      this->m_events.push_back({sf::Event::LostFocus});
      this->m_cache.hasFocus = false;
    }
    else {
      this->m_cache.mousePos = sf::Mouse::getPosition();
    }
    sf::Event tmp;
    while (this->m_window->pollEvent(tmp))
      this->m_events.push_back(tmp);
    for (auto &event : this->m_events) {
      switch (event.type) { 
        case sf::Event::Closed :
          this->m_cache.closed = true;
        break;
        case sf::Event::LostFocus :
          this->m_cache.hasFocus = false;
        break;
        case sf::Event::GainedFocus :
          this->m_cache.hasFocus = true;
        break;
        case sf::Event::MouseMoved :
          this->m_cache.mousePos = {event.mouseMove.x, event.mouseMove.y};
        break;
        default : break;
      }
    }
}

List<sf::Event> EventManager::getPendingEvents() const
{
    return this->m_events;
}

List<sf::Event::MouseButtonEvent> EventManager::getMouseClicks() const
{
    List<sf::Event::MouseButtonEvent> clicks;
    for (const auto &event : this->m_events) {
      if (event.type == sf::Event::MouseButtonPressed)
        clicks.push_back(event.mouseButton);
    }
    return clicks;
}

bool EventManager::isMouseButtonPressed(sf::Mouse::Button button) const
{
    for (const auto& event : m_events)
      if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == button)
        return true;
    return false;
}

bool EventManager::isKeyPressed(sf::Keyboard::Key key) const
{
    for (const auto& event : m_events)
      if (event.type == sf::Event::KeyPressed && event.key.code == key)
        return true;
    return false;
}

bool EventManager::isCloseRequested() const
{
    return this->m_cache.closed;
}

bool EventManager::hasFocus() const
{
    return this->m_cache.hasFocus;
}

sf::Vector2i EventManager::getMousePosition() const
{
    return this->m_cache.mousePos;
}

} // namespace LaserWave