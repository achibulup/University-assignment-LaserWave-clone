#include "geometry_impl.hpp"

namespace LaserWave
{


float algebraicArea(const IPolygon &polygon)
{
    float area = 0;
    for (auto edge : getEdges(polygon))
      area += algebraicArea(edge);
    return area;
}

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
    if (!(std::min(line1.p1.x, line1.p2.x) <= std::max(line2.p1.x, line2.p2.x)
       && std::max(line1.p1.x, line1.p2.x) >= std::min(line2.p1.x, line2.p2.x)
       && std::min(line1.p1.y, line1.p2.y) <= std::max(line2.p1.y, line2.p2.y)
       && std::max(line1.p1.y, line1.p2.y) >= std::min(line2.p1.y, line2.p2.y)))
      return NO_INTERSECTION;
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
    if ((a1 < 0) != (a2 < 0) && (a3 < 0) != (a4 < 0)) return INTERSECT_CROSS;
    return NO_INTERSECTION;
}



}