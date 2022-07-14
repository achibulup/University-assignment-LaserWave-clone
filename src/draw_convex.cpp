#include "draw_convex.hpp"

namespace LaserWave
{

void Rect::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();
    target.draw(this->m_vertices, 4, sf::Quads, states);
}

static sf::ConvexShape shape;

void draw(sf::RenderTarget &target, const IConvexPolygon &poly, 
          sf::Color color, sf::RenderStates states)
{
    auto vertices = poly.vertexCount();
    shape.setPointCount(vertices);
    int i = 0;
    for (Point point : move(poly.vertices()))
      shape.setPoint(i++, point);
    shape.setFillColor(color);
    target.draw(shape, states);
}

} // namespace LaserWave