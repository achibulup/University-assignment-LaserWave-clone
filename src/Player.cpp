#include <cmath>
#include "Player.hpp"
#include "GameMaths.hpp"

namespace LaserWave
{

Player::Player(sf::Vector2f center)
: m_hitbox(center)
{
    m_hitbox.setLocalVertex(0, {0.f, -PLAYER_SIZE.y / 2});
    m_hitbox.setLocalVertex(1, {PLAYER_SIZE.x / 2, 0.f});
    m_hitbox.setLocalVertex(2, {0.f, PLAYER_SIZE.y / 2});
    m_hitbox.setLocalVertex(3, {-PLAYER_SIZE.x / 2, 0.f});
}

Point Player::getCenter() const
{
    return this->m_hitbox.getCenter();
}

const Player::Hitbox& Player::getHitbox() const
{
    return this->m_hitbox;
}

sf::Color Player::getColor() const noexcept
{
    return PLAYER_COLOR;
}

void Player::update(sf::Time dt)
{
    this->m_hitbox.translate(dt.asSeconds() * this->m_velocity);
    this->applyAcceleration(dt);
    if (this->isInvincible()) {
      this->m_invincibility_timer 
          = std::max(this->m_invincibility_timer - dt, sf::Time::Zero);
    }
}

bool Player::isInvincible() const
{
    return this->m_invincibility_timer > sf::Time::Zero;
}

void Player::shoot(sf::Vector2f direction)
{
    this->m_velocity = -direction * PLAYER_SHOOT_SPEED;
}

void Player::kick(sf::Vector2f direction)
{
    this->setVelocity(-direction * PLAYER_SPEED);
}

void Player::getHit(sf::Vector2f direction)
{
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

void Player::setVelocity(sf::Vector2f velocity)
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