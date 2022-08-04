#ifndef BOSS_HPP_INCLUDED
#define BOSS_HPP_INCLUDED

#include "Enemy.hpp"

namespace LaserWave
{

class Boss : public Enemy 
{
  public:
    virtual int getHealth() const noexcept = 0;
};

} // namespace LaserWave

#endif // BOSS_HPP_INCLUDED