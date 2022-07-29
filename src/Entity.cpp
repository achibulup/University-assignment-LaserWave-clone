#include "Entity.hpp"
#include "collisions.hpp"

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
      const IPolygon &poly_hitbox = *poly;
      auto vertices = poly_hitbox.vertexCount();
      sf::ConvexShape show(vertices);
      int cnt = 0;
      for (Point v : poly_hitbox.vertices())
        show.setPoint(cnt++, v);
      show.setOutlineColor(sf::Color::Green);
      show.setOutlineThickness(1.f);
      show.setFillColor(sf::Color::Transparent);
      target.draw(show);
    }
    else if (auto *cir = dynamic_cast<const Circle*>(&hitbox)) {
      const Circle &circle_hitbox = *cir;
      sf::CircleShape show;
      show.setRadius(circle_hitbox.getRadius());
      show.setPosition(circle_hitbox.getCenter());
      show.setOrigin({-show.getRadius(), -show.getRadius()});
      show.setOutlineColor(sf::Color::Green);
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
    return collides(a.getHitbox(), b.getHitbox());
}


} // namespace LaserWave