#include "ConvexEntity.hpp"

namespace LaserWave
{

void draw(sf::RenderTarget &target, const IConvexPolygon &poly, 
          sf::Color color, sf::RenderStates states)
{
    auto vertices = poly.vertexCount();
    sf::ConvexShape shape(vertices);
    for (int i = 0; i < vertices; ++i)
      shape.setPoint(i, poly.getVertex(i));
    shape.setFillColor(color);
    target.draw(shape, states);
}

} // namespace LaserWave