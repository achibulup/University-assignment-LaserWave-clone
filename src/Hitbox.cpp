#include "Hitbox.hpp"

namespace LaserWave
{

void StaticHitboxConvexBase::init(Point *array, int vertices, Point *args, int argc)
{
    if (argc > vertices)
      throw std::invalid_argument(Achibulup::stringFormat(
          "HitboxConvex::HitboxConvex(std::initializer_list<Point>): too many vertices (convex is ",
          vertices," points (n_vertices == ",argc));

    std::copy_n(args, argc, array);
}



const Shape::Id RegularHitboxConvex::ID("RegularHitboxConvex");

RegularHitboxConvex::RegularHitboxConvex(int n_vertices, Point center, 
                                         float radius, Angle angle) 
: HitboxConvex(center)
{
    this->setVertexCount(n_vertices);
    this->setRadius(radius);
    this->setAngle(angle);
}
void RegularHitboxConvex::setVertexCount(int n_vertices)
{
    if (n_vertices < 3) this->throwInvalidVertexCount(n_vertices);
    Angle part = Angle::fullCycle() / n_vertices;
    this->m_transformCache = toDirection(part);
    this->m_nVertices = n_vertices;
}

Vec2 RegularHitboxConvex::getDirection(int index) const
{
    if (index == 0) return this->m_firstDirCache;
    Vec2 transform = {1, 0};
    Vec2 tmp = this->m_transformCache;
    while (index > 0) {
      if (index % 2 == 1) transform = rotateTransform(transform, tmp);
      index /= 2;
      if (index) tmp = rotateTransform(tmp, tmp);
    }
    return rotateTransform(this->m_firstDirCache, transform);
}

void RegularHitboxConvex::throwInvalidVertexCount(int n_vertices) const
{
    throw std::invalid_argument(Achibulup::stringFormat(
        "setVertexCount(int n_vertices): a polygon should have at least 3 vertices (n_vertices == ",
        n_vertices,")"));
}
void RegularHitboxConvex::throwInvalidRadius(float radius) const
{
    throw std::invalid_argument(Achibulup::stringFormat(
        "RegularHitboxConvex::setRadius(float): radius must be >= 0 (radius == ",
        radius,")"));
}

} // namespace LaserWave