#include "BasicEnemy.hpp"
#include "GameMaths.hpp"
#include "constants.hpp"
#include "randoms.hpp"

namespace LaserWave
{

BasicEnemy::BasicEnemy(sf::Vector2f center, sf::Vector2f velocity)
: m_pos(center, BASIC_ENEMY_SIZE), 
  m_rotation(Angle::fromDegrees(rand() % 360)),
  m_rotation_speed(Angle::fromDegrees(randSign()
      * randFloat(BASIC_ENEMY_MIN_ROTATION_SPEED.asDegrees(),
                  BASIC_ENEMY_MAX_ROTATION_SPEED.asDegrees()))),
  m_velocity(velocity) {}

sf::Vector2f BasicEnemy::getCenter() const
{
    return this->m_pos.getCenter();
}
sf::FloatRect BasicEnemy::getHitbox() const
{
    return this->m_pos.getHitbox();
}

void BasicEnemy::update(sf::Time dt)
{
    if (this->gotHit()) {
      this->m_freeze_time -= dt;
      if (this->m_freeze_time <= sf::Time::Zero)
        kill(*this);
    }
    else {
      this->m_pos.translate(dt.asSeconds() * this->m_velocity);
      this->m_rotation += dt.asSeconds() * this->m_rotation_speed;
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

void BasicEnemy::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    /// draw a hexagon shape
    const float DIST_FROM_CENTER = BASIC_ENEMY_SIZE.x / 1.4f;
    sf::Vector2f points[6] = {
        this->m_pos.getCenter() + DIST_FROM_CENTER * toDirection(this->m_rotation + Angle::fromDegrees(0)),
        this->m_pos.getCenter() + DIST_FROM_CENTER * toDirection(this->m_rotation + Angle::fromDegrees(60)),
        this->m_pos.getCenter() + DIST_FROM_CENTER * toDirection(this->m_rotation + Angle::fromDegrees(120)),
        this->m_pos.getCenter() + DIST_FROM_CENTER * toDirection(this->m_rotation + Angle::fromDegrees(180)),
        this->m_pos.getCenter() + DIST_FROM_CENTER * toDirection(this->m_rotation + Angle::fromDegrees(240)),
        this->m_pos.getCenter() + DIST_FROM_CENTER * toDirection(this->m_rotation + Angle::fromDegrees(300))
    };

    sf::ConvexShape hexagon(6);
    hexagon.setPoint(0, points[0]); hexagon.setPoint(1, points[1]);
    hexagon.setPoint(2, points[2]); hexagon.setPoint(3, points[3]);
    hexagon.setPoint(4, points[4]); hexagon.setPoint(5, points[5]);
    if (this->gotHit())
      hexagon.setFillColor(ENEMY_HIT_COLOR);
    else
      hexagon.setFillColor(BASIC_ENEMY_COLOR);    
    target.draw(hexagon, states);
}

} // namespace LaserWave