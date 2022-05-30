#include "BasicEnemy.hpp"
#include "GameMaths.hpp"
#include "constants.hpp"
#include "randoms.hpp"

namespace LaserWave
{

BasicEnemy::BasicEnemy(sf::Vector2f center, sf::Vector2f velocity)
: m_hitbox(center, BASIC_ENEMY_RADIUS, Angle::fromDegrees(randInt(0, 359))),
  m_rotation_speed(Angle::fromDegrees(randSign()
      * randFloat(BASIC_ENEMY_MIN_ROTATION_SPEED.asDegrees(),
                  BASIC_ENEMY_MAX_ROTATION_SPEED.asDegrees()))),
  m_velocity(velocity) {}

sf::Vector2f BasicEnemy::getCenter() const
{
    return this->m_hitbox.getCenter();
}
const BasicEnemy::Hitbox& BasicEnemy::getHitbox() const
{
    return this->m_hitbox;
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
    }
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