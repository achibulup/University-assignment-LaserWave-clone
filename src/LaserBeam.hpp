#ifndef LASERBEAM_HPP_INCLUDED
#define LASERBEAM_HPP_INCLUDED

#include "Particle.hpp"
#include "gamemaths.hpp"
#include "constants.hpp"

namespace LaserWave
{

class LaserBeam : public Particle
{
  public:
    LaserBeam(sf::Vector2f start, sf::Vector2f end);

  private:
    void onUpdate(sf::Time dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates) const override;

    sf::Vector2f m_start;
    Angle m_angle;
    float m_length;
};

} // namespace LaserWave

#endif // LASERBEAM_HPP_INCLUDED