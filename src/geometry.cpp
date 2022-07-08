#include "geometry_impl.hpp"

namespace LaserWave
{

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

List<Point> IPolygon::getVertices() const
{
    int vertices = this->vertexCount();
    List<Point> res(vertices);
    int index = 0;
    for (auto point : move(this->vertices()))
      res[index++] = point;
    return res;
}

void IPolygon::throwInvalidIndex(int index) const
{
    throw std::out_of_range(Achibulup::stringFormat(
        "IPolygon::checkIndex(int index): index out of range (the number of vertices is ",
        this->vertexCount(),"and index == ",index));
}

class IPolygon::VertexIterator : public Achibulup::IInputIterator<Point>
{
  public:
    explicit VertexIterator(const IPolygon &polygon, int index = 0)
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
    return VertexIterator(*this);
}


// IPolygon::EdgeIterator::EdgeIterator(const IPolygon &polygon)
// : m_vertexIter(polygon.v_verticesBegin())
// {
//     this->m_currentP1 = *this->m_vertexIter;
//     ++this->m_vertexIter;
//     this->m_currentP2 = *this->m_vertexIter;
//     this->m_first = this->m_currentP1;
// }

// LineSegment IPolygon::EdgeIterator::get() const
// {
//     return LineSegment(this->m_currentP1, this->m_currentP2);
// }

// void IPolygon::EdgeIterator::advance()
// {
//     if (this->m_vertexIter.isEnd()) {
//       this->m_vertexIter = {};
//       return;
//     }
//     this->m_currentP1 = this->m_currentP2;
//     ++this->m_vertexIter;
//     if (this->m_vertexIter.isEnd()) 
//       this->m_currentP2 = this->m_first;
//     else
//       this->m_currentP2 = *this->m_vertexIter;
// }

// bool IPolygon::EdgeIterator::isEnd() const noexcept
// {
//     return !this->m_vertexIter;
// }

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
    if (points.size() < 3) throw std::invalid_argument(Achibulup::stringFormat(
        "Polygon::Polygon(std::initializer_list<Point>): Polygon must have at least 3 points (n_vertices == ",
        points.size()));
    this->resize(points.size());
    auto iter = this->vertices().begin();
    for (int i = 0; i < points.size(); ++i) {
      *iter = points.begin()[i];
      ++iter;
    }
}

void Polygon::resize(int n_vertices)
{
    this->m_nVertices = n_vertices;
    if (n_vertices > SMALL_POLYGON_OPT_THRESHOLD 
     && n_vertices > this->m_list.size())
      this->m_list.resize(n_vertices);
}


const Shape::Id Circle::ID("Circle");

void Circle::throwInvalidSetRadius(float radius) const
{
    throw std::invalid_argument(Achibulup::stringFormat(
        "Circle::setRadius(float radius): radius must be non-negative (radius == ",
        radius));
}




} // namespace LaserWave