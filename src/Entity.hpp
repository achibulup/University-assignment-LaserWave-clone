#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#include "commons.hpp"
#include "Hitbox.hpp"
#include <SFML/Graphics.hpp>
#include <set>

namespace LaserWave
{

class Entity : public sf::Drawable
{
  public:
    Entity();
    Entity(const Entity&);
    virtual ~Entity();
    virtual Point getCenter() const = 0;
    virtual const Shape& getHitbox() const = 0;
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
    virtual void draw(sf::RenderTarget &target, sf::RenderStates) const override = 0;

    bool m_is_alive = true;

};

bool isColliding(const Entity &a, const Entity &b) noexcept;

} // namespace LaserWave
#endif // ENTITY_HPP_INCLUDED