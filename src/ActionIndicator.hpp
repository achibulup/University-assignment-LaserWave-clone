#ifndef ACTIONINDICATOR_HPP_INCLUDED
#define ACTIONINDICATOR_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace LaserWave
{

enum class PlayerAction
{
    KICK,
    SHOOT
};

struct DrumNote
{
    PlayerAction type;
    float delay;
};

class ActionIndicator
{
  public:
    

    void update(sf::Time);
    void drawTo(sf::RenderTarget&, sf::RenderStates = {}) const;
  
  private:
    
};

} // namespace LaserWave

#endif // ACTIONINDICATOR_HPP_INCLUDED