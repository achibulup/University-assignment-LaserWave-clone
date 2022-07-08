#ifndef GEOMETRY_IMPL_HPP_INCLUDED
#define GEOMETRY_IMPL_HPP_INCLUDED

#include "geometry.hpp"
#include "gamemaths.hpp"

namespace LaserWave
{
// struct Triangle
// {
//     Point p1, p2, p3;
// };

inline float algebraicArea(const LineSegment &line)
{
    return 0.5 * (line.p2.x - line.p1.x) * (line.p1.y + line.p2.y);
}

inline float algebraicArea(Point p1, Point p2, Point p3)
{
    return 0.5 * ((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y));
}

float algebraicArea(const IPolygon &polygon);

inline float area(const Circle &circle)
{
    return PI * sqr(circle.getRadius());
}

inline const int NO_INTERSECTION = 0;
inline const int INTERSECT_CROSS_TOUCH = 1;
inline const int INTERSECT_CROSS = 2;
inline const int INTERSECT_OVERLAP = 4;

inline const int UNDEFINED_ANGLE = -1;
inline const int ZERO_ANGLE = 0;
inline const int ACUTE_ANGLE = 1;
inline const int RIGHT_ANGLE = 2;
inline const int OBTUSE_ANGLE = 3;
inline const int STRAIGHT_ANGLE = 4;

inline float sqrLength(sf::Vector2f v)
{
    return sqr(v.x) + sqr(v.y);
}

/// checks if a point is on a line segment, assuming the point is already on the line
bool unsafeOnSeg(const Point &p, const LineSegment &line);

int angleType(Point p1, Point root, Point p2);

int intersectType(const LineSegment line1, const LineSegment line2);

} //namespace LaserWave

#endif // GEOMETRY_IMPL_HPP_INCLUDED