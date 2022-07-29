#include "KickParticle.hpp"
#include "constants.hpp"

#include <fstream>

extern std::ofstream log_file;

namespace LaserWave
{

KickParticle::KickParticle(sf::Vector2f center, 
    Angle angle, const sf::Texture *texture)
: Particle(sf::seconds(KICK_PARTICLE_LIFETIME)),
  m_center(center), m_angle(angle), m_texture(texture) {}


void KickParticle::onUpdate(sf::Time dt) {}

void KickParticle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Sprite sprite;
    sprite.setTexture(*this->m_texture, true);
    sprite.setOrigin(Vec2(this->m_texture->getSize()) / 2.f);
    sprite.setPosition(this->m_center);
    sprite.setRotation(this->m_angle.asDegrees());
    sprite.setScale({0.75f, 0.75f});

    float interpolation = this->getTimeLeft().asSeconds() / KICK_PARTICLE_LIFETIME;
    float color_interpolation = lerp(1, 0, 0.5, 0, interpolation);
    sf::Color color = sf::Color::White;
    color.a = 255 * color_interpolation;
    sprite.setColor(color);
    
    target.draw(sprite, states);
}

} // namespace LaserWave