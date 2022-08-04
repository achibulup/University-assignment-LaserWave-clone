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
#include <variant>

namespace LaserWave
{


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

class VertexBuffer
{
  public:
    VertexBuffer(int size = 0);
    VertexBuffer(const VertexBuffer&);
    VertexBuffer(VertexBuffer&&) noexcept;
    VertexBuffer& operator=(const VertexBuffer&) &;
    VertexBuffer& operator=(VertexBuffer&&) & noexcept;
    ~VertexBuffer();

    int size() const noexcept;
    void resize(int size);

    Point* getArray() noexcept;
    const Point* getArray() const noexcept;
  
  private:
    static constexpr int SMALL_POLYGON_OPT_THRESHOLD = 9;
    using StaticArray = std::array<Point, SMALL_POLYGON_OPT_THRESHOLD>;
    using DynamicArray = std::vector<Point>;
    
    bool useSmallOpt() const noexcept;

    int m_size = 0;
    enum ArrayType : int { STATIC, DYNAMIC } m_type = STATIC;
    union {
      StaticArray m_static = {};
      DynamicArray m_dynamic;
    };
};

class IPolygon : public Shape INIT_DEBUG_ID(IPolygon)
{
  public:
    static const Shape::Id ID;
    
    Shape::Id getId() const noexcept override { return ID; }

    Box getBoundingBox() const noexcept override;

    virtual int vertexCount() const noexcept = 0;
    virtual Point getVertex(int index) const = 0;
    virtual LineSegment getEdge(int index) const;
    virtual void copyVertices(Point *destination) const;
    
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

/// Used to iterate over the vertices of a polygon.
/// A copy of the first vertex is appended to the end of the array 
/// to make it easy to iterate the edges of the polygon.
class VertexList
{
  public:
    VertexList();
    VertexList(const IPolygon &polygon);

    void create(const IPolygon &polygon);

    int size() const noexcept;

    /// this enables both indexing and underlying array access
    operator const Point* () const noexcept;
  
    const Point* begin() const noexcept;
    const Point* end() const noexcept;

    class EdgeIterator
    {
      public:
        EdgeIterator(const Point*);

        LineSegment operator * () const;
        EdgeIterator& operator ++ () &;
        bool operator == (const EdgeIterator&) const;
        bool operator != (const EdgeIterator&) const;

      private:
        const Point *m_p;
    };

    Achibulup::IterRange<EdgeIterator> edges() const noexcept;

  private:
    VertexBuffer m_buffer;
};
class EdgeList
{
  public:
    EdgeList();
    EdgeList(const IPolygon &polygon);

    explicit EdgeList(VertexList vertices);

    void create(const IPolygon &polygon);

    int size() const noexcept;

    LineSegment operator [] (int index) const;
  
    VertexList::EdgeIterator begin() const noexcept;
    VertexList::EdgeIterator end() const noexcept;

  private:
    VertexList m_vertices;
};

VertexList getVertices(const IPolygon &polygon);
EdgeList getEdges(const IPolygon &polygon);



class IConvexPolygon : public IPolygon INIT_DEBUG_ID(IConvexPolygon) 
{
  public:
    static const Shape::Id ID;

    Shape::Id getId() const noexcept override { return ID; }
};

/// represent a general polygon
/// a Polygon always has at least 3 vertices
/// this class use a contiguous array to store the vertices
class Polygon : public IPolygon INIT_DEBUG_ID(Polygon)
{
  public:
    static const Shape::Id ID;

    /// same as Polygon(3)
    Polygon();
    /// these constructors throw if the number of vertices is less than 3
    Polygon(int n_vertices);
    Polygon(std::initializer_list<Point> vertices);
    ~Polygon();

    Shape::Id getId() const noexcept override final { return ID; }

    // float area() const noexcept override;

    int vertexCount() const noexcept override final;

    /// throws std::out_of_range if index is out of range
    Point& vertex(int index);
    Point vertex(int index) const;
    Point getVertex(int index) const override final;
    void copyVertices(Point *destination) const override final;

    Achibulup::IterRange<Point*> vertices();
    Achibulup::IterRange<const Point*> vertices() const;

    /// get pointer to array of vertices
    Point* getArray() noexcept;
    const Point* getArray() const noexcept;

  private:
    Achibulup::PMIIterator<Point> v_verticesBegin() const override final;
    void resize(int n_vertices);

    VertexBuffer m_buffer;
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