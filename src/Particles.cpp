#include "Particles.hpp"
#include "constants.hpp"
#include "GameMaths.hpp"

namespace LaserWave
{

Particles::Particles() = default;

int Particles::getParticleCount() const
{
    return this->m_particles.size();
}

Particle& Particles::getParticle(int index)
{
    return *this->m_particles[index];
}
const Particle& Particles::getParticle(int index) const
{
    return *this->m_particles[index];
}

void Particles::update(sf::Time dt)
{
    for (auto &particle : *this)
      particle.update(dt);
    this->filterExpiredParticles();
}

void Particles::drawTo(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const auto &particle : *this)
      target.draw(particle, states);
}

void Particles::filterExpiredParticles()
{
    for (int i = this->m_particles.size(); i --> 0;)
      if (this->m_particles[i]->isExpired())
        this->m_particles.erase(this->m_particles.begin() + i);
}

void Particles::addParticle(Unique<Particle> particle)
{
    this->m_particles.push_back(std::move(particle));
}

} // namespace LaserWave