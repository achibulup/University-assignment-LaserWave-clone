#include "EventManager.hpp"

namespace LaserWave
{

EventManager::EventManager() = default;
EventManager::~EventManager() = default;

EventManager::EventManager(sf::Window *window)
: m_window(window)
{
    sf::Event tmp;
    while (this->m_window->pollEvent(tmp))
      this->m_events.push_back(tmp);
    for (auto &event : this->m_events) {
      if (event.type == sf::Event::Closed)
        this->m_events_cache.closed = true;
      if (event.type == sf::Event::LostFocus)
        this->m_events_cache.lostFocus = true;
    }
}

std::vector<sf::Event> EventManager::getPendingEvents() const
{
    return this->m_events;
}

std::vector<sf::Event::MouseButtonEvent> EventManager::getMouseClicks() const
{
    std::vector<sf::Event::MouseButtonEvent> clicks;
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
    return this->m_events_cache.closed;
}

bool EventManager::hasLostFocus() const
{
    return this->m_events_cache.lostFocus;
}

} // namespace LaserWave