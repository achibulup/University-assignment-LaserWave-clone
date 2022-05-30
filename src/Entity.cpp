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
    auto &&hitbox = this->getHitbox();
    if (hitbox.getId() == IPolygon::ID) {
      const IPolygon &poly_hitbox = static_cast<const IPolygon&>(hitbox);
      auto vertices = poly_hitbox.vertexCount();
      sf::ConvexShape show(vertices);
      for (int i = 0; i < vertices; ++i)
        show.setPoint(i, poly_hitbox.getVertex(i));
      show.setOutlineColor(sf::Color::Red);
      show.setOutlineThickness(1.f);
      show.setFillColor(sf::Color::Transparent);
      target.draw(show);
    }
    else if (hitbox.getId() == Circle::ID) {
      const Circle &circle_hitbox = static_cast<const Circle&>(hitbox);
      sf::CircleShape show;
      show.setRadius(circle_hitbox.getRadius());
      show.setPosition(circle_hitbox.getCenter());
      show.setOrigin({-show.getRadius(), -show.getRadius()});
      show.setOutlineColor(sf::Color::Red);
      show.setOutlineThickness(1.f);
      show.setFillColor(sf::Color::Transparent);
      target.draw(show);
    }
    else throw std::runtime_error("unsupported hitbox");
}

const std::set<Entity*>& Entity::allEntities() noexcept
{
    return entity_list;
}

bool isColliding(const Entity &a, const Entity &b) noexcept
{
    return intersects(a.getHitbox(), b.getHitbox());
}


} // namespace LaserWave