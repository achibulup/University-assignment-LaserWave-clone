#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Entity.hpp"
#include "ConvexEntity.hpp"
#include "AssetManager.hpp"
#include "constants.hpp"

namespace LaserWave
{

class Player : public Entity, public ConvexEntity
{
  public:
    using Hitbox = HitboxConvex<4>;

    Player(sf::Vector2f center);
    
    sf::Vector2f getCenter() const override;
    const Hitbox& getHitbox() const override;

    sf::Color getColor() const noexcept override;

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

  protected:
    void draw(sf::RenderTarget &tar, sf::RenderStates state = {}) const override
    {
        return this->ConvexEntity::draw(tar, state);
    }

  private:
    void applyAcceleration(sf::Time dt);
    float calcDrag() const;

    Hitbox m_hitbox;
    sf::Vector2f m_velocity = {};
    int m_health = PLAYER_MAX_HEALTH;
    sf::Time m_invincibility_timer = sf::Time::Zero;
};

static_assert(!std::is_abstract_v<Player>, "Player shall not be abstract");

} // namespace LaserWave

#endif // PLAYER_HPP_INCLUDED