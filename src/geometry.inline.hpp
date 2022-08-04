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


inline int VertexBuffer::size() const noexcept
{
    return this->m_size; 
}
inline Point* VertexBuffer::getArray() noexcept
{
    if (this->useSmallOpt()) return this->m_static.data();
    return this->m_dynamic.data();
}
inline const Point* VertexBuffer::getArray() const noexcept
{
    return const_cast<VertexBuffer&>(*this).getArray();
}
inline bool VertexBuffer::useSmallOpt() const noexcept
{
    return m_type == STATIC;
}




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




inline VertexList::VertexList() = default;
inline VertexList::VertexList(const IPolygon &polygon)
: VertexList()
{
    this->create(polygon);
}
inline int VertexList::size() const noexcept
{
    return this->m_buffer.size() - 1;
}
inline VertexList::operator const Point* () const noexcept
{
    return this->m_buffer.getArray();
}
inline const Point* VertexList::begin() const noexcept
{
    return this->m_buffer.getArray();
}
inline const Point* VertexList::end() const noexcept
{
    return this->begin() + this->size();
}
inline Achibulup::IterRange<VertexList::EdgeIterator> 
VertexList::edges() const noexcept
{
    return {{this->begin()}, {this->end()}};
}

inline VertexList::EdgeIterator::EdgeIterator(const Point *p)
: m_p(p) {}
inline LineSegment VertexList::EdgeIterator::operator * () const
{
    return {*this->m_p, *(this->m_p + 1)};
}
inline VertexList::EdgeIterator& VertexList::EdgeIterator::operator ++ () &
{
    ++this->m_p;
    return *this;
}
inline bool VertexList::EdgeIterator::operator == (const EdgeIterator &rhs) const
{
    return this->m_p == rhs.m_p;
}
inline bool VertexList::EdgeIterator::operator != (const EdgeIterator &rhs) const
{
    return !(*this == rhs);
}


inline EdgeList::EdgeList() = default;
inline EdgeList::EdgeList(const IPolygon &polygon)
: EdgeList()
{
    this->create(polygon);
}
inline EdgeList::EdgeList(VertexList vertex_list)
: m_vertices(std::move(vertex_list)) {}

inline void EdgeList::create(const IPolygon &polygon)
{
    this->m_vertices = VertexList(polygon);
}
inline int EdgeList::size() const noexcept
{
    return this->m_vertices.size();
}
inline LineSegment EdgeList::operator [] (int index) const
{
    return {this->m_vertices[index], this->m_vertices[index + 1]};
}
inline VertexList::EdgeIterator EdgeList::begin() const noexcept
{
    return {this->m_vertices.begin()};
}
inline VertexList::EdgeIterator EdgeList::end() const noexcept
{
    return {this->m_vertices.end()};
}






inline VertexList getVertices(const IPolygon &polygon)
{
    return VertexList(polygon);
}
inline EdgeList getEdges(const IPolygon &polygon)
{
    return EdgeList(polygon);
}




inline Polygon::Polygon() : Polygon(3) {}
inline int Polygon::vertexCount() const noexcept
{
    return this->m_buffer.size();
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
inline Point* Polygon::getArray() noexcept
{
    return this->m_buffer.getArray();
}
inline const Point* Polygon::getArray() const noexcept
{
    return const_cast<Polygon&>(*this).getArray();
}
inline void Polygon::resize(int n_vertices)
{
    this->m_buffer.resize(n_vertices);
}
inline Achibulup::IterRange<Point*> Polygon::vertices()
{
    auto begin = this->getArray();
    return {begin, begin + this->vertexCount()};
}
inline Achibulup::IterRange<const Point*> Polygon::vertices() const
{
    auto begin = this->getArray();
    return {begin, begin + this->vertexCount()};
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