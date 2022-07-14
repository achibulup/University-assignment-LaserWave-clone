#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

#include "commons.hpp"
#include "gamemaths.hpp"
#include "IdTemplate.hpp"
#include <SFML/Achibulup_dependencies/PMIterator.hpp>
#include <SFML/Achibulup_dependencies/Range.hpp>
#include <string_view>
#include <functional>
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

    LineSegment() = default;
    LineSegment(Point p1, Point p2) noexcept
    : p1(p1), p2(p2) {}

    LineSegment(float p1X, float p1Y, float p2X, float p2Y) noexcept
    : p1(p1X, p1Y), p2(p2X, p2Y) {}
};
float length(const LineSegment &line);
bool intersects(const LineSegment&, const LineSegment&);

class Box
{
  public:
    Box() = default;

    Box(float x, float y) noexcept;
    Box(Vec2 size) noexcept;
    Box(float left, float top, float right, float bottom);
    Box(float left, float top, Vec2 size);

    float getWidth() const noexcept;
    float getHeight() const noexcept;
    Vec2 getSize() const noexcept;
    float getLeft() const noexcept;
    float getTop() const noexcept;
    float getRight() const noexcept;
    float getBottom() const noexcept;
    Point getCenter() const noexcept;
    Point getTopLeft() const noexcept;
    Point getTopRight() const noexcept;
    Point getBottomLeft() const noexcept;
    Point getBottomRight() const noexcept;
    /// get the topleft position
    Point getPosition() const noexcept;

    /// keep the other bounds
    void setLeft(float pos);
    /// keep the other bounds
    void setTop(float pos);
    /// keep the other bounds
    void setRight(float pos);
    /// keep the other bounds
    void setBottom(float pos);
    /// keep the size of the box
    void setCenter(Point pos);
    /// keep the other bounds
    void setTopRight(Point pos);
    /// keep the other bounds
    void setBottomLeft(Point pos);
    /// keep the other bounds
    void setBottomRight(Point pos);
    /// set the topleft postion
    void setPosition(Point pos);
    /// keep the position
    void setSize(Vec2 size);

    void move(Vec2 displacement);
    void setSizeKeepCenter(Vec2 size);
    
  private:
    Point m_topLeft = {};
    Vec2 m_size = {};
};
std::ostream& operator<<(std::ostream&, const Box&);


Box boundingBox(const LineSegment&);

bool intersects(const Box&, const Box&);

bool contains(const Box &box, Point point);

bool intersects(const Box&, const LineSegment&);
inline bool intersects(const LineSegment&, const Box&);


class Shape
{
  public:
    using Id = IdTemplate<Shape>;

    virtual ~Shape() = default;

    virtual Id getId() const noexcept = 0;

    virtual Box getBoundingBox() const noexcept = 0;

    // virtual float area() const noexcept = 0;

    DEBUG_ID_VAR
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

class IPolygon : public Shape INIT_DEBUG_ID(IPolygon)
{
  public:
    static const Shape::Id ID;
    
    Shape::Id getId() const noexcept override { return ID; }

    Box getBoundingBox() const noexcept override;

    virtual int vertexCount() const noexcept = 0;
    virtual Point getVertex(int index) const = 0;
    virtual LineSegment getEdge(int index) const;
    virtual List<Point> getVertices() const;

    class VertexIterator;
    
    class EdgeIterator : public Achibulup::IInputIterator<LineSegment>
    {
      public:
        explicit EdgeIterator(const IPolygon &polygon);

        LineSegment yield() override final;

      private:
        Achibulup::PMIIterator<Point> m_vertexIter;
        Point m_currentP1;
        Point m_first;
    };


    Achibulup::PMRange<Point> vertices() const;
    Achibulup::IterRange<EdgeIterator> edges() const;
  
  protected:
    virtual Achibulup::PMIIterator<Point> v_verticesBegin() const;
    EdgeIterator edgesBegin() const;

    ///throw if index is out of range
    void checkIndex(int index) const;
    void throwInvalidIndex(int index) const;
};

class IConvexPolygon : public IPolygon INIT_DEBUG_ID(IConvexPolygon) 
{
  public:
    static const Shape::Id ID;

    Shape::Id getId() const noexcept override { return ID; }
};

class Polygon : public IPolygon INIT_DEBUG_ID(Polygon)
{
  public:
    static const Shape::Id ID;

    /// a Polygon polygon always has at least 3 vertices

    Polygon();

    /// these constructors throw if the number of vertices is less than 3
    Polygon(int n_vertices);
    Polygon(std::initializer_list<Point> vertices);

    Shape::Id getId() const noexcept override final { return ID; }

    // float area() const noexcept override;

    int vertexCount() const noexcept override final;

    /// throws std::out_of_range if index is out of range
    Point& vertex(int index);
    Point vertex(int index) const;
    Point getVertex(int index) const override final;

    class VertexIterator : public Achibulup::IInputIterator<Point>
    {
      public:
        using iterator_category = std::forward_iterator_tag;

        explicit VertexIterator(Polygon &poly, int index = 0);

        Point& operator * () const;
        VertexIterator& operator ++ () &;

        Point yield() override final;

      private:
        Point *m_iter;
        int m_verticesLeft;
    };
    class ConstVertexIterator : public Achibulup::IInputIterator<Point>
    {
      public:
        using iterator_category = std::forward_iterator_tag;

        explicit ConstVertexIterator(const Polygon &poly, int index = 0);

        Point operator * () const;
        ConstVertexIterator& operator ++ () &;

        Point yield() override final;

      private:
        const Point *m_iter;
        int m_verticesLeft;
    };

    Achibulup::IterRange<VertexIterator> vertices();
    Achibulup::IterRange<ConstVertexIterator> vertices() const;

  private:
    VertexIterator verticesBegin();
    ConstVertexIterator verticesBegin() const;
    Achibulup::PMIIterator<Point> v_verticesBegin() const override final;

    bool useSmallOpt() const noexcept;
    Point* getArray() noexcept;
    const Point* getArray() const noexcept;
    void resize(int n_vertices);

    static constexpr int SMALL_POLYGON_OPT_THRESHOLD = 6;

    int m_nVertices;
    std::array<Point, SMALL_POLYGON_OPT_THRESHOLD> m_reservedVertices;
    std::vector<Point> m_list;
};

class Circle : public Shape INIT_DEBUG_ID(Circle)
{
  public:
    static const Shape::Id ID;

    Circle();
    Circle(Point center, float radius);

    Shape::Id getId() const noexcept override final { return ID; }

    Box getBoundingBox() const noexcept override final;

    Point getCenter() const noexcept;
    float getRadius() const noexcept;

    void setCenter(Point center) noexcept;
    /// throws std::invalid_argument if radius is negative
    void setRadius(float radius);

  private:
    void throwInvalidSetRadius(float radius) const;

    Point m_center;
    float m_radius;
};

} // namespace LaserWave

#include "geometry.inline.hpp"

#endif // GEOMETRY_HPP_INCLUDED