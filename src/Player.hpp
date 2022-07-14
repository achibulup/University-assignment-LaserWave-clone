#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Entity.hpp"
#include "draw_convex.hpp"
#include "constants.hpp"

namespace LaserWave
{

class Player : public Entity INIT_DEBUG_ID(Player)
{
    using HitboxShape = StaticHitboxConvex<4>;

  public:
    using Hitbox = BasicHitboxRef<HitboxShape>;

    static const Id ID;

    Player(sf::Vector2f center);

    Entity::Id getId() const noexcept override { return ID; }
    
    sf::Vector2f getCenter() const override;

    Hitbox getHitbox() const;
    sf::Color getColor() const noexcept;

    void update(sf::Time dt) override;

    bool isInvincible() const;
    int health() const;

    void shoot(sf::Vector2f direction);
    void kick(sf::Vector2f direction);
    void getHit(sf::Vector2f direction);

    void setVelocity(sf::Vector2f velocity);

  protected:
    Entity::Hitbox v_getHitbox() const override;

    void draw(sf::RenderTarget&, sf::RenderStates = {}) const override;

  private:
    void applyAcceleration(sf::Time dt);
    float calcDrag() const;

    BasicHitbox<HitboxShape> m_hitbox;
    sf::Vector2f m_velocity = {};
    int m_health = PLAYER_MAX_HEALTH;
    sf::Time m_invincibility_timer = sf::Time::Zero;
};

static_assert(!std::is_abstract_v<Player>, "Player shall not be abstract");

} // namespace LaserWave

#endif // PLAYER_HPP_INCLUDED