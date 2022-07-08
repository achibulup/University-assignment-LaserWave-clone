#ifndef CONVEX_ENTITY_HPP_INCLUDED
#define CONVEX_ENTITY_HPP_INCLUDED

#include "Entity.hpp"

namespace LaserWave
{

void draw(sf::RenderTarget&, const IConvexPolygon&, 
          sf::Color, sf::RenderStates = {});

} // namespace LaserWave

#endif // CONVEX_ENTITY_HPP_INCLUDED