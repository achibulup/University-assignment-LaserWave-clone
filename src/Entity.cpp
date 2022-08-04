#include "Entity.hpp"
#include "collisions.hpp"
#include "draw.hpp"

namespace LaserWave
{

static std::set<Entity*> entity_list;

Entity::Entity()
{
    entity_list.insert(this);
}
Entity::Entity(const Entity&) : Entity() {}
Entity::~Entity()
{
    entity_list.erase(this);
}

void Entity::showHitBoxTo(sf::RenderWindow &target) const
{
    auto &&hitbox = this->getHitbox().getShape();
    if (auto *poly = dynamic_cast<const IConvexPolygon*>(&hitbox)) {
      Appearance outline(sf::Color::Transparent, sf::Color::Green, 1.f);
      LaserWave::draw(target, *poly, outline);
    }
    else if (auto *cir = dynamic_cast<const Circle*>(&hitbox)) {
      Appearance outline(sf::Color::Transparent, sf::Color::Green, 1.f);
      LaserWave::draw(target, *cir, outline);
    }
    else throw std::runtime_error("unsupported hitbox");
}

const std::set<Entity*>& Entity::allEntities() noexcept
{
    return entity_list;
}

bool isColliding(const Entity &a, const Entity &b) noexcept
{
    return collides(a.getHitbox(), b.getHitbox());
}


} // namespace LaserWave