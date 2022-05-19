#ifndef ENEMIES_HPP_INCLUDED
#define ENEMIES_HPP_INCLUDED

#include "Enemy.hpp"
#include <memory>

namespace LaserWave
{

class Enemies
{
  public:
    Enemies();
    Enemies(Enemies&&) = default;
    Enemies& operator = (Enemies&&) = default;

    int getEnemyCount() const;

    Enemy& getEnemy(int index);
    const Enemy& getEnemy(int index) const;

    class Iterator
    {
      public:
        Iterator(Enemies& enemies, int index) noexcept
        : m_enemies(&enemies), m_index(index) {}
        Iterator(const Iterator&) = default;
        Iterator& operator = (const Iterator&) & = default;
        Iterator& operator ++ () &
        {
            ++this->m_index;
            return *this;
        }
        Iterator operator ++ (int) &
        {
            Iterator res(*this);
            ++(*this);
            return res;
        }
        Enemy& operator * () const
        {
            return this->m_enemies->getEnemy(this->m_index);
        }
        Enemy* operator -> () const
        {
            return &this->m_enemies->getEnemy(this->m_index);
        }
        bool operator == (const Iterator& other) const
        {
            return this->m_index == other.m_index;
        }
        bool operator != (const Iterator& other) const
        {
            return !(*this == other);
        }

      private:
        Enemies *m_enemies;
        int m_index;
    };
    class ConstIterator
    {
      public:
        ConstIterator(const Enemies& enemies, int index) noexcept
        : m_enemies(&enemies), m_index(index) {}
        ConstIterator(const ConstIterator&) = default;
        ConstIterator& operator = (const ConstIterator&) & = default;
        ConstIterator& operator ++ () &
        {
            ++this->m_index;
            return *this;
        }
        ConstIterator operator ++ (int) &
        {
            ConstIterator res(*this);
            ++(*this);
            return res;
        }
        const Enemy& operator * () const
        {
            return this->m_enemies->getEnemy(this->m_index);
        }
        const Enemy* operator -> () const
        {
            return &this->m_enemies->getEnemy(this->m_index);
        }
        bool operator == (const ConstIterator& other) const
        {
            return this->m_index == other.m_index;
        }
        bool operator != (const ConstIterator& other) const
        {
            return !(*this == other);
        }

      private:
        const Enemies *m_enemies;
        int m_index;
    };

    Iterator begin() &
    {
        return Iterator(*this, 0);
    }
    Iterator end() &
    {
        return Iterator(*this, this->m_enemies.size());
    }
    ConstIterator begin() const &
    {
        return ConstIterator(*this, 0);
    }
    ConstIterator end() const &
    {
        return ConstIterator(*this, this->m_enemies.size());
    }

    void update(sf::Time dt);
    void drawTo(sf::RenderTarget &target, sf::RenderStates = {}) const;

    void addRandomEnemy(sf::Vector2f position, sf::Vector2f player_position);
    void filterDeadEnemies();

  private:
    void addEnemy(std::unique_ptr<Enemy> enemy);

    std::vector<std::unique_ptr<Enemy>> m_enemies;
};

}

#endif // ENEMIES_HPP_INCLUDED