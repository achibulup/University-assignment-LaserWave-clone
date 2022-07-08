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
    using Hitbox = LaserWave::HitboxRef;
    using Id = IdTemplate<Entity>;

    Entity();
    Entity(const Entity&);
    virtual ~Entity();

    virtual Id getId() const noexcept = 0;

    virtual Point getCenter() const = 0;
    Hitbox getHitbox() const { return this->v_getHitbox(); }

    virtual void update(sf::Time dt) {}

    bool isAlive() const noexcept { return this->m_isAlive; }
    friend void kill(Entity &entity) noexcept { entity.m_isAlive = false; }


    static const std::set<Entity*>& allEntities() noexcept;
    void showHitBoxTo(sf::RenderWindow &target) const;

  protected:
    virtual Hitbox v_getHitbox() const = 0;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override = 0;

    bool m_isAlive = true;

    DEBUG_ID_VAR
};

bool isColliding(const Entity &a, const Entity &b) noexcept;

} // namespace LaserWave
#endif // ENTITY_HPP_INCLUDED