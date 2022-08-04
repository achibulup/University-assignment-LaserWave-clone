#include "HealthBar.hpp"
#include "constants.hpp"
#include "draw.hpp"

namespace LaserWave
{

HealthBar::HealthBar(Point pos, Direction dir, sf::Color color, int health) 
: m_pos(pos), m_color(color), m_direction(dir), m_health(health) {}

void HealthBar::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    /// draw m_health rectangles
    Rect rectangle(HEALTH_BAR_UNIT_SIZE);
    rectangle.setPosition(this->m_pos);
    rectangle.setFillColor(this->m_color);
    if (this->m_direction == Direction::LEFT) {
      states.transform.scale({-1, 1}, this->m_pos);
    }
    for (int i = 0; i < m_health; ++i) {
      target.draw(rectangle, states);
      rectangle.move(HEALTH_BAR_UNIT_SIZE.x + HEALTH_BAR_UNIT_SPACING, 0);
    }
}

} // namespace LaserWave