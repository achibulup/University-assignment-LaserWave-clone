#include "Entity.hpp"

namespace LaserWave
{

static std::set<Entity*> entity_list;

Entity::Entity()
{
    entity_list.insert(this);
}
Entity::Entity(const Entity&)
{
    entity_list.insert(this);
}
Entity::~Entity()
{
    entity_list.erase(this);
}

void Entity::showHitBoxTo(sf::RenderWindow &target) const
{
    auto hitbox = this->getHitbox();
    sf::RectangleShape rect;
    rect.setPosition({hitbox.left, hitbox.top});
    rect.setSize({hitbox.width, hitbox.height});
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1.f);
    rect.setFillColor(sf::Color::Transparent);
    target.draw(rect);
}

const std::set<Entity*>& Entity::allEntities() noexcept
{
    return entity_list;
}

bool isColliding(const Entity &a, const Entity &b) noexcept
{
    return a.getHitbox().intersects(b.getHitbox());
}


} // namespace LaserWave