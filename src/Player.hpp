#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Entity.hpp"
#include "AssetManager.hpp"
#include "constants.hpp"

namespace LaserWave
{

class Player : public Entity
{
  public:
    Player(sf::Vector2f center);
    
    sf::Vector2f getCenter() const override;
    sf::FloatRect getHitbox() const override;

    void update(sf::Time dt) override;

    bool isInvincible() const;
    int health() const
    {
        return this->m_health;
    }

    void shoot(sf::Vector2f direction);
    void kick(sf::Vector2f direction);
    void getHit(sf::Vector2f direction);

    void setVelocity(sf::Vector2f velocity);

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates = {}) const override;
    
    void applyAcceleration(sf::Time dt);
    float calcDrag() const;

    EntityPosition m_pos;
    sf::Vector2f m_velocity = {};
    int m_health = PLAYER_MAX_HEALTH;
    sf::Time m_invincibility_timer = sf::Time::Zero;
};

} // namespace LaserWave

#endif // PLAYER_HPP_INCLUDED