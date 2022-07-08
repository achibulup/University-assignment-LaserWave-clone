#include "Hitbox.hpp"
#include <SFML/Achibulup_dependencies/StringConstant.hpp>

namespace LaserWave
{

inline HitboxConvex::HitboxConvex() {}
inline HitboxConvex::HitboxConvex(Point center) : m_center(center) {}
// HitboxConvex(std::initializer_list<Point> points, Point center = {})
// : m_center()
// {
//     if (points.size() > vertices)
//         throw std::invalid_argument(Achibulup::stringFormat(
//             "HitboxConvex::HitboxConvex(std::initializer_list<Point>): too many vertices (convex is ",
//             vertices," points (n_vertices == ",points.size()));
//     for (int i = 0; i < points.size(); ++i)
//       this->m_local_vertices[i] = points.begin()[i];
// }

inline Point HitboxConvex::getVertex(int index) const
{
    return this->getCenter() + this->getLocalVertex(index);
}

inline Point HitboxConvex::getCenter() const noexcept
{
    return this->m_center;
}
inline void HitboxConvex::setCenter(Point pos) noexcept
{
    this->m_center = pos;
}

inline void HitboxConvex::translate(sf::Vector2f delta) noexcept
{
    this->m_center += delta;
}



inline StaticHitboxConvexBase::VertexIterator::
VertexIterator(Point center, const Point *vertex_array, int n_v)
: m_center(center), m_localVertexIter(vertex_array), m_verticesLeft(n_v) {}

inline Point StaticHitboxConvexBase::VertexIterator::operator * () const
{
    return this->m_center + *this->m_localVertexIter;
}

inline StaticHitboxConvexBase::VertexIterator& 
StaticHitboxConvexBase::VertexIterator::operator ++ () &
{
    ++this->m_localVertexIter;
    --this->m_verticesLeft;
    this->setIsEnd(this->m_verticesLeft == 0);
    return *this;
}

inline Point StaticHitboxConvexBase::VertexIterator::yield()
{
    auto res = **this;
    ++*this;
    return res;
}


#define NAMEOF(...) ACHIBULUP__NAMEOF(__VA_ARGS__)
#define TOSTRING(...) ACHIBULUP__TOSTRING(__VA_ARGS__)

template<int VERTICES>
const Shape::Id StaticHitboxConvex<VERTICES>::ID(NAMEOF(StaticHitboxConvex<) + TOSTRING(VERTICES) + NAMEOF(>));

#undef NAMEOF
#undef TOSTRING

template<int VERTICES>
StaticHitboxConvex<VERTICES>::StaticHitboxConvex(Point center) 
: StaticHitboxConvexBase(center), m_local_vertices() {}

template<int VERTICES>
StaticHitboxConvex<VERTICES>::
StaticHitboxConvex(std::initializer_list<Point> points, Point center)
: StaticHitboxConvex(points.begin(), points.size(), center) {}

template<int VERTICES>
int StaticHitboxConvex<VERTICES>::vertexCount() const noexcept
{
    return VERTICES;
}

/// get the vertex relative to center
template<int VERTICES>
Point StaticHitboxConvex<VERTICES>::getLocalVertex(int index) const
{
    this->checkIndex(index);
    return this->m_local_vertices[index];
}
template<int VERTICES>
void StaticHitboxConvex<VERTICES>::setLocalVertex(int index, Point pos)
{
    this->checkIndex(index);
    this->m_local_vertices[index] = pos;
}

template<int VERTICES>
auto StaticHitboxConvex<VERTICES>::vertices() const
-> Achibulup::IterRange<VertexIterator> 
{
    return {this->verticesBegin()};
}

template<int VERTICES>
StaticHitboxConvex<VERTICES>::
StaticHitboxConvex(const Point *args, int n_arg, Point center)
: StaticHitboxConvexBase(center)
{
    this->init(this->m_local_vertices, VERTICES, args, n_arg);
}

template<int VERTICES>
auto StaticHitboxConvex<VERTICES>::verticesBegin() const -> VertexIterator
{
    return VertexIterator(this->m_center, this->m_local_vertices, VERTICES);
}

template<int VERTICES>
Achibulup::PMIIterator<Point> 
StaticHitboxConvex<VERTICES>::v_verticesBegin() const
{
    return this->verticesBegin();
}




inline int RegularHitboxConvex::vertexCount() const noexcept
{
    return this->m_nVertices;
}

inline float RegularHitboxConvex::getRadius() const noexcept
{
    return this->m_radius;
}
inline void RegularHitboxConvex::setRadius(float radius)
{
    if (radius < 0) this->throwInvalidRadius(radius);
    this->m_radius = radius;
}

inline Angle RegularHitboxConvex::getAngle() const noexcept
{
    return this->m_angle;
}
inline void RegularHitboxConvex::setAngle(Angle angle) noexcept
{
    this->m_angle = angle;
    this->m_firstDirCache = toDirection(angle);
}

inline void RegularHitboxConvex::rotate(Angle angle)
{
    this->setAngle(this->getAngle() + angle);
}

inline Angle RegularHitboxConvex::getLocalAngle(int index) const
{
    this->checkIndex(index);
    return this->getAngle() 
          + Angle::fullCycle() * index / this->vertexCount(); 
}

inline Point RegularHitboxConvex::getLocalVertex(int index) const
{
    this->checkIndex(index);
    return this->getRadius() * this->getDirection(index);
}


inline RegularHitboxConvex::VertexIterator::
VertexIterator(const RegularHitboxConvex &hitbox, int index)
: m_center(hitbox.getCenter()), m_current(hitbox.getLocalVertex(index)),
  m_transform(hitbox.m_transformCache), 
  m_verticesLeft(hitbox.vertexCount() - index) {}


inline Point RegularHitboxConvex::VertexIterator::operator * () const
{
    return this->m_center + this->m_current;
}
inline RegularHitboxConvex::VertexIterator&
RegularHitboxConvex::VertexIterator::operator ++ () &
{
    this->m_current = rotateTransform(this->m_transform, 
                                      this->m_current);
    this->m_verticesLeft--;
    this->setIsEnd(this->m_verticesLeft == 0);
    return *this;
}

inline Point RegularHitboxConvex::VertexIterator::yield()
{
    auto res = **this;
    ++*this;
    return res;
}

inline Achibulup::IterRange<RegularHitboxConvex::VertexIterator> 
RegularHitboxConvex::vertices() const
{
    return {this->verticesBegin()};
}

inline RegularHitboxConvex::VertexIterator 
RegularHitboxConvex::verticesBegin() const
{
    return VertexIterator(*this);
}
inline Achibulup::PMIIterator<Point> 
RegularHitboxConvex::v_verticesBegin() const
{
    return this->verticesBegin();
}

inline Vec2 RegularHitboxConvex::rotateTransform(Vec2 v1, Vec2 v2)
{
    return {v1.x * v2.x - v1.y * v2.y, v1.x * v2.y + v1.y * v2.x};
}

} // namespace LaserWave
