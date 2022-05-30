#include "ConvexEntity.hpp"

namespace LaserWave
{

void ConvexEntity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto &&hitbox = this->getHitbox();
    auto vertices = hitbox.vertexCount();
    sf::ConvexShape shape(vertices);
    for (int i = 0; i < vertices; ++i)
        shape.setPoint(i, hitbox.getVertex(i));
    shape.setFillColor(this->getColor());
    target.draw(shape, states);
}

} // namespace LaserWave