#ifndef HEALTHBAR_HPP_INCLUDED
#define HEALTHBAR_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace LaserWave
{

class HealthBar : public sf::Drawable
{
  public:
    HealthBar(int health = 0);

    void setHealth(int health)
    {
        this->m_health = health;
    }
    int health() const
    {
        return this->m_health;
    }

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates) const override;

    int m_health;
};

} // namespace LaserWave

#endif // HEALTHBAR_HPP_INCLUDED
