#ifndef BASIC_ENEMY_HPP_INCLUDED
#define BASIC_ENEMY_HPP_INCLUDED

#include "Enemy.hpp"
#include "gamemaths.hpp"

namespace LaserWave
{

class BasicEnemy : public Enemy INIT_DEBUG_ID(BasicEnemy)
{
    using HitboxShape = RegularHitboxConvex;

  public:
    using Hitbox = BasicHitboxRef<HitboxShape>;

    static const Id ID;

    BasicEnemy(Point center, Vec2 velocity = {});

    Entity::Id getId() const noexcept override { return ID; }

    Point getCenter() const noexcept override;

    Hitbox getHitbox() const;
    sf::Color getSignatureColor() const noexcept final override;
    sf::Color getColor() const noexcept;

    void update(sf::Time dt) override;
    
    void getHit(Vec2 direction = {}) override;
    bool gotHit() const;

  protected:
    Entity::Hitbox v_getHitbox() const noexcept override;

    void draw(sf::RenderTarget&, sf::RenderStates = {}) const override;

  private:
    BasicHitbox<HitboxShape> m_hitbox = BasicHitbox<HitboxShape>(6);
    Angle m_rotation_speed;
    Vec2 m_velocity = {};
    sf::Time m_freeze_time = sf::Time::Zero;
};

static_assert(!std::is_abstract_v<BasicEnemy>, "BasicEnemy shall not be abstract");

} // namespace LaserWave

#endif // BASIC_ENEMY_HPP_INCLUDED