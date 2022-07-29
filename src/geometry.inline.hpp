#include "geometry.hpp"
namespace LaserWave
{
inline float length(const LineSegment &line)
{
    return abs(line.p1 - line.p2);
}


inline bool contains(const Box &box, Point point)
{
    return ((box.getLeft() <= point.x) & (point.x <= box.getRight()))
        && ((box.getTop() <= point.y) & (point.y <= box.getBottom()));
}

inline Box boundingBox(const LineSegment &line)
{
    return {std::min(line.p1.x, line.p2.x), std::min(line.p1.y, line.p2.y),
            std::max(line.p1.x, line.p2.x), std::max(line.p1.y, line.p2.y)};
}

inline bool intersects(const Box &box1, const Box &box2)
{
    return ((box1.getLeft() <= box2.getRight()) 
         & (box1.getRight() >= box2.getLeft()))
        && ((box1.getTop() <= box2.getBottom())
         & (box1.getBottom() >= box2.getTop()));
}

inline bool intersects(const LineSegment &lhs, const Box &rhs)
{ return intersects(rhs, lhs); }



inline IPolygon::EdgeIterator::EdgeIterator(const IPolygon &polygon)
: m_vertexIter(polygon.v_verticesBegin())
{
    this->m_currentP1 = *this->m_vertexIter;
    ++this->m_vertexIter;
    this->m_first = this->m_currentP1;
}

inline LineSegment IPolygon::EdgeIterator::yield()
{
    Point next_vertex;
    if (this->m_vertexIter.isEnd()) {
      this->markAsEnd();
      next_vertex = this->m_first;
    }
    else {
      next_vertex = *this->m_vertexIter;
      ++this->m_vertexIter;
    }
    LineSegment res(this->m_currentP1, next_vertex);
    this->m_currentP1 = next_vertex;
    return res;
}


inline Achibulup::PMRange<Point> IPolygon::vertices() const 
{
    return {this->v_verticesBegin()};
}
inline Achibulup::IterRange<IPolygon::EdgeIterator> IPolygon::edges() const
{
    return {this->edgesBegin()};
}

inline IPolygon::EdgeIterator IPolygon::edgesBegin() const
{
    return EdgeIterator(*this);
}

inline void IPolygon::checkIndex(int index) const
{
    if (index < 0 || index >= this->vertexCount()) 
      this->throwInvalidIndex(index);
}





inline Polygon::Polygon() : Polygon(3) {}
inline int Polygon::vertexCount() const noexcept
{
    return this->m_nVertices;
}
inline Point& Polygon::vertex(int index)
{
    this->checkIndex(index);
    return this->getArray()[index];
}
inline Point Polygon::vertex(int index) const
{
    return const_cast<Polygon*>(this)->vertex(index);
}
inline Point Polygon::getVertex(int index) const
{ 
    return this->vertex(index); 
}
inline bool Polygon::useSmallOpt() const noexcept
{
    return this->m_list.empty();
}
inline Point* Polygon::getArray() noexcept
{
    if (this->useSmallOpt()) return this->m_reservedVertices.data();
    return this->m_list.data();
}
inline const Point* Polygon::getArray() const noexcept
{
    return const_cast<Polygon&>(*this).getArray();
}


inline Polygon::VertexIterator::VertexIterator(Polygon &poly, int index)
: m_iter(poly.getArray()), m_verticesLeft(poly.vertexCount() - index) {}

inline Point& Polygon::VertexIterator::operator * () const
{
    return *this->m_iter;
}

inline Polygon::VertexIterator& Polygon::VertexIterator::operator ++ () &
{
    ++this->m_iter;
    --this->m_verticesLeft;
    this->setIsEnd(this->m_verticesLeft == 0);
    return *this;
}

inline Point Polygon::VertexIterator::yield()
{
    auto res = **this;
    ++*this;
    return res;
}


inline Polygon::
ConstVertexIterator::ConstVertexIterator(const Polygon &poly, int index)
: m_iter(poly.getArray()), m_verticesLeft(poly.vertexCount() - index) {}

inline Point Polygon::ConstVertexIterator::operator * () const
{
    return *this->m_iter;
}
inline Polygon::ConstVertexIterator& 
Polygon::ConstVertexIterator::operator ++ () &
{
    ++this->m_iter;
    --this->m_verticesLeft;
    this->setIsEnd(this->m_verticesLeft == 0);
    return *this;
}

inline Point Polygon::ConstVertexIterator::yield()
{
    auto res = **this;
    ++*this;
    return res;
}



inline Achibulup::IterRange<Polygon::VertexIterator> 
Polygon::vertices()
{
    return {this->verticesBegin()};
}
inline Achibulup::IterRange<Polygon::ConstVertexIterator> 
Polygon::vertices() const
{
    return {this->verticesBegin()};
}

inline Polygon::VertexIterator Polygon::verticesBegin()
{
    return VertexIterator(*this);
}
inline Polygon::ConstVertexIterator Polygon::verticesBegin() const
{
    return ConstVertexIterator(*this);
}
inline Achibulup::PMIIterator<Point> Polygon::v_verticesBegin() const
{
    return this->verticesBegin();
}




inline Circle::Circle()
{
    this->setRadius(0);
}
inline Circle::Circle(Point center, float radius)
{
    this->setCenter(center);
    this->setRadius(radius);
}
inline Box Circle::getBoundingBox() const noexcept
{
    auto center = this->getCenter();
    float radius = this->getRadius();
    return {{center.x - radius, center.y - radius}, 
            {radius * 2.f, radius * 2.f}};
}
inline Point Circle::getCenter() const noexcept 
{ 
    return this->m_center; 
}
inline float Circle::getRadius() const noexcept 
{ 
    return this->m_radius; 
}
inline void Circle::setCenter(Point center) noexcept 
{ 
    this->m_center = center;
}
/// throws std::invalid_argument if radius is negative
inline void Circle::setRadius(float radius)
{
    if (radius < 0) throwInvalidSetRadius(radius);
    this->m_radius = radius;
}






} // namespace LaserWave