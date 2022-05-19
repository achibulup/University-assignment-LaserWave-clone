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
    KickParticle(sf::Vector2f center, Angle angle, const sf::Texture *texture);

    void update(sf::Time dt) override;

  private:
    void draw(sf::RenderTarget& target, sf::RenderStates = {}) const override;

    sf::Time m_timeleft = sf::seconds(KICK_PARTICLE_LIFETIME);
    sf::Sprite m_sprite;
};
 
} // namespace LaserWave

#endif // KICK_PARTICLE_HPP_INCLUDED