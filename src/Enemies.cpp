#include "Enemies.hpp"
#include "BasicEnemy.hpp"
#include "constants.hpp"
#include "GameMaths.hpp"

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
    sf::Vector2f velocity = direction * BASIC_ENEMY_SPEED;
    this->addEnemy(std::make_unique<BasicEnemy>(position, velocity));
}

void Enemies::filterDeadEnemies()
{
    for (int i = this->m_enemies.size(); i --> 0;)
      if (!this->m_enemies[i]->isAlive())
        this->m_enemies.erase(this->m_enemies.begin() + i);
}

void Enemies::addEnemy(std::unique_ptr<Enemy> enemy)
{
    this->m_enemies.push_back(std::move(enemy));
}

} // namespace LaserWave