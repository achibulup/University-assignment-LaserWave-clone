#ifndef BASIC_ENEMY_HPP_INCLUDED
#define BASIC_ENEMY_HPP_INCLUDED

#include "Enemy.hpp"
#include "GameMaths.hpp"

namespace LaserWave
{

class BasicEnemy : public Enemy
{
  public:
    BasicEnemy(sf::Vector2f center, sf::Vector2f velocity = {});

    sf::Vector2f getCenter() const override;
    sf::FloatRect getHitbox() const override;

    void update(sf::Time dt) override;
    void getHit(sf::Vector2f direction = {}) override;
    bool gotHit() const;

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates = {}) const override;
    
    EntityPosition m_pos;
    Angle m_rotation;
    Angle m_rotation_speed;
    sf::Vector2f m_velocity = {};
    sf::Time m_freeze_time = sf::Time::Zero;
};

} // namespace LaserWave

#endif // BASIC_ENEMY_HPP_INCLUDED