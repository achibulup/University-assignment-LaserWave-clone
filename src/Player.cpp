#include <cmath>
#include "Player.hpp"
#include "GameMaths.hpp"

namespace LaserWave
{

Player::Player(sf::Vector2f center)
: m_pos(center, PLAYER_SIZE) {}

sf::Vector2f Player::getCenter() const
{
    return this->m_pos.getCenter();
}

sf::FloatRect Player::getHitbox() const
{
    return this->m_pos.getHitbox();
}

void Player::update(sf::Time dt)
{
    this->m_pos.translate(dt.asSeconds() * this->m_velocity);
    this->applyAcceleration(dt);
    if (this->isInvincible()) {
      this->m_invincibility_timer 
          = std::max(this->m_invincibility_timer - dt, sf::Time::Zero);
    }
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    /// draw a diamond shape from two triangles
    sf::Vector2f points[4] = {
        this->m_pos.getCenter() + sf::Vector2f(0, -32.f),
        this->m_pos.getCenter() + sf::Vector2f(20.f, 0),
        this->m_pos.getCenter() + sf::Vector2f(0, 32.f),
        this->m_pos.getCenter() + sf::Vector2f(-20.f, 0)
    };
    sf::ConvexShape shape(4);
    shape.setPoint(0, points[0]);
    shape.setPoint(1, points[1]);
    shape.setPoint(2, points[2]);
    shape.setPoint(3, points[3]);
    
    float invincibility_interpolation = this->m_invincibility_timer.asSeconds()
                                      / PLAYER_INVINCIBILITY_DURATION;
    float color_interpolation = std::cbrt(sqr((invincibility_interpolation)));
    sf::Color color = {
      PLAYER_COLOR.r + (PLAYER_INVINCIBLE_COLOR.r - PLAYER_COLOR.r) * color_interpolation,
      PLAYER_COLOR.g + (PLAYER_INVINCIBLE_COLOR.g - PLAYER_COLOR.g) * color_interpolation,
      PLAYER_COLOR.b + (PLAYER_INVINCIBLE_COLOR.b - PLAYER_COLOR.b) * color_interpolation
    };
    shape.setFillColor(color);
    target.draw(shape, states);
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