#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <set>

namespace LaserWave
{

struct EntityPosition
{
    sf::Vector2f center;
    sf::Vector2f size;
    sf::Vector2f topleft_offset;
    EntityPosition(sf::Vector2f center = {}, sf::Vector2f size = {}) noexcept
    : EntityPosition(center, size, size / -2.f) {}
    EntityPosition(sf::Vector2f center, sf::Vector2f size, sf::Vector2f topleft) noexcept
    : center(center), size(size), topleft_offset(topleft) {}

    sf::Vector2f getCenter() const noexcept 
    { 
        return this->center; 
    }
    sf::FloatRect getHitbox() const noexcept
    {
        return sf::FloatRect(this->topleft_offset + this->center, this->size);
    }

    EntityPosition& translate(sf::Vector2f offset) noexcept
    {
        this->center += offset;
        return *this;
    }
};

class Entity : public sf::Drawable
{
  public:
    Entity();
    Entity(const Entity&);
    virtual ~Entity();
    virtual sf::Vector2f getCenter() const = 0;
    virtual sf::FloatRect getHitbox() const = 0;
    virtual void update(sf::Time dt) {}

    bool isAlive() const noexcept
    {
        return this->m_is_alive;
    }

    friend void kill(Entity &entity) noexcept
    {
        entity.m_is_alive = false;
    }

    static const std::set<Entity*>& allEntities() noexcept;

    void showHitBoxTo(sf::RenderWindow &target) const;

  protected:
    bool m_is_alive = true;

};

bool isColliding(const Entity &a, const Entity &b) noexcept;

} // namespace LaserWave
#endif // ENTITY_HPP_INCLUDED