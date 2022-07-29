#ifndef HEALTHBAR_HPP_INCLUDED
#define HEALTHBAR_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace LaserWave
{

class HealthBar : public sf::Drawable
{
  public:
    HealthBar(sf::Color color = sf::Color::Red, int health = 0);

    int health() const
    {
        return this->m_health;
    }
    void setColor(sf::Color color)
    {
        this->m_color = color;
    }
    void setHealth(int health)
    {
        this->m_health = health;
    }

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates) const override;

    sf::Color m_color;
    int m_health;
};

} // namespace LaserWave

#endif // HEALTHBAR_HPP_INCLUDED
