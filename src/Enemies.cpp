#include "Enemies.hpp"
#include "constants.hpp"
#include "GameMaths.hpp"
#include "randoms.hpp"

#include <algorithm>

namespace LaserWave
{

Enemies::Enemies() = default;

int Enemies::getEnemyCount() const noexcept
{
    return this->m_enemies.size() + !!this->m_boss;
}

Enemy& Enemies::getEnemy(int index)
{
    if (index < 0 || index >= this->getEnemyCount())
      throw std::out_of_range("Enemies::getEnemy(): index out of range");
    if (this->haveBoss()) {
      if (index == 0) return *this->m_boss;
      return *this->m_enemies[index - 1];
    }
    return *this->m_enemies[index];
}
const Enemy& Enemies::getEnemy(int index) const
{
    return const_cast<Enemies*>(this)->getEnemy(index);
}

bool Enemies::haveBoss() const noexcept
{
    return !!this->m_boss;
}

Boss* Enemies::getBossPtr()
{
    return this->m_boss.get();
}
const Boss* Enemies::getBossPtr() const
{
    return const_cast<Enemies*>(this)->getBossPtr();
}
Boss& Enemies::getBoss()
{
    auto *result = this->getBossPtr();
    if (!result)
      throw std::runtime_error("Enemies::getBoss(): no boss present");
    return *result;
}
const Boss& Enemies::getBoss() const
{
    return const_cast<Enemies*>(this)->getBoss();
}

void Enemies::update(sf::Time dt)
{
    for (auto &enemy : *this)
      enemy.update(dt);
}

void Enemies::drawTo(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const auto &enemy : *this)
      target.draw(enemy, states);
}


void Enemies::filterDeadEnemies()
{
    if (auto *boss = this->getBossPtr()) {
      if (!boss->isAlive()) this->m_boss.reset();
    }
    removeIf(this->m_enemies, [](const Unique<Enemy> &enemy) 
        { 
            return !enemy->isAlive(); 
        });
}

void Enemies::addEnemy(Unique<Enemy> enemy)
{
    this->m_enemies.push_back(std::move(enemy));
}
void Enemies::addBoss(Unique<Boss> boss)
{
    this->m_boss = std::move(boss);
}

} // namespace LaserWave