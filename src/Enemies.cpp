#include "Enemies.hpp"
#include "BasicEnemy.hpp"
#include "constants.hpp"
#include "GameMaths.hpp"
#include "randoms.hpp"

#include <algorithm>

namespace LaserWave
{

Enemies::Enemies() = default;

int Enemies::getEnemyCount() const
{
    return this->m_enemies.size();
}

Enemy& Enemies::getEnemy(int index)
{
    return *this->m_enemies[index];
}
const Enemy& Enemies::getEnemy(int index) const
{
    return *this->m_enemies[index];
}

void Enemies::update(sf::Time dt)
{
    for (auto &enemy : *this)
      enemy.update(dt);
    this->filterDeadEnemies();
}

void Enemies::drawTo(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const auto &enemy : *this)
      target.draw(enemy, states);
}

void Enemies::addRandomEnemy(sf::Vector2f position, sf::Vector2f player_pos)
{
    sf::Vector2f direction = normalize(player_pos - position);
    Angle rand_angle = toAngle(direction) + Angle::fromDegrees(randInt(-10, 10));
    sf::Vector2f velocity = toDirection(rand_angle) * BASIC_ENEMY_SPEED;
    this->addEnemy(makeUnique<BasicEnemy>(position, velocity));
}

void Enemies::filterDeadEnemies()
{
    removeIf(this->m_enemies, [](const Unique<Enemy> &enemy) 
        { 
            return !enemy->isAlive(); 
        });
}

void Enemies::addEnemy(Unique<Enemy> enemy)
{
    this->m_enemies.push_back(std::move(enemy));
}

} // namespace LaserWave