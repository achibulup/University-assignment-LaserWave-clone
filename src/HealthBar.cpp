#include "HealthBar.hpp"
#include "constants.hpp"
#include "draw.hpp"

namespace LaserWave
{

HealthBar::HealthBar(sf::Color color, int health) 
: m_color(color), m_health(health) {}

void HealthBar::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    /// draw m_health rectangles
    Rect rectangle(HEALTH_BAR_UNIT_SIZE);
    rectangle.setPosition(HEALTH_BAR_POSITION);
    rectangle.setFillColor(this->m_color);
    for (int i = 0; i < m_health; ++i) {
      target.draw(rectangle, states);
      rectangle.move(HEALTH_BAR_UNIT_SIZE.x + HEALTH_BAR_UNIT_SPACING, 0);
    }
}

} // namespace LaserWave