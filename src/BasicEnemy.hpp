#ifndef BASIC_ENEMY_HPP_INCLUDED
#define BASIC_ENEMY_HPP_INCLUDED

#include "Enemy.hpp"
#include "GameMaths.hpp"
#include "ConvexEntity.hpp"

namespace LaserWave
{

class BasicEnemy : public Enemy, public ConvexEntity
{
  public:
    using Hitbox = SymetricHitboxConvex<6>;

    BasicEnemy(sf::Vector2f center, sf::Vector2f velocity = {});

    sf::Vector2f getCenter() const override;
    const Hitbox& getHitbox() const override;

    sf::Color getColor() const noexcept override;

    void update(sf::Time dt) override;
    void getHit(sf::Vector2f direction = {}) override;
    bool gotHit() const;

  protected:
    void draw(sf::RenderTarget &tar, sf::RenderStates state = {}) const override
    {
        return this->ConvexEntity::draw(tar, state);
    }

  private:
    Hitbox m_hitbox;
    Angle m_rotation_speed;
    sf::Vector2f m_velocity = {};
    sf::Time m_freeze_time = sf::Time::Zero;
};

static_assert(!std::is_abstract_v<BasicEnemy>, "BasicEnemy shall not be abstract");

} // namespace LaserWave

#endif // BASIC_ENEMY_HPP_INCLUDED