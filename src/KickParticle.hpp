#ifndef KICK_PARTICLE_HPP_INCLUDED
#define KICK_PARTICLE_HPP_INCLUDED

#include "Particle.hpp"
#include "GameMaths.hpp"
#include "constants.hpp"

namespace LaserWave
{

class KickParticle : public Particle
{
  public:
    KickParticle(Point center, Angle angle, const sf::Texture *texture);

  private:
    void onUpdate(sf::Time dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates) const override;

    Point m_center;
    Angle m_angle;
    const sf::Texture *m_texture;
};
 
} // namespace LaserWave

#endif // KICK_PARTICLE_HPP_INCLUDED