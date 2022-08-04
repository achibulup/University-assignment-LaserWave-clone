#include <cmath>
#include "Player.hpp"
#include "GameMaths.hpp"
#include "randoms.hpp"
#include "draw.hpp"

namespace LaserWave
{

const Entity::Id Player::ID("Player");

Player::Player(Vec2 center)
: m_hitbox(center)
{
    m_hitbox.setLocalVertex(0, {0.f, -PLAYER_SIZE.y / 2});
    m_hitbox.setLocalVertex(1, {PLAYER_SIZE.x / 2, 0.f});
    m_hitbox.setLocalVertex(2, {0.f, PLAYER_SIZE.y / 2});
    m_hitbox.setLocalVertex(3, {-PLAYER_SIZE.x / 2, 0.f});
}

Point Player::getCenter() const noexcept
{
    return this->m_hitbox.getCenter();
}

auto Player::getHitbox() const noexcept -> Hitbox
{
    return this->m_hitbox;
}
auto Player::v_getHitbox() const noexcept -> Entity::Hitbox
{
    return this->getHitbox();
}

auto Player::getSignatureColor() const noexcept -> sf::Color 
{
    return PLAYER_COLOR;
}
auto Player::getColor() const noexcept -> sf::Color 
{
    float invincibility_interpolation = this->m_invincibility_timer.asSeconds()
                                      / PLAYER_INVINCIBILITY_DURATION;
    float color_interpolation = sqrt(std::max(invincibility_interpolation, 0.f));
    return lerp(PLAYER_COLOR, PLAYER_INVINCIBLE_COLOR, 0, 1, color_interpolation);
}

void Player::update(sf::Time dt)
{
    this->m_hitbox.translate(dt.asSeconds() * this->m_velocity);
    this->applyAcceleration(dt);
    this->m_hitbox.updateBoundingBox();
    if (this->isInvincible()) {
      this->m_invincibility_timer 
          = std::max(this->m_invincibility_timer - dt, sf::Time::Zero);
    }
}


void Player::draw(sf::RenderTarget &tar, sf::RenderStates state) const
{
    return LaserWave::draw(tar, this->m_hitbox, this->getColor(), state);
}

bool Player::isInvincible() const
{
    return this->m_invincibility_timer > sf::Time::Zero;
}
int Player::health() const
{
    return this->m_health;
}

void Player::shoot(Vec2 direction)
{
    direction = -direction;
    // this->m_velocity = direction * PLAYER_SHOOT_SPEED;

    auto perp = perpendicular(direction);
    auto old_velocity = this->m_velocity;
    float proj_len = old_velocity.x * direction.x + old_velocity.y * direction.y;
    float perp_len = old_velocity.x * perp.x + old_velocity.y * perp.y;
    float proj_velocity = std::max(proj_len - PLAYER_SHOOT_SPEED, PLAYER_SHOOT_SPEED);
    float perp_velocity = (perp_len < 0) ? 
        std::min(0.f, perp_len + PLAYER_SHOOT_SPEED * 2)
      : std::max(0.f, perp_len - PLAYER_SHOOT_SPEED * 2);
    this->m_velocity = proj_velocity * direction + perp_velocity * perp;
}

void Player::kick(Vec2 direction)
{
    this->setVelocity(-direction * PLAYER_SPEED);
}

void Player::getHit(Vec2 direction)
{
    if (direction == Vec2()) direction = toDirection(
      Angle::fromDegrees(randFloat(0, 360)));
    if (this->isAlive() && !this->isInvincible()) {
      --this->m_health;
      if (this->m_health == 0)
        kill(*this);
      else
        this->m_invincibility_timer = sf::seconds(PLAYER_INVINCIBILITY_DURATION);
    }
    if (this->isAlive())
      this->setVelocity(direction * PLAYER_HIT_SPEED);
}

void Player::setVelocity(Vec2 velocity)
{
    this->m_velocity = velocity;
}

void Player::applyAcceleration(sf::Time dt)
{
    float speed = abs(this->m_velocity);
    float acc = this->calcDrag();
    if (speed < dt.asSeconds() * acc)
      this->m_velocity = {};
    else if (speed != 0)
      this->m_velocity -= dt.asSeconds() * acc * normalize(this->m_velocity);
    if (this->m_velocity.y < PLAYER_MAX_FALL)
      this->m_velocity.y += dt.asSeconds() * GRAVITY;
}

float Player::calcDrag() const
{
  //TODO
    float acc; 
    float speed = abs(this->m_velocity);
    if (speed < 20) acc = 30;
    else if (speed < 50.f) acc = 60;
    else acc = PLAYER_MIN_DRAG;
    return acc;
}

}