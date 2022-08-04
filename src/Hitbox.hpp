#ifndef HITBOX_HPP_INCLUDED
#define HITBOX_HPP_INCLUDED

#include "geometry.hpp"
#include "gamemaths.hpp"

#include <fstream>

extern std::ofstream log_file;

namespace LaserWave
{

class HitboxConvex : public IConvexPolygon
{
  public:
    HitboxConvex();
    HitboxConvex(Point center);
    
    Point getVertex(int index) const override final;

    Point getCenter() const noexcept;
    void setCenter(Point pos) noexcept;

    void translate(sf::Vector2f delta) noexcept;

    /// get the vertex relative to center
    virtual Point getLocalVertex(int index) const = 0;

  protected:
    Point m_center;
};

class StaticHitboxConvexBase : public HitboxConvex
{
  public:
    using HitboxConvex::HitboxConvex;

    class VertexIterator : public Achibulup::IInputIterator<Point>
    {
      public:
        VertexIterator(Point center, const Point *vertex_array, int n_vertex);

        Point operator * () const;
        VertexIterator& operator ++ () &;

        Point yield() override final;

      private:
        Point m_center;
        const Point *m_localVertexIter;
        int m_verticesLeft;
    };

  protected:
    void init(Point *array, int vertices, Point *args, int argc);
};

template<int VERTICES>
class StaticHitboxConvex : public StaticHitboxConvexBase INIT_DEBUG_ID(StaticHitboxConvex<VERTICES>)
{
    static_assert(VERTICES >= 3, "A polygon should have at least 3 vertices");

  public:
    static const Shape::Id ID;

    StaticHitboxConvex(Point center = {});
    StaticHitboxConvex(std::initializer_list<Point> points, Point center = {});

    Shape::Id getId() const noexcept override final { return ID; }

    int vertexCount() const noexcept override final;

    /// get the vertex relative to center
    Point getLocalVertex(int index) const override final;
    void setLocalVertex(int index, Point pos);

    void copyVertices(Point *destination) const override final;

    Achibulup::IterRange<VertexIterator> vertices() const;

  protected:
    StaticHitboxConvex(const Point *args, int n_arg, Point center = {});

    VertexIterator verticesBegin() const;
    Achibulup::PMIIterator<Point> v_verticesBegin() const override final;

  private:
    Point m_local_vertices[VERTICES];
};

class RegularHitboxConvex : public HitboxConvex INIT_DEBUG_ID(RegularHitboxConvex)
{
  public:
    static const Shape::Id ID;

    RegularHitboxConvex(int n_vertices = 3, Point center = {}, 
                        float radius = {}, Angle angle = {});

    Shape::Id getId() const noexcept override final { return ID; }

    int vertexCount() const noexcept override final;
    void setVertexCount(int n_vertices);

    float getRadius() const noexcept;
    void setRadius(float radius);

    Angle getAngle() const noexcept;
    void setAngle(Angle angle) noexcept;

    void rotate(Angle angle);
    Angle getLocalAngle(int index) const;
    Point getLocalVertex(int index) const override final;

    void copyVertices(Point *destination) const override final;

    class VertexIterator : public Achibulup::IInputIterator<Point>
    {
      public:
        using iterator_category = std::forward_iterator_tag;

        explicit VertexIterator(const RegularHitboxConvex &hitbox, 
                                int index = 0);

        Point operator * () const;
        VertexIterator& operator ++ () &;

        Point yield() override final;

      private:
        Point m_center;
        Vec2 m_current;
        Vec2 m_transform;
        int m_verticesLeft;
    };

    Achibulup::IterRange<VertexIterator> vertices() const;

  private:
    VertexIterator verticesBegin() const;
    Achibulup::PMIIterator<Point> v_verticesBegin() const override final;

    Vec2 getDirection(int index) const;

    static Vec2 rotateTransform(Vec2 v1, Vec2 v2);

    void throwInvalidVertexCount(int n_vertices) const;
    void throwInvalidRadius(float radius) const;

    int m_nVertices;
    Vec2 m_firstDirCache;
    Vec2 m_transformCache;
    float m_radius;
    Angle m_angle;
};





template<typename SHape>
class BasicHitboxRef
{
  public:
    explicit BasicHitboxRef(const SHape &shape)
    : m_shape(&shape), m_bound(shape.getBoundingBox()) {}

    BasicHitboxRef(const SHape &shape, const Box &bounding_box)
    : m_shape(&shape), m_bound(bounding_box) {}

    template<typename SHape2>
    BasicHitboxRef(const BasicHitboxRef<SHape2> &other)
    : BasicHitboxRef(other.getShape(), other.getBoundingBox()) {}

    const SHape& getShape() const noexcept
    {
        return *this->m_shape;
    }

    Box getBoundingBox() const noexcept
    {
        // ::log_file << this->m_bound;
        return this->m_bound;
    }

  private:
    const SHape *m_shape;
    Box m_bound; 
};

using HitboxRef = BasicHitboxRef<Shape>;


template<class Container>
class HitBoxBoundingBox
{
  public:
    HitBoxBoundingBox()
    {
        static_cast<Container&>(*this).updateBoundingBox();
    }

    Box getBoundingBox() const noexcept
    {
        return this->m_bound;
    }

  protected:
    Box m_bound;
};

template<class SHape>
class BasicHitbox : public SHape, public HitBoxBoundingBox<BasicHitbox<SHape>>
{
    using ShapeBase = SHape;
    using BoxBase = HitBoxBoundingBox<BasicHitbox<SHape>>;

  public:
    using SHape::SHape;
    using BoxBase::getBoundingBox;

    const Shape& getShape() const noexcept
    {
        return *this;
    }

    void updateBoundingBox()
    {
        this->m_bound = SHape::getBoundingBox();
    }

    operator BasicHitboxRef<SHape>() const noexcept
    {
        // ::log_file << "hitbox->ref: " << this->getBoundingBox() << std::endl;
        return BasicHitboxRef<SHape>(*this, this->getBoundingBox());
    }
    template<class SHape2>
    explicit operator BasicHitboxRef<SHape2>() const noexcept
    {
        return BasicHitboxRef<SHape2>(*this, this->getBoundingBox());
    }
};


} // namespace LaserWave

#include "Hitbox.inline.hpp"

#endif // HITBOX_HPP_INCLUDED