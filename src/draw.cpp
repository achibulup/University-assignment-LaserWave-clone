#include "draw.hpp"

namespace LaserWave
{

static sf::ConvexShape draw_shape;
static sf::RectangleShape draw_rect;
static sf::CircleShape draw_circle;



sf::RenderStates operator * (const sf::Transform &transform,
                             const sf::RenderStates &states)
{
    sf::RenderStates result = states;
    result *= transform;
    return result;
}
sf::RenderStates& operator *= (sf::RenderStates &states,
                               const sf::Transform &transform)
{
    states.transform *= transform;
    return states;
}


void apply(sf::Shape &shape, Appearance app)
{
    shape.setFillColor(app.fillColor);
    shape.setOutlineColor(app.outlineColor);
    shape.setOutlineThickness(app.outlineThickness);
}
void apply(sf::Text &text, Appearance app)
{
    text.setFillColor(app.fillColor);
    text.setOutlineColor(app.outlineColor);
    text.setOutlineThickness(app.outlineThickness);
}


void Rect::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    draw_rect.setSize(this->getSize());
    static_cast<sf::Transformable&>(draw_rect) = *this;
    apply(draw_rect, this->getAppearance());

    target.draw(draw_rect, states);
}


void draw(sf::RenderTarget &target, const IConvexPolygon &poly, 
          Appearance app, sf::RenderStates states)
{
    auto vertices = poly.vertexCount();
    draw_shape.setPointCount(vertices);
    int i = 0;
    for (Point point : move(poly.vertices()))
      draw_shape.setPoint(i++, point);
    apply(draw_shape, app);
    target.draw(draw_shape, states);
}

void draw(sf::RenderTarget &target, const Circle &circle, 
          Appearance app, sf::RenderStates states)
{
    draw_circle.setOrigin(circle.getRadius(), circle.getRadius());
    draw_circle.setPosition(circle.getCenter());
    draw_circle.setRadius(circle.getRadius());
    apply(draw_circle, app);
    target.draw(draw_circle, states);
}

} // namespace LaserWave