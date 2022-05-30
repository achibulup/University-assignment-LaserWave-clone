#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

#include "commons.hpp"
#include <string_view>
#include <type_traits>
#include <SFML/Graphics.hpp>
#include <array>

namespace LaserWave
{

using Vec2 = sf::Vector2f;

using Point = Vec2;

struct LineSegment
{
    Point p1, p2;

    LineSegment(Point p1, Point p2) noexcept
    : p1(p1), p2(p2) {}

    LineSegment(float p1X, float p1Y, float p2X, float p2Y) noexcept
    : p1(p1X, p1Y), p2(p2X, p2Y) {}
};
float length(LineSegment);

class Shape
{
  public:
    class Id : public std::string_view
    {
      public:
        using std::string_view::string_view;

        friend bool operator == (Id lhs, Id rhs)
        {
            return lhs.data() == rhs.data() && lhs.size() == rhs.size();
        }
        friend bool operator != (Id lhs, Id rhs)
        {
            return !(lhs == rhs);
        }
    };

    virtual ~Shape() = default;

    virtual Id getId() const noexcept = 0;

    // virtual float area() const noexcept = 0;
};

// template<typename ShapeType>
// std::enable_if_t<std::is_convertible_v<ShapeType*, Shape*>, ShapeType&>
// DynamicCast(Shape &shape)
// {
//     if (shape.getId() != ShapeType::ID)
//       throw std::bad_cast();
//     return static_cast<ShapeType&>(shape);
// }
// template<typename ShapeType>
// std::enable_if_t<std::is_convertible_v<ShapeType*, Shape*>, const ShapeType&>
// DynamicCast(const Shape &shape)
// {
//     if (shape.getId() != ShapeType::ID)
//       throw std::bad_cast();
//     return static_cast<const ShapeType&>(shape);
// }
// template<typename ShapeType>
// std::enable_if_t<std::is_convertible_v<ShapeType*, Shape*>, ShapeType&&>
// DynamicCast(Shape &&shape)
// {
//     if (shape.getId() != ShapeType::ID)
//       throw std::bad_cast();
//     return static_cast<ShapeType&&>(shape);
// }
// template<typename ShapeType>
// std::enable_if_t<std::is_convertible_v<ShapeType*, Shape*>, const ShapeType&&>
// DynamicCast(const Shape &&shape)
// {
//     if (shape.getId() != ShapeType::ID)
//       throw std::bad_cast();
//     return static_cast<const ShapeType&&>(shape);
// }
// template<typename ShapeType>
// std::enable_if_t<std::is_convertible_v<ShapeType*, Shape*>, ShapeType*>
// DynamicCast(Shape *shape)
// {
//     if (shape.getId() != ShapeType::ID)
//       return nullptr;
//     return static_cast<ShapeType*>(shape);
// }
// template<typename ShapeType>
// std::enable_if_t<std::is_convertible_v<ShapeType*, Shape*>, const ShapeType*>
// DynamicCast(const Shape *shape)
// {
//     if (shape.getId() != ShapeType::ID)
//       return nullptr;
//     return static_cast<const ShapeType*>(shape);
// }

class IPolygon : public Shape
{
  public:
    static const Shape::Id ID;
    
    Shape::Id getId() const noexcept override
    {
        return ID;
    }

    virtual int vertexCount() const noexcept = 0;

    virtual Point getVertex(int index) const = 0;

    virtual LineSegment getEdge(int index) const;

    virtual List<Point> getVertices() const;
  
  protected:
    ///throw if index is out of range
    void checkIndex(int index) const;
};

class Polygon : public IPolygon
{
  public:

    /// a Polygon polygon always has at least 3 vertices

    Polygon() : Polygon(3) {}

    /// these constructors throw if the number of vertices is less than 3
    Polygon(int n_vertices);
    
    Polygon(std::initializer_list<Point> vertices);

    // float area() const noexcept override;

    int vertexCount() const noexcept override;

    /// throws std::out_of_range if index is out of range
    Point& vertex(int index);
    const Point& vertex(int index) const;

    Point getVertex(int index) const override
    {
        return this->vertex(index);
    }

  private:
    void resize(int n_vertices);
    void addVertex(Point vertex);

    static constexpr int SMALL_POLYGON_OPT_THRESHOLD = 6;

    int m_nVertices;
    std::array<Point, SMALL_POLYGON_OPT_THRESHOLD> m_reservedVertices;
    List<Vec2> m_remainingVertices;
};

class Circle : public Shape
{
  public:
    static const Shape::Id ID;

    Circle()
    {
        this->setRadius(0);
    }

    Circle(Point center, float radius)
    {
        this->setCenter(center);
        this->setRadius(radius);
    }

    Shape::Id getId() const noexcept override
    {
        return ID;
    }

    // float area() const noexcept override;

    Point getCenter() const noexcept
    {
        return this->m_center;
    }
    float getRadius() const noexcept
    {
        return this->m_radius;
    }

    void setCenter(Point center) noexcept
    {
        this->m_center = center;
    }
    /// throws std::invalid_argument if radius is negative
    void setRadius(float radius);

  private:
    Point m_center;
    float m_radius;
};




bool contains(const IPolygon&, Point);
bool contains(const Circle&, Point);

bool intersects(const LineSegment&, const LineSegment&);
bool intersects(const IPolygon&, const IPolygon&);
bool intersects(const Circle&, const Circle&);

bool intersects(const LineSegment&, const IPolygon&);
bool intersects(const LineSegment&, const Circle&);
bool intersects(const IPolygon&, const Circle&);

inline bool intersects(const IPolygon &lhs, const LineSegment &rhs)
{ return intersects(rhs, lhs);}
inline bool intersects(const Circle &lhs, const LineSegment &rhs)
{ return intersects(rhs, lhs); }
inline bool intersects(const Circle &lhs, const IPolygon &rhs)
{ return intersects(rhs, lhs); }



bool intersects(const Shape &lhs, const Shape &rhs);

} // namespace LaserWave

#endif // GEOMETRY_HPP_INCLUDED