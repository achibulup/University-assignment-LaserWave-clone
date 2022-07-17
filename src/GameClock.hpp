#ifndef GAMECLOCK_HPP_INCLUDED
#define GAMECLOCK_HPP_INCLUDED

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

namespace LaserWave
{

class GameClock : public sf::Drawable
{
  public:
    GameClock(sf::Vector2f position, const sf::Font *font);

    void draw(sf::RenderTarget &target, sf::RenderStates) const override;

    /// add the timer by 1 frame (1/60 seconds)
    void add1Tick();
    
    int asMilliseconds() const;

    std::string toString() const;

  private:
    int m_milliseconds = 0;
    const sf::Font *m_font;
    sf::Vector2f m_position;
};

} // namespace LaserWave

#endif // GAMECLOCK_HPP_INCLUDED