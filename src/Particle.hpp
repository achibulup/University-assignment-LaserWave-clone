#ifndef PARTICLE_HPP_INCLUDED
#define PARTICLE_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace LaserWave
{

class Particle : public sf::Drawable
{
  public:
    Particle(sf::Time lifetime) : m_timeLeft(lifetime) {}
    virtual ~Particle() = default;

    void update(sf::Time dt)
    {
        this->m_timeLeft -= dt;
        this->onUpdate(dt);
    }

    bool isExpired() const noexcept
    {
        return this->m_timeLeft <= sf::Time::Zero;
    }

  protected:
    virtual void onUpdate(sf::Time dt) = 0;

    sf::Time getTimeLeft() const noexcept
    {
        return this->m_timeLeft;
    }

    void setTimeLeft(sf::Time timeLeft) noexcept
    {
        this->m_timeLeft = timeLeft;
    }

  private:
    sf::Time m_timeLeft;
};

} // namespace LaserWave

#endif // PARTICLE_HPP_INCLUDED