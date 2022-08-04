#ifndef HEALTHBAR_HPP_INCLUDED
#define HEALTHBAR_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace LaserWave
{


class HealthBar : public sf::Drawable
{
  public:
    using Point = sf::Vector2f;
    enum class Direction
    {
        LEFT,
        RIGHT
    };

    HealthBar(Point pos, Direction = Direction::RIGHT, 
              sf::Color color = sf::Color::Red, int health = 0);

    Point getPosition() const
    {
        return this->m_pos;
    }
    int getHealth() const
    {
        return this->m_health;
    }
    void setPosition(Point pos)
    {
        this->m_pos = pos;
    }
    void setColor(sf::Color color)
    {
        this->m_color = color;
    }
    void setDirection(Direction direction)
    {
        this->m_direction = direction;
    }
    void setHealth(int health)
    {
        this->m_health = health;
    }

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates) const override;

    Point m_pos;
    sf::Color m_color;
    Direction m_direction;
    int m_health;
};

} // namespace LaserWave

#endif // HEALTHBAR_HPP_INCLUDED
