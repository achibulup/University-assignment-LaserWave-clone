#ifndef CONVEX_ENTITY_HPP_INCLUDED
#define CONVEX_ENTITY_HPP_INCLUDED

#include "Entity.hpp"

namespace LaserWave
{

class ConvexEntity
{
  public:
    virtual const IPolygon& getHitbox() const = 0;
    virtual sf::Color getColor() const noexcept = 0;

  protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates) const;
};

} // namespace LaserWave

#endif // CONVEX_ENTITY_HPP_INCLUDED