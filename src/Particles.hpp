#ifndef PARTICLES_HPP_INCLUDED
#define PARTICLES_HPP_INCLUDED

#include "Particle.hpp"
#include <memory>

namespace LaserWave
{

class Particles
{
  public:
    Particles();
    Particles(Particles&&) = default;
    Particles& operator = (Particles&&) = default;

    int getParticleCount() const;

    Particle& getParticle(int index);
    const Particle& getParticle(int index) const;

    class Iterator
    {
      public:
        Iterator(Particles& Particles, int index) noexcept
        : m_particles(&Particles), m_index(index) {}
        Iterator(const Iterator&) = default;
        Iterator& operator = (const Iterator&) & = default;
        Iterator& operator ++ () &
        {
            ++this->m_index;
            return *this;
        }
        Iterator operator ++ (int) &
        {
            Iterator res(*this);
            ++(*this);
            return res;
        }
        Particle& operator * () const
        {
            return this->m_particles->getParticle(this->m_index);
        }
        Particle* operator -> () const
        {
            return &this->m_particles->getParticle(this->m_index);
        }
        bool operator == (const Iterator& other) const
        {
            return this->m_index == other.m_index;
        }
        bool operator != (const Iterator& other) const
        {
            return !(*this == other);
        }

      private:
        Particles *m_particles;
        int m_index;
    };
    class ConstIterator
    {
      public:
        ConstIterator(const Particles& Particles, int index) noexcept
        : m_particles(&Particles), m_index(index) {}
        ConstIterator(const ConstIterator&) = default;
        ConstIterator& operator = (const ConstIterator&) & = default;
        ConstIterator& operator ++ () &
        {
            ++this->m_index;
            return *this;
        }
        ConstIterator operator ++ (int) &
        {
            ConstIterator res(*this);
            ++(*this);
            return res;
        }
        const Particle& operator * () const
        {
            return this->m_particles->getParticle(this->m_index);
        }
        const Particle* operator -> () const
        {
            return &this->m_particles->getParticle(this->m_index);
        }
        bool operator == (const ConstIterator& other) const
        {
            return this->m_index == other.m_index;
        }
        bool operator != (const ConstIterator& other) const
        {
            return !(*this == other);
        }

      private:
        const Particles *m_particles;
        int m_index;
    };

    Iterator begin() &
    {
        return Iterator(*this, 0);
    }
    Iterator end() &
    {
        return Iterator(*this, this->m_particles.size());
    }
    ConstIterator begin() const &
    {
        return ConstIterator(*this, 0);
    }
    ConstIterator end() const &
    {
        return ConstIterator(*this, this->m_particles.size());
    }

    void update(sf::Time dt);
    void drawTo(sf::RenderTarget &target, sf::RenderStates = {}) const;

    void addParticle(std::unique_ptr<Particle> particle);

    void filterExpiredParticles();

  private:
    std::vector<std::unique_ptr<Particle>> m_particles;
};

} // namespace LaserWave

#endif // PARTICLES_HPP_INCLUDED