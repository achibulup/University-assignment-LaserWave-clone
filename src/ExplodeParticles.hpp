#ifndef EXPLODEPARTICLES_HPP_INCLUDED
#define EXPLODEPARTICLES_HPP_INCLUDED

#include "Particle.hpp"
#include "commons.hpp"
#include "gamemaths.hpp"
#include "constants.hpp"

namespace LaserWave
{

class ExplodeParticle : public Particle
{
  public:
    ExplodeParticle(sf::Time lifetime) : Particle(lifetime) {}

    Point center;
    float radius;
    Vec2 velocity;
    Angle angle;
    Angle rotation;
    sf::Color color;

  private:
    void onUpdate(sf::Time dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates) const override;
};

/// a collection of triangle particles created when an enemy is destroyed
List<Unique<ExplodeParticle>> explode(Vec2 center, sf::Color color, 
    float average_particle_size = AVERAGE_EXPLODE_PARTICLE_SIZE,
    float average_radius = AVERAGE_EXPLODE_RADIUS, 
    float average_lifetime = AVERAGE_EXPLODE_PARTICLE_LIFETIME,
    float estimate_particle_count = AVERAGE_EXPLODE_PARTICLE_COUNT);

} // namespace LaserWave

#endif // EXPLODEPARTICLES_HPP_INCLUDED
