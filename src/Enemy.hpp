#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include "Entity.hpp"
#include <SFML/Achibulup_dependencies/common_utils.hpp>

namespace LaserWave
{

class Enemy : public Entity 
{
  public:
    virtual void getHit(sf::Vector2f direction = {}) = 0;
};

} // namespace LaserWave

#endif // ENEMY_HPP_INCLUDED