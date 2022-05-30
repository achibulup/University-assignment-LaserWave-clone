#include "geometry.hpp"
#include "gamemaths.hpp"

namespace LaserWave
{

float length(LineSegment line)
{
    return abs(line.p1 - line.p2);
}

const Shape::Id IPolygon::ID = "IPolygon";

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
    for (int i = 0; i < vertices; ++i)
      res[i] = this->getVertex(i);
    return res;
}

void IPolygon::checkIndex(int index) const
{
    if (index < 0 || index >= this->vertexCount()) 
        throw std::out_of_range(Achibulup::stringFormat(
            "IPolygon::checkIndex(int index): index out of range (the number of vertices is ",
            this->vertexCount(),"and index == ",index));
}


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
    for (int i = 0; i < points.size(); ++i)
      this->vertex(i) = points.begin()[i];
}

int Polygon::vertexCount() const noexcept
{
    return this->m_nVertices;
}

Point& Polygon::vertex(int index)
{
    this->checkIndex(index);
    if (index < SMALL_POLYGON_OPT_THRESHOLD)
      return this->m_reservedVertices[index];
    else
      return this->m_remainingVertices[index - SMALL_POLYGON_OPT_THRESHOLD];
}
const Point& Polygon::vertex(int index) const
{
    return const_cast<Polygon*>(this)->vertex(index);
}

void Polygon::resize(int n_vertices)
{
    this->m_nVertices = n_vertices;
    if (n_vertices < SMALL_POLYGON_OPT_THRESHOLD)
      this->m_remainingVertices.clear();
    else
      this->m_remainingVertices.resize(n_vertices - SMALL_POLYGON_OPT_THRESHOLD);
}

void Polygon::addVertex(Point vertex)
{
    if (this->m_nVertices < SMALL_POLYGON_OPT_THRESHOLD)
      this->m_reservedVertices[this->m_nVertices] = vertex;
    else
      this->m_remainingVertices.push_back(vertex);
    ++this->m_nVertices;
}



const Shape::Id Circle::ID = "Circle";
void Circle::setRadius(float radius)
{
    if (radius < 0) throw std::invalid_argument(Achibulup::stringFormat(
        "Circle::setRadius(float radius): radius must be non-negative (radius == ",
        radius));
    this->m_radius = radius;
}



// struct Triangle
// {
//     Point p1, p2, p3;
// };

float algebraicArea(const LineSegment &line)
{
    return 0.5 * (line.p2.x - line.p1.x) * (line.p1.y + line.p2.y);
}

float algebraicArea(Point p1, Point p2, Point p3)
{
    return 0.5 * ((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y));
}

float algebraicArea(const IPolygon &polygon)
{
    float area = 0;
    int vertices = polygon.vertexCount();
    for (int i = 0; i < vertices; ++i)
      area += algebraicArea(polygon.getEdge(i));
    return area;
}

float area(const Circle &circle)
{
    return PI * sqr(circle.getRadius());
}

const int NO_INTERSECTION = 0;
const int INTERSECT_CROSS_TOUCH = 1;
const int INTERSECT_CROSS = 2;
const int INTERSECT_OVERLAP = 4;

const int UNDEFINED_ANGLE = -1;
const int ZERO_ANGLE = 0;
const int ACUTE_ANGLE = 1;
const int RIGHT_ANGLE = 2;
const int OBTUSE_ANGLE = 3;
const int STRAIGHT_ANGLE = 4;

float sqrLength(sf::Vector2f v)
{
    return sqr(v.x) + sqr(v.y);
}

/// checks if a point is on a line segment, assuming the point is already on the line
bool unsafeOnSeg(const Point &p, const LineSegment &line)
{
    return std::min(line.p1.x, line.p2.x) <= p.x && p.x <= std::max(line.p1.x, line.p2.x)
        && std::min(line.p1.y, line.p2.y) <= p.y && p.y <= std::max(line.p1.y, line.p2.y);
}

int angleType(Point p1, Point root, Point p2)
{
    if (p1 == root || p2 == root) return UNDEFINED_ANGLE;
    auto v1 = p1 - root;
    auto v2 = p2 - root;
    auto in_product = v1.x * v2.x + v1.y * v2.y;
    auto out_product = v1.x * v2.y - v1.y * v2.x;
    if (out_product == 0) {
      if (in_product == 0) return ZERO_ANGLE;
      return STRAIGHT_ANGLE;
    }
    if (in_product > 0) return ACUTE_ANGLE;
    if (in_product < 0) return OBTUSE_ANGLE;
    return RIGHT_ANGLE;
}

int intersectType(const LineSegment line1, const LineSegment line2)
{
    float a1 = algebraicArea(line1.p1, line1.p2, line2.p1);
    float a2 = algebraicArea(line1.p1, line1.p2, line2.p2);
    if (a1 == 0 && a2 == 0) {
      if (unsafeOnSeg(line1.p1, line2) || unsafeOnSeg(line1.p2, line2) 
       || unsafeOnSeg(line2.p1, line1) || unsafeOnSeg(line2.p2, line1))
        return INTERSECT_OVERLAP;
      return NO_INTERSECTION;
    }
    if (a1 == 0 && unsafeOnSeg(line2.p1, line1)) return INTERSECT_CROSS_TOUCH;
    if (a2 == 0 && unsafeOnSeg(line2.p2, line1)) return INTERSECT_CROSS_TOUCH;
    float a3 = algebraicArea(line2.p1, line2.p2, line1.p1);
    if (a3 == 0 && unsafeOnSeg(line1.p1, line2)) return INTERSECT_CROSS_TOUCH;
    float a4 = algebraicArea(line2.p1, line2.p2, line1.p2);
    if (a4 == 0 && unsafeOnSeg(line1.p2, line2)) return INTERSECT_CROSS_TOUCH;
    if (a1 < 0 != a2 < 0 && a3 < 0 != a4 < 0) return INTERSECT_CROSS;
    return NO_INTERSECTION;
}



bool contains(const IPolygon &polygon, Point point)
{
    int vertices = polygon.vertexCount();
    float max_x = polygon.getVertex(0).x;
    for (int i = 1; i < vertices; ++i)
      max_x = std::max(max_x, polygon.getVertex(i).x);
    LineSegment ray(point, Point(max_x, point.y));
    int intersect_points = 0;
    for (int i = 0; i < vertices; ++i)
      intersect_points += intersectType(polygon.getEdge(i), ray);
    /// expects intersect_points to be even
    return intersect_points % 4 == 2;
}

bool contains(const Circle &circle, Point point)
{
    return sqrLength(point - circle.getCenter()) <= sqr(circle.getRadius());
}


bool intersects(const LineSegment &line1, const LineSegment &line2)
{
    return intersectType(line1, line2) != NO_INTERSECTION;
}

bool intersects(const IPolygon &poly1, const IPolygon &poly2)
{
    int vertices1 = poly1.vertexCount();
    int vertices2 = poly2.vertexCount();
    for (int i = 0; i < vertices1; ++i)
    for (int j = 0; j < vertices2; ++j)
      if (intersects(poly1.getEdge(i), poly2.getEdge(j)))
        return true;
    return contains(poly1, poly2.getVertex(0)) 
        || contains(poly2, poly1.getVertex(0));
}

bool intersects(const Circle &circle1, const Circle &circle2)
{
    return sqrLength(circle1.getCenter() - circle2.getCenter()) 
        <= sqr(circle1.getRadius() + circle2.getRadius());
}

bool intersects(const LineSegment &line, const IPolygon &poly)
{
    int vertices = poly.vertexCount();
    for (int i = 0; i < vertices; ++i)
      if (intersects(line, poly.getEdge(i))) return true;
    return contains(poly, line.p1);
}

bool intersects(const LineSegment &line, const Circle &circle)
{
    if (contains(circle, line.p1) || contains(circle, line.p2)) return true; 
    float area = algebraicArea(line.p1, line.p2, circle.getCenter());
    float hypot = area * 2 / length(line);
    if (hypot > circle.getRadius()) return false;
    return angleType(line.p1, line.p2, circle.getCenter()) <= RIGHT_ANGLE
        && angleType(line.p2, line.p1, circle.getCenter()) <= RIGHT_ANGLE;
}

bool intersects(const IPolygon &poly, const Circle &circle)
{
    int vertices = poly.vertexCount();
    for (int i = 0; i < vertices; ++i)
      if (intersects(poly.getEdge(i), circle)) return true;
    return contains(circle, poly.getVertex(0)) 
        || contains(poly, circle.getCenter());
}


bool intersects(const Shape &lhs, const Shape &rhs)
{
    Shape::Id lid = lhs.getId();
    Shape::Id rid = rhs.getId();
    if (lid == IPolygon::ID && rid == IPolygon::ID) return intersects(
        static_cast<const IPolygon&>(lhs), static_cast<const IPolygon&>(rhs));
    if (lid == Circle::ID && rid == Circle::ID) return intersects(
        static_cast<const Circle&>(lhs), static_cast<const Circle&>(rhs));
    if (lid == IPolygon::ID && rid == Circle::ID) return intersects(
        static_cast<const IPolygon&>(lhs), static_cast<const Circle&>(rhs));
    if (lid == Circle::ID && rid == IPolygon::ID) return intersects(
        static_cast<const Circle&>(lhs), static_cast<const IPolygon&>(rhs));
    throw std::runtime_error("Unsupported shape types");
}

} // namespace LaserWave