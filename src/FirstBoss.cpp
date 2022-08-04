#include "FirstBoss.hpp"
#include "draw.hpp"
#include "constants.hpp"
#include "randoms.hpp"

namespace LaserWave
{

const Entity::Id FirstBoss::ID ("FirstBoss");

FirstBoss::FirstBoss(Point pos, const Entity *player)
: m_hitbox (
    4, 
    pos, 
    FIRST_BOSS_SIZE, 
    Angle::fromDegrees(45)
  ), 
  m_target(player),
  m_health(4) 
{
    this->jump();
}

auto FirstBoss::getCenter() const noexcept -> Point
{
    return this->m_hitbox.getCenter();
}
auto FirstBoss::getHitbox() const noexcept -> Hitbox
{
    return this->m_hitbox;
}
auto FirstBoss::v_getHitbox() const noexcept -> Entity::Hitbox
{
    return this->getHitbox();
}

auto FirstBoss::getSignatureColor() const noexcept -> sf::Color
{
    return FIRST_BOSS_COLOR;
}
auto FirstBoss::getColor() const noexcept -> sf::Color
{
    if (this->m_hitTimer == sf::Time::Zero || !this->isAlive())
      return this->getSignatureColor();
    return sf::Color::White;
}

void FirstBoss::update(sf::Time dt) 
{
    if (this->m_hitTimer != sf::Time::Zero) {
      this->m_hitTimer -= dt;
      if (this->m_hitTimer < sf::Time::Zero)
        this->m_hitTimer = sf::Time::Zero;
      if (this->m_hitTimer == sf::Time::Zero && this->m_health == 0)
        kill(*this);
    }   
    if (this->isAlive() && this->m_target->isAlive()) {
      this->m_jumpTimer += dt;
      if (decide(this->getJumpProbability())) {
        this->jump();
        this->m_jumpTimer = sf::Time::Zero;
      }
    }
}

void FirstBoss::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    LaserWave::draw(target, this->m_hitbox, this->getColor(), states);
}

void FirstBoss::getHit(Vec2 direction)
{
    if (this->m_health) {
      --this->m_health;
      this->m_jumpTimer = std::max(sf::seconds(0), 
                                   this->m_jumpTimer - sf::seconds(0.3));
      if (this->m_health)
        this->m_hitTimer = sf::seconds(0.15f);
      else 
        this->m_hitTimer = sf::seconds(ENEMY_FREEZE_TIME);
    }
}

auto FirstBoss::getHealth() const noexcept -> int
{
    return this->m_health;
}

void FirstBoss::jump()
{
    Point target = this->m_target->getCenter();
    Point pos = this->getCenter();
    this->m_hitbox.setCenter(target + (pos - target) / 2.5f);
    this->m_hitbox.updateBoundingBox();
}

float FirstBoss::getJumpProbability() const
{
    if (this->m_jumpTimer < sf::seconds(0.3f)) return 0;
    return (this->m_jumpTimer.asSeconds() - 0.3f) * 0.006;
}


} // namespace LaserWave