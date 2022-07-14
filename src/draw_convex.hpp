#ifndef CONVEX_ENTITY_HPP_INCLUDED
#define CONVEX_ENTITY_HPP_INCLUDED

#include "Entity.hpp"

namespace LaserWave
{

class Rect : public sf::Drawable, public sf::Transformable
{
  public:
    Rect() = default;
    Rect(sf::Vector2f size) : Rect()
    {
        this->setSize(size);
    }

    void setSize(sf::Vector2f size)
    {
        this->m_vertices[1].position.x = size.x;
        this->m_vertices[2].position = size;
        this->m_vertices[3].position.y = size.y;
    }

    void setColor(sf::Color color)
    {
        for (auto& vertex : this->m_vertices)
            vertex.color = color;
    }

    sf::Vector2f getSize() const
    {
        return this->m_vertices[2].position;
    }

  private:
    void draw(sf::RenderTarget&, sf::RenderStates = {}) const override;

    sf::Vertex m_vertices[4] = {};
};

void draw(sf::RenderTarget&, const IConvexPolygon&, 
          sf::Color, sf::RenderStates = {});

} // namespace LaserWave

#endif // CONVEX_ENTITY_HPP_INCLUDED