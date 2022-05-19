#include "GameClock.hpp"
#include "constants.hpp"
#include <SFML/Achibulup_dependencies/common_utils.h>

namespace LaserWave
{

GameClock::GameClock(sf::Vector2f position, const sf::Font *font)
: m_font(font), m_position(position) {}

void GameClock::add1Tick()
{
    if (this->m_milliseconds % 10 == 0)
      this->m_milliseconds += 16;
    else this->m_milliseconds += 17;
}

void GameClock::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Text display(this->toDisplayString(), *this->m_font, 
                     GAMECLOCK_FONT_SIZE);
    display.setPosition(this->m_position.x - display.getGlobalBounds().width / 2.f,
                        this->m_position.y);
    display.setColor(GAMECLOCK_COLOR);
    target.draw(display, states);
}

sf::String GameClock::toDisplayString() const
{
    int tmp = this->m_milliseconds / 10;
    int centiseconds = tmp % 100;
    tmp /= 100;
    int seconds = tmp % 60;
    tmp /= 60;
    int minutes = tmp % 60;
    char result[9] = {
      (minutes / 10) + '0', (minutes % 10) + '0',
      ':',
      (seconds / 10) + '0', (seconds % 10) + '0',
      '.',
      (centiseconds / 10) + '0', (centiseconds % 10) + '0'
    };
    return result;
}

} // namespace LaserWave
