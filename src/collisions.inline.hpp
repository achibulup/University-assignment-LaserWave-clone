#include "collisions.hpp"

namespace LaserWave
{
  
inline bool intersects(const IPolygon &lhs, const LineSegment &rhs)
{ return intersects(rhs, lhs); }
inline bool intersects(const IConvexPolygon &lhs, const LineSegment &rhs)
{ return intersects(rhs, lhs); }
inline bool intersects(const Circle &lhs, const LineSegment &rhs)
{ return intersects(rhs, lhs); }
inline bool intersects(const IConvexPolygon &lhs, const IPolygon &rhs)
{ return intersects(rhs, lhs); }
inline bool intersects(const Circle &lhs, const IPolygon &rhs)
{ return intersects(rhs, lhs); }
inline bool intersects(const Circle &lhs, const IConvexPolygon &rhs)
{ return intersects(rhs, lhs); }



inline bool intersects(const BasicHitboxRef<IPolygon> &lhs, const LineSegment &rhs)
{ return intersects(rhs, lhs); }
inline bool intersects(const BasicHitboxRef<IConvexPolygon> &lhs, const LineSegment &rhs)
{ return intersects(rhs, lhs); }
inline bool intersects(const BasicHitboxRef<Circle> &lhs, const LineSegment &rhs)
{ return intersects(rhs, lhs); }
inline bool collides(const BasicHitboxRef<IConvexPolygon> &lhs, const BasicHitboxRef<IPolygon> &rhs)
{ return collides(rhs, lhs); }
inline bool collides(const BasicHitboxRef<Circle> &lhs, const BasicHitboxRef<IPolygon> &rhs)
{ return collides(rhs, lhs); }
inline bool collides(const BasicHitboxRef<Circle> &lhs, const BasicHitboxRef<IConvexPolygon> &rhs)
{ return collides(rhs, lhs); }

}