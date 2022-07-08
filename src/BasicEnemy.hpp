#ifndef BASIC_ENEMY_HPP_INCLUDED
#define BASIC_ENEMY_HPP_INCLUDED

#include "Enemy.hpp"
#include "GameMaths.hpp"
#include "ConvexEntity.hpp"

namespace LaserWave
{

class BasicEnemy : public Enemy INIT_DEBUG_ID(BasicEnemy)
{
    using HitboxShape = RegularHitboxConvex;

  public:
    using Hitbox = BasicHitboxRef<HitboxShape>;

    static const Id ID;

    BasicEnemy(sf::Vector2f center, sf::Vector2f velocity = {});

    Entity::Id getId() const noexcept override { return ID; }

    sf::Vector2f getCenter() const override;

    Hitbox getHitbox() const;
    sf::Color getColor() const noexcept;

    void update(sf::Time dt) override;
    void getHit(sf::Vector2f direction = {}) override;
    bool gotHit() const;

  protected:
    Entity::Hitbox v_getHitbox() const override;

    void draw(sf::RenderTarget&, sf::RenderStates = {}) const override;

  private:
    BasicHitbox<HitboxShape> m_hitbox = BasicHitbox<HitboxShape>(6);
    Angle m_rotation_speed;
    sf::Vector2f m_velocity = {};
    sf::Time m_freeze_time = sf::Time::Zero;
};

static_assert(!std::is_abstract_v<BasicEnemy>, "BasicEnemy shall not be abstract");

} // namespace LaserWave

#endif // BASIC_ENEMY_HPP_INCLUDED