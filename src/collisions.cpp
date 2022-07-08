#include "collisions.hpp"
#include "geometry_impl.hpp"

namespace LaserWave
{

bool contains(const IPolygon &polygon, Point point)
{
    return contains(BasicHitboxRef(polygon), point);
}
bool contains(const IConvexPolygon &polygon, Point point)
{
    return contains(BasicHitboxRef(polygon), point);
}
bool contains(const Circle &circle, Point point)
{
    return contains(BasicHitboxRef(circle), point);
}

bool intersects(const LineSegment &line, const IPolygon &polygon)
{
   return intersects(line, BasicHitboxRef(polygon));
}
bool intersects(const LineSegment &line, const IConvexPolygon &polygon)
{
   return intersects(line, BasicHitboxRef(polygon));
}
bool intersects(const LineSegment &line, const Circle &circle)
{
   return intersects(line, BasicHitboxRef(circle));
}


bool intersects(const IPolygon &lhs, const IPolygon &rhs)
{
    return collides(BasicHitboxRef(lhs), BasicHitboxRef(rhs));
}
bool intersects(const IPolygon &lhs, const IConvexPolygon &rhs)
{
    return collides(BasicHitboxRef(lhs), BasicHitboxRef(rhs));
}
bool intersects(const IPolygon &lhs, const Circle &rhs)
{
    return collides(BasicHitboxRef(lhs), BasicHitboxRef(rhs));
}
bool intersects(const IConvexPolygon &lhs, const IConvexPolygon &rhs)
{
    return collides(BasicHitboxRef(lhs), BasicHitboxRef(rhs));
}
bool intersects(const IConvexPolygon &lhs, const Circle &rhs)
{
    return collides(BasicHitboxRef(lhs), BasicHitboxRef(rhs));
}
bool intersects(const Circle &lhs, const Circle &rhs)
{
    return collides(BasicHitboxRef(lhs), BasicHitboxRef(rhs));
}


bool intersects(const Shape &lhs, const Shape &rhs)
{
    return collides(HitboxRef(lhs), HitboxRef(rhs));
}




bool contains(const BasicHitboxRef<IPolygon> &hitbox, Point point)
{
    Box bound = hitbox.getBoundingBox();
    if (!contains(bound, point)) return false;
    LineSegment ray;
    ray.p1 = point;
    if (bound.getRight() - point.x > point.x - bound.getLeft())
      ray.p2 = {bound.getLeft(), point.y};
    else ray.p2 = {bound.getRight(), point.y};
    int intersect_up = 0;
    for (auto edge : hitbox.getShape().edges()){     
      if (std::max(edge.p1.y, edge.p2.y) <= point.y
       || !intersects(boundingBox(edge), boundingBox(ray))
       || edge.p1.y == edge.p2.y) 
        continue;
      auto a1 = algebraicArea(edge.p1, edge.p2, ray.p1);
      auto a2 = algebraicArea(edge.p1, edge.p2, edge.p2);
      if (!(((a1 < 0) & (a2 < 0)) | ((a1 > 0) & (a2 > 0))))
        ++intersect_up;
    }
    return intersect_up % 2 == 1;
}

bool contains(const BasicHitboxRef<IConvexPolygon> &hitbox, Point point)
{
    if (!contains(hitbox.getBoundingBox(), point)) return false;
    enum State {NONE, CW, CCW} state = NONE;
    for (auto edge : hitbox.getShape().edges()) {
      auto aa = algebraicArea(point, edge.p1, edge.p2);
      if (aa > 0) {
        if (state == CW) return false;
        state = CCW;
      }
      else if (aa < 0) {
        if (state == CCW) return false;
        state = CW;
      }
    }
    return true;
}

bool contains(const BasicHitboxRef<Circle> &hitbox, Point point)
{
    auto &&circle = hitbox.getShape();
    return sqrLength(point - circle.getCenter()) <= sqr(circle.getRadius());
}



bool intersects(const LineSegment &line, const BasicHitboxRef<Circle> &hitbox)
{
    if (!intersects(boundingBox(line), hitbox.getBoundingBox())) return false;
    if (contains(hitbox, line.p1) || contains(hitbox, line.p2)) return true; 
    auto &&circle = hitbox.getShape();
    float area = algebraicArea(line.p1, line.p2, circle.getCenter());
    float hypot = area * 2 / length(line);
    if (hypot > circle.getRadius()) return false;
    return angleType(line.p1, line.p2, circle.getCenter()) <= RIGHT_ANGLE
        && angleType(line.p2, line.p1, circle.getCenter()) <= RIGHT_ANGLE;
}

bool intersects(const LineSegment &line, 
                const BasicHitboxRef<IConvexPolygon> &hitbox)
{
    if (!intersects(hitbox.getBoundingBox(), boundingBox(line))) return false;
    if (contains(hitbox, line.p1)) return true;
    for (auto edge : hitbox.getShape().edges())
      if (intersects(line, edge)) return true;
    return false;
}
bool intersects(const LineSegment &line, const BasicHitboxRef<IPolygon> &hitbox)
{
    if (!intersects(hitbox.getBoundingBox(), boundingBox(line))) return false;
    if (contains(hitbox, line.p1)) return true;
    for (auto edge : hitbox.getShape().edges())
      if (intersects(line, edge)) return true;
    return false;
}



bool collides(const BasicHitboxRef<IPolygon> &lhs, 
              const BasicHitboxRef<IPolygon> &rhs)
{
    if (!intersects(lhs.getBoundingBox(), rhs.getBoundingBox())) 
      return false;
    auto &&l_shape = lhs.getShape();
    auto &&r_shape = rhs.getShape();
    for (auto edge : l_shape.edges())
      if (intersects(edge, rhs)) return true;
    if (contains(lhs, r_shape.getVertex(0))
     || contains(rhs, l_shape.getVertex(0)))
      return true;
    return false;
}

bool collides(const BasicHitboxRef<IConvexPolygon> &lhs, 
              const BasicHitboxRef<IConvexPolygon> &rhs)
{
    if (!intersects(lhs.getBoundingBox(), rhs.getBoundingBox())) 
      return false;
    auto &&l_shape = lhs.getShape();
    auto &&r_shape = rhs.getShape();
    for (auto edge : l_shape.edges())
      if (intersects(edge, rhs)) return true;
    if ((contains(lhs, r_shape.getVertex(0)))
     || (contains(rhs, l_shape.getVertex(0))))
       return true;
    return false;
}

bool collides(const BasicHitboxRef<IPolygon> &lhs, 
                const BasicHitboxRef<IConvexPolygon> &rhs)
{
    if (!intersects(lhs.getBoundingBox(), rhs.getBoundingBox())) 
      return false;
    auto &&l_shape = lhs.getShape();
    auto &&r_shape = rhs.getShape();
    for (auto edge : l_shape.edges())
      if (intersects(edge, rhs)) return true;
    if (contains(lhs, r_shape.getVertex(0))
     || contains(rhs, l_shape.getVertex(0)))
      return true;
    return false;
}

bool collides(const BasicHitboxRef<Circle> &lhs, 
                const BasicHitboxRef<Circle> &rhs)
{
    auto &&l_shape = lhs.getShape();
    auto &&r_shape = rhs.getShape();
    return sqrLength(l_shape.getCenter() - r_shape.getCenter()) 
        <= sqr(l_shape.getRadius() + r_shape.getRadius());
}


bool collides(const BasicHitboxRef<IPolygon> &lhs, 
                const BasicHitboxRef<Circle> &rhs)
{
    if (!intersects(lhs.getBoundingBox(), rhs.getBoundingBox())) 
      return false;
    auto &&l_shape = lhs.getShape();
    auto &&r_shape = rhs.getShape();
    for (auto edge : l_shape.edges())
      if (intersects(edge, rhs)) return true;
    return contains(rhs, l_shape.getVertex(0)) 
        || contains(lhs, r_shape.getCenter());
}
bool collides(const BasicHitboxRef<IConvexPolygon> &lhs, 
                const BasicHitboxRef<Circle> &rhs)
{
    if (!intersects(lhs.getBoundingBox(), rhs.getBoundingBox())) 
      return false;
    auto &&l_shape = lhs.getShape();
    auto &&r_shape = rhs.getShape();
    for (auto edge : l_shape.edges())
      if (intersects(edge, rhs)) return true;
    return contains(rhs, l_shape.getVertex(0)) 
        || (contains(lhs, r_shape.getCenter()));
}




bool collides(const BasicHitboxRef<IPolygon> &lhs, const HitboxRef &rhs)
{
    auto box2 = rhs.getBoundingBox();
    auto *r_shape = &rhs.getShape();
    if (auto try_poly = dynamic_cast<const IConvexPolygon*>(r_shape)){
      auto poly = BasicHitboxRef(*try_poly, box2);
      return collides(lhs, poly);
    } 
    if (auto try_poly = dynamic_cast<const IPolygon*>(r_shape)){
      auto poly = BasicHitboxRef(*try_poly, box2);
      return collides(lhs, poly);
    }
    if (auto try_circle = dynamic_cast<const Circle*>(r_shape)){
      auto circle = BasicHitboxRef(*try_circle, box2);
      return collides(lhs, circle);
    }
    throw std::runtime_error("shape not supported");
}
bool collides(const BasicHitboxRef<IConvexPolygon> &lhs, const HitboxRef &rhs)
{
    auto box2 = rhs.getBoundingBox();
    auto *r_shape = &rhs.getShape();
    if (auto try_poly = dynamic_cast<const IConvexPolygon*>(r_shape)){
      auto poly = BasicHitboxRef(*try_poly, box2);
      return collides(lhs, poly);
    } 
    if (auto try_poly = dynamic_cast<const IPolygon*>(r_shape)){
      auto poly = BasicHitboxRef(*try_poly, box2);
      return collides(lhs, poly);
    }
    if (auto try_circle = dynamic_cast<const Circle*>(r_shape)){
      auto circle = BasicHitboxRef(*try_circle, box2);
      return collides(lhs, circle);
    }
    throw std::runtime_error("shape not supported");
}
bool collides(const BasicHitboxRef<Circle> &lhs, const HitboxRef &rhs)
{
    auto box2 = rhs.getBoundingBox();
    auto *r_shape = &rhs.getShape();
    if (auto try_poly = dynamic_cast<const IConvexPolygon*>(r_shape)){
      auto poly = BasicHitboxRef(*try_poly, box2);
      return collides(lhs, poly);
    } 
    if (auto try_poly = dynamic_cast<const IPolygon*>(r_shape)){
      auto poly = BasicHitboxRef(*try_poly, box2);
      return collides(lhs, poly);
    }
    if (auto try_circle = dynamic_cast<const Circle*>(r_shape)){
      auto circle = BasicHitboxRef(*try_circle, box2);
      return collides(lhs, circle);
    }
    throw std::runtime_error("shape not supported");
}


bool collides(const HitboxRef &lhs, const HitboxRef &rhs)
{
    auto box1 = lhs.getBoundingBox(), box2 = rhs.getBoundingBox();
    if (!intersects(box1, box2)) return false;
    auto *l_shape = &lhs.getShape();
    if (auto try_poly = dynamic_cast<const IConvexPolygon*>(l_shape)){
      auto poly = BasicHitboxRef(*try_poly, box1);
      return collides(poly, rhs);
    } 
    if (auto try_poly = dynamic_cast<const IPolygon*>(l_shape)){
      auto poly = BasicHitboxRef(*try_poly, box1);
      return collides(poly, rhs);
    }
    if (auto try_circle = dynamic_cast<const Circle*>(l_shape)){
      auto circle = BasicHitboxRef(*try_circle, box1);
      return collides(circle, rhs);
    }
    throw std::runtime_error("shape not supported");
}

} // namespace LaserWave