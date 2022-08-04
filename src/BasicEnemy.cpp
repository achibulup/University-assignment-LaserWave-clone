#include "BasicEnemy.hpp"
#include "GameMaths.hpp"
#include "constants.hpp"
#include "randoms.hpp"
#include "draw.hpp"

namespace LaserWave
{

const Entity::Id BasicEnemy::ID("BasicEnemy");

BasicEnemy::BasicEnemy(sf::Vector2f center, sf::Vector2f velocity)
: m_hitbox(3, center, BASIC_ENEMY_RADIUS, Angle::fromDegrees(randInt(0, 359))),
  m_rotation_speed(Angle::fromDegrees(randSign()
      * randFloat(BASIC_ENEMY_MIN_ROTATION_SPEED.asDegrees(),
                  BASIC_ENEMY_MAX_ROTATION_SPEED.asDegrees()))),
  m_velocity(velocity) {}

sf::Vector2f BasicEnemy::getCenter() const noexcept
{
    return this->m_hitbox.getCenter();
}
auto BasicEnemy::getHitbox() const -> Hitbox
{
    return this->m_hitbox;
}
auto BasicEnemy::v_getHitbox() const noexcept -> Entity::Hitbox
{
    return this->getHitbox();
}

sf::Color BasicEnemy::getSignatureColor() const noexcept
{
    return BASIC_ENEMY_COLOR;
} 
sf::Color BasicEnemy::getColor() const noexcept
{
    if (this->gotHit())
      return ENEMY_HIT_COLOR;
    return BASIC_ENEMY_COLOR;
} 

void BasicEnemy::update(sf::Time dt)
{
    if (this->gotHit()) {
      this->m_freeze_time -= dt;
      if (this->m_freeze_time <= sf::Time::Zero)
        kill(*this);
    }
    else {
      this->m_hitbox.translate(dt.asSeconds() * this->m_velocity);
      this->m_hitbox.rotate(dt.asSeconds() * this->m_rotation_speed);
      this->m_hitbox.updateBoundingBox();
    }
}

void BasicEnemy::draw(sf::RenderTarget &tar, sf::RenderStates state) const
{
    LaserWave::draw(tar, this->getHitbox().getShape(), 
                    this->getColor(), state);
}

void BasicEnemy::getHit(sf::Vector2f)
{
    if (this->gotHit()) return;
    this->m_freeze_time = sf::seconds(ENEMY_FREEZE_TIME);
}

bool BasicEnemy::gotHit() const
{
    return this->m_freeze_time > sf::Time::Zero;
}

} // namespace LaserWave