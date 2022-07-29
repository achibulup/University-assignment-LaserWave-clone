#include "GameClock.hpp"
#include "constants.hpp"
#include "debug_utils.hpp"
#include <SFML/Achibulup_dependencies/common_utils.hpp>

extern std::ofstream log_file;

namespace LaserWave
{

GameClock::GameClock(sf::Vector2f position, const sf::Font *font)
: m_font(font), m_position(position) {}

void GameClock::update(sf::Time dt)
{
    this->m_timer += dt;
}

void GameClock::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Text display(this->toString(), *this->m_font, GAMECLOCK_FONT_SIZE);
    display.setPosition(this->m_position.x - display.getGlobalBounds().width / 2.f,
                        this->m_position.y);
    display.setFillColor(GAMECLOCK_COLOR);
    target.draw(display, states);
}

std::string GameClock::toString() const
{
    int tmp = this->m_timer.asMicroseconds() / 10000;
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
