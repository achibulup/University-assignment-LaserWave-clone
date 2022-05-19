#ifndef PARTICLE_HPP_INCLUDED
#define PARTICLE_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace LaserWave
{

class Particle : public sf::Drawable
{
  public:
    virtual ~Particle() = default;

    virtual void update(sf::Time dt) = 0;

    bool isExpired() const noexcept
    {
        return this->m_expired;
    }

  protected:
    bool m_expired = false;
};

} // namespace LaserWave

#endif // PARTICLE_HPP_INCLUDED