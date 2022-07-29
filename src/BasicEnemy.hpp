#ifndef BASIC_ENEMY_HPP_INCLUDED
#define BASIC_ENEMY_HPP_INCLUDED

#include "Enemy.hpp"
#include "GameMaths.hpp"

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

    Point getCenter() const override;

    Hitbox getHitbox() const;
    sf::Color getColor() const noexcept override;

    void update(sf::Time dt) override;
    void getHit(Vec2 direction = {}) override;
    bool gotHit() const;

  protected:
    Entity::Hitbox v_getHitbox() const override;

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