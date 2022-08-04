#include "geometry_impl.hpp"
#include <iostream>

namespace LaserWave
{

std::ostream& operator << (std::ostream &out, const Box &box)
{
    out << "(" << box.getLeft() << ", " << box.getTop() << ")->(" << box.getRight() << ", " << box.getBottom() << ")";
    return out;
}

bool intersects(const LineSegment &line1, const LineSegment &line2)
{
    return intersectType(line1, line2) != NO_INTERSECTION;
}

bool intersects(const Box &box, const LineSegment &line)
{
    if (!intersects(box, boundingBox(line))) return false;
    if ((line.p1.x == line.p2.x)
      & (line.p1.y == line.p2.y)) return true;

    if (contains(box, line.p1) || contains(box, line.p2)) return true;

    int x1 = box.getLeft();
    int x2 = box.getRight();
    int y1 = box.getTop();
    int y2 = box.getBottom();

    auto v = line.p2 - line.p1;
    float dx1 = x1 - line.p1.x;
    float dx2 = x2 - line.p1.x;
    float dy1 = y1 - line.p1.y;
    float dy2 = y2 - line.p1.y;
    float dx1vy = dx1 * v.y;
    float dx2vy = dx2 * v.y;
    float dy1vx = dy1 * v.x;
    float dy2vx = dy2 * v.x;
    
    if (((line.p1.x != x1) & (line.p2.x != x1))
     && (line.p1.x < x1) != (line.p2.x < x1)
     && (dy1vx - dx1vy < 0) != (dy2vx - dx1vy < 0))
      return true;
    
    if (((line.p1.x != x2) & (line.p2.x != x2))
     && (line.p1.x < x2) != (line.p2.x < x2)
     && (dy1vx - dx2vy < 0) != (dy2vx - dx2vy < 0))
      return true;
    
    if (((line.p1.y != y1) & (line.p2.y != y1))
     && (line.p1.y < y1) != (line.p2.y < y1)
     && (dy1vx - dx1vy < 0) != (dy1vx - dx2vy < 0))
      return true;
      
    if (((line.p1.y != y2) & (line.p2.y != y2))
     && (line.p1.y < y2) != (line.p2.y < y2)
     && (dy2vx - dx1vy < 0) != (dy2vx - dx2vy < 0))
      return true;
    
    return false;
}



VertexBuffer::VertexBuffer(int size)
{
    this->resize(size);
}
VertexBuffer::VertexBuffer(const VertexBuffer &other)
{
    (*this) = other;
}
VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
{
    (*this) = std::move(other);
}
VertexBuffer& VertexBuffer::operator = (const VertexBuffer &other) &
{
    this->resize(other.size());
    std::copy_n(other.getArray(), other.size(), this->getArray());
    return *this;
}
VertexBuffer& VertexBuffer::operator = (VertexBuffer &&other) & noexcept
{
    if (other.useSmallOpt()) {
      std::copy_n(other.getArray(), other.size(), this->getArray());
      this->m_size = other.m_size;
    }
    else {
      if (this == &other) return *this;
      if (this->useSmallOpt()) {
        Achibulup::destroy(&this->m_static);
        Achibulup::construct(&this->m_dynamic, other.m_dynamic);
        this->m_type = DYNAMIC;
      }
      else {
        this->m_dynamic = other.m_dynamic;
      }
      this->m_size = other.m_size;

      Achibulup::destroy(&other.m_dynamic);
      Achibulup::construct(&other.m_static);
      other.m_type = STATIC;
      other.m_size = 0;
    }
    return *this;
}
VertexBuffer::~VertexBuffer()
{
    if (this->useSmallOpt()) Achibulup::destroy(&this->m_static);
    else Achibulup::destroy(&this->m_dynamic);
}
void VertexBuffer::resize(int size)
{
    if (this->useSmallOpt()) {
      if (size > SMALL_POLYGON_OPT_THRESHOLD) {
        Achibulup::destroy(&this->m_static);
        Achibulup::construct(&this->m_dynamic, size);
        this->m_type = DYNAMIC;
      }
    }
    else if (size > this->m_dynamic.size()) this->m_dynamic.resize(size);
    this->m_size = size;
}



const Shape::Id IPolygon::ID("Polygon");

Box IPolygon::getBoundingBox() const noexcept
{
    float top, left, bottom, right;
    auto first = this->getVertex(0);
    top = first.y; bottom = top;
    left = first.x; right = left;
    for (Point point : move(this->vertices())) {
      top = std::min(top, point.y);
      left = std::min(left, point.x);
      bottom = std::max(bottom, point.y);
      right = std::max(right, point.x);
    }
    return {left, top, right, bottom};
}

LineSegment IPolygon::getEdge(int index) const
{
    int vertices = this->vertexCount();
    if (index < 0 || index >= vertices) 
        throw std::out_of_range(Achibulup::stringFormat(
            "IPolygon::getEdge(int index): index out of range (the number of edges is ",
            vertices,"and index == ",index));
    int next_index = index + 1;
    if (next_index == vertices) next_index = 0;
    return LineSegment(this->getVertex(index), this->getVertex(next_index));
}

void IPolygon::copyVertices(Point *destination) const
{
    for (auto point : move(this->vertices()))
      *destination++ = point;
}

void IPolygon::throwInvalidIndex(int index) const
{
    throw std::out_of_range(Achibulup::stringFormat(
        "IPolygon::checkIndex(int index): index out of range (the number of vertices is ",
        this->vertexCount(),"and index == ",index));
}

class IPolygonVertexIterator : public Achibulup::IInputIterator<Point>
{
  public:
    explicit IPolygonVertexIterator(const IPolygon &polygon, int index = 0)
    : m_source(&polygon), m_index(index), m_end(polygon.vertexCount()) {}

    Point yield() override final
    {
        auto res = this->m_source->getVertex(this->m_index++);
        this->setIsEnd(this->m_index == this->m_end);
        return res;
    }

  private:
    const IPolygon *m_source;
    int m_index, m_end;
};

Achibulup::PMIIterator<Point> IPolygon::v_verticesBegin() const
{
    return IPolygonVertexIterator(*this);
}


void VertexList::create(const IPolygon &polygon)
{
    int vertices = polygon.vertexCount();
    this->m_buffer.resize(vertices + 1);
    auto arr = this->m_buffer.getArray();
    polygon.copyVertices(arr);
    arr[vertices] = arr[0];
}


const Shape::Id IConvexPolygon::ID("ConvexPolygon");


const Polygon::Id Polygon::ID("Polygon");

Polygon::Polygon(int n_vertices)
{
    if (n_vertices < 3) throw std::invalid_argument(Achibulup::stringFormat(
        "Polygon::Polygon(int n_vertices): Polygon must have at least 3 points (n_vertices == ",
        n_vertices));
    this->resize(n_vertices);
}
Polygon::Polygon(std::initializer_list<Point> points)
{
    int new_size = points.size();
    if (new_size < 3) throw std::invalid_argument(Achibulup::stringFormat(
        "Polygon::Polygon(std::initializer_list<Point>): Polygon must have at least 3 points (n_vertices == ",
        new_size));
    this->resize(new_size);
    std::copy_n(points.begin(), new_size, this->getArray());
}
Polygon::~Polygon() = default;

void Polygon::copyVertices(Point *destination) const
{
    std::copy_n(this->getArray(), this->vertexCount(), destination);
}

namespace {
class PolygonVertexIterator : public Achibulup::IInputIterator<Point>
{
  public:
    using iterator_category = std::forward_iterator_tag;

    explicit PolygonVertexIterator(const Polygon &poly, int index = 0)
    : m_iter(poly.getArray()), m_verticesLeft(poly.vertexCount() - index) {}

    Point operator * () const
    {
        return *this->m_iter;
    }
    PolygonVertexIterator& operator ++ () &
    {
        ++this->m_iter;
        --this->m_verticesLeft;
        this->setIsEnd(this->m_verticesLeft == 0);
        return *this;
    }

    Point yield() override final
    {
        auto res = **this;
        ++*this;
        return res;
    }

  private:
    const Point *m_iter;
    int m_verticesLeft;
};
}

Achibulup::PMIIterator<Point> Polygon::v_verticesBegin() const
{
    return PolygonVertexIterator(*this);
}



const Shape::Id Circle::ID("Circle");

void Circle::throwInvalidSetRadius(float radius) const
{
    throw std::invalid_argument(Achibulup::stringFormat(
        "Circle::setRadius(float radius): radius must be non-negative (radius == ",
        radius));
}




} // namespace LaserWave