#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Entity.hpp"
#include "constants.hpp"

namespace LaserWave
{

class Player : public Entity INIT_DEBUG_ID(Player)
{
    using HitboxShape = StaticHitboxConvex<4>;

  public:
    using Hitbox = BasicHitboxRef<HitboxShape>;

    static const Id ID;

    Player(Point center);

    Entity::Id getId() const noexcept override { return ID; }
    
    Point getCenter() const override;

    Hitbox getHitbox() const;
    sf::Color getColor() const noexcept override;

    void update(sf::Time dt) override;

    bool isInvincible() const;
    int health() const;

    void shoot(Vec2 direction);
    void kick(Vec2 direction);
    void getHit(Vec2 direction);

    void setVelocity(Vec2 velocity);

  protected:
    Entity::Hitbox v_getHitbox() const override;

    void draw(sf::RenderTarget&, sf::RenderStates = {}) const override;

  private:
    void applyAcceleration(sf::Time dt);
    float calcDrag() const;

    BasicHitbox<HitboxShape> m_hitbox;
    Vec2 m_velocity = {};
    int m_health = PLAYER_MAX_HEALTH;
    sf::Time m_invincibility_timer = sf::Time::Zero;
};

static_assert(!std::is_abstract_v<Player>, "Player shall not be abstract");

} // namespace LaserWave

#endif // PLAYER_HPP_INCLUDED