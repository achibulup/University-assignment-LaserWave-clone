#include "KickParticle.hpp"
#include "constants.hpp"

#include <fstream>

extern std::ofstream log_file;

namespace LaserWave
{

KickParticle::KickParticle(sf::Vector2f center, 
    Angle angle, const sf::Texture *texture)
{   
    // log_file << angle.asDegrees() << '\n';
    this->m_sprite.setTexture(*texture, true);
    this->m_sprite.setOrigin(sf::Vector2f(texture->getSize()) / 2.f);
    this->m_sprite.setPosition(center);
    this->m_sprite.setRotation(angle.asDegrees());
    this->m_sprite.setScale({0.75f, 0.75f});
    // log_file << this->m_sprite.getPosition().x << " " << this->m_sprite.getPosition().y << " " << this->m_sprite.getRotation() << std::endl;
}


void KickParticle::update(sf::Time dt)
{
    this->m_timeleft -= dt;
    float interpolation = this->m_timeleft.asSeconds() / KICK_PARTICLE_LIFETIME;
    float color_interpolation = lerp(1, 0, 0.5, 0, interpolation);
    sf::Color color = sf::Color::White;
    color.a = 255 * color_interpolation;
    this->m_sprite.setColor(color);
    if (this->m_timeleft <= sf::Time::Zero)
      this->m_expired = true;
}

void KickParticle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(this->m_sprite, states);
}

} // namespace LaserWave