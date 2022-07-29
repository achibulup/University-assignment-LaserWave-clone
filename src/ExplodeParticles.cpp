#include "ExplodeParticles.hpp"
#include "Hitbox.hpp"
#include "randoms.hpp"

namespace LaserWave
{

void ExplodeParticle::onUpdate(sf::Time dt)
{
    this->center += this->velocity * dt.asSeconds();
    this->rotation += this->angle * dt.asSeconds();
    this->velocity.y += EXPLODE_PARTICLE_GRAVITY * dt.asSeconds();
}

static sf::Vertex triangle[3];

void ExplodeParticle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Color color = this->color;
    color.a = 255 * 0.5f * this->getTimeLeft().asSeconds() / AVERAGE_EXPLODE_PARTICLE_LIFETIME;
    RegularHitboxConvex tri(3, this->center, this->radius, this->angle);
    for (int i = 0; i < 3; i++) {
      triangle[i].position = tri.getVertex(i);
      triangle[i].color = color;
    }
    target.draw(triangle, 3, sf::Triangles, states);
}

List<Unique<ExplodeParticle>> explode(Vec2 center, sf::Color color, 
    float average_particle_size, float average_radius, float average_lifetime,
    float estimate_particle_count)
{
    int particle_count = estimate_particle_count * randFloat(0.75, 1.3);
    float average_speed = average_radius / average_lifetime;

    List<Unique<ExplodeParticle>> particles;
    particles.reserve(particle_count);
    for (int i = 0; i < particle_count; i++) {
        float lifetime = average_lifetime * randFloat(0.9f, 1.1f);
        float size = average_particle_size * randFloat(0.95f, 1.05f);
        float speed = average_speed * randFloat(0.5f, 1.15f);
        Angle angle = Angle::fromDegrees(randFloat(0, 360));
        Angle rotation = Angle::fromDegrees(randFloat(-10, 10));
        Angle direction = Angle::fromDegrees(randFloat(0, 360));
        Vec2 velocity = toDirection(direction) * speed 
                      + EXPLODE_VELOCITY_ADJUSTMENT;
        Point pos = center + velocity * randFloat(0.f, 0.2f);

        auto particle = makeUnique<ExplodeParticle>(sf::seconds(lifetime));
        particle->center = pos;
        particle->radius = size;
        particle->velocity = velocity;
        particle->angle = angle;
        particle->rotation = rotation;
        particle->color = color;

        particles.push_back(std::move(particle));
    }
    return particles;
}

} // namespace LaserWave