#ifndef HITBOX_HPP_INCLUDED
#define HITBOX_HPP_INCLUDED

#include "geometry.hpp"
#include "gamemaths.hpp"

namespace LaserWave
{

template<int vertices>
class HitboxConvex : public IPolygon
{
  public:
    HitboxConvex(Point center = {}) : m_center(center), m_local_vertices() {}

    HitboxConvex(std::initializer_list<Point> points, Point center = {})
    : m_center()
    {
        if (points.size() > vertices)
            throw std::invalid_argument(Achibulup::stringFormat(
                "HitboxConvex::HitboxConvex(std::initializer_list<Point>): too many vertices (convex is ",
                vertices," points (n_vertices == ",points.size()));
        for (int i = 0; i < points.size(); ++i)
          this->m_local_vertices[i] = points.begin()[i];
    }

    int vertexCount() const noexcept override
    {
        return vertices;
    }
    
    Point getVertex(int index) const override
    {
        return this->getCenter() + this->getLocalVertex(index);
    }

    Point getCenter() const noexcept
    {
        return this->m_center;
    }
    void setCenter(Point pos) noexcept
    {
        this->m_center = pos;
    }

    void translate(sf::Vector2f delta) noexcept
    {
        this->m_center += delta;
    }

    /// get the vertex relative to center
    Point getLocalVertex(int index) const
    {
        this->checkIndex(index);
        return this->m_local_vertices[index];
    }
    void setLocalVertex(int index, Point pos)
    {
        this->checkIndex(index);
        this->m_local_vertices[index] = pos;
    }

  private:
    Point m_center;
    Point m_local_vertices[vertices];
};

template<int vertices>
class SymetricHitboxConvex : public IPolygon
{  
  public:
    SymetricHitboxConvex(Point center = {}, float radius = {}, Angle angle = {}) 
    : m_center(center), m_radius(radius), m_angle(angle) {}

    int vertexCount() const noexcept override
    {
        return vertices;
    }

    Point getVertex(int index) const override
    {
        return this->getCenter() + this->getLocalVertex(index);
    }

    Point getCenter() const noexcept
    {
        return this->m_center;
    }
    void setCenter(Point pos) noexcept
    {
        this->m_center = pos;
    }

    void translate(sf::Vector2f delta) noexcept
    {
        this->m_center += delta;
    }

    float getRadius() const noexcept
    {
        return this->m_radius;
    }
    void setRadius(float radius)
    {
        if (radius < 0)
          throw std::invalid_argument("SymetricHitboxConvex::setRadius(float): radius must be >= 0");
        this->m_radius = radius;
    }

    Angle getAngle() const noexcept
    {
        return this->m_angle;
    }
    void setAngle(Angle angle) noexcept
    {
        this->m_angle = angle;
    }

    void rotate(Angle angle)
    {
        this->m_angle += angle;
    }

    Angle getLocalAngle(int index) const
    {
        this->checkIndex(index);
        return this->getAngle() + Angle::fullCycle() * index / vertices; 
    }

    Point getLocalVertex(int index) const
    {
        return this->getRadius() * toDirection(this->getLocalAngle(index));
    }

  private:
    Point m_center;
    float m_radius;
    Angle m_angle;
};

} // namespace LaserWave

#endif // HITBOX_HPP_INCLUDED