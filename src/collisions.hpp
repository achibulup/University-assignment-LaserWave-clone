#ifndef COLLISIONS_HPP_INCLUDED
#define COLLISIONS_HPP_INCLUDED

#include "Hitbox.hpp"

namespace LaserWave
{

bool contains(const IPolygon&, Point);
bool contains(const IConvexPolygon&, Point);
bool contains(const Circle&, Point);

bool intersects(const LineSegment&, const IPolygon&);
bool intersects(const LineSegment&, const IConvexPolygon&);
bool intersects(const LineSegment&, const Circle&);

bool intersects(const IPolygon&, const IPolygon&);
bool intersects(const IPolygon&, const IConvexPolygon&);
bool intersects(const IPolygon&, const Circle&);

bool intersects(const IConvexPolygon&, const IConvexPolygon&);
bool intersects(const IConvexPolygon&, const Circle&);

bool intersects(const Circle&, const Circle&);



inline bool intersects(const IPolygon&, const LineSegment&);
inline bool intersects(const IConvexPolygon&, const LineSegment&);
inline bool intersects(const Circle&, const LineSegment&);

inline bool intersects(const IConvexPolygon&, const IPolygon&);
inline bool intersects(const Circle&, const IPolygon&);
inline bool intersects(const Circle&, const IConvexPolygon&);


// bool borderIntersects(const Shape &lhs, const Shape &rhs);
bool intersects(const Shape &lhs, const Shape &rhs);





bool contains(const BasicHitboxRef<IPolygon>&, Point);
bool contains(const BasicHitboxRef<IConvexPolygon>&, Point);
bool contains(const BasicHitboxRef<Circle>&, Point);

bool intersects(const LineSegment&, const BasicHitboxRef<IPolygon>&);
bool intersects(const LineSegment&, const BasicHitboxRef<IConvexPolygon>&);
bool intersects(const LineSegment&, const BasicHitboxRef<Circle>&);

bool collides(const BasicHitboxRef<IPolygon>&, const BasicHitboxRef<IPolygon>&);
bool collides(const BasicHitboxRef<IPolygon>&, const BasicHitboxRef<IConvexPolygon>&);
bool collides(const BasicHitboxRef<IPolygon>&, const BasicHitboxRef<Circle>&);

bool collides(const BasicHitboxRef<IConvexPolygon>&, const BasicHitboxRef<IConvexPolygon>&);
bool collides(const BasicHitboxRef<IConvexPolygon>&, const BasicHitboxRef<Circle>&);

bool collides(const BasicHitboxRef<Circle>&, const BasicHitboxRef<Circle>&);



inline bool intersects(const BasicHitboxRef<IPolygon>&, const LineSegment&);
inline bool intersects(const BasicHitboxRef<IConvexPolygon>&, const LineSegment&);
inline bool intersects(const BasicHitboxRef<Circle>&, const LineSegment&);

inline bool collides(const BasicHitboxRef<IConvexPolygon>&, const BasicHitboxRef<IPolygon>&);
inline bool collides(const BasicHitboxRef<Circle>&, const BasicHitboxRef<IPolygon>&);
inline bool collides(const BasicHitboxRef<Circle>&, const BasicHitboxRef<IConvexPolygon>&);



bool collides(const HitboxRef&, const HitboxRef&);

} // namespace LaserWave

#include "collisions.inline.hpp"

#endif // COLLISIONS_HPP_INCLUDED