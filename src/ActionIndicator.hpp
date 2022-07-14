#ifndef ACTIONINDICATOR_HPP_INCLUDED
#define ACTIONINDICATOR_HPP_INCLUDED

#include "commons.hpp"
#include <SFML/Graphics.hpp>

namespace LaserWave
{

enum class PlayerAction
{
    NONE,
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
    ActionIndicator(List<DrumNote> pattern, sf::Time display_time,
        const sf::Texture *shoot_icon, const sf::Texture *kick_icon,
        sf::Time delay = sf::Time::Zero);

    void update(sf::Time, bool game_over = false);
    PlayerAction getPendingAction() const;
    void drawTo(sf::RenderTarget&, sf::RenderStates = {}) const;

  private:
    List<DrumNote> m_pattern;
    sf::Time m_displayTime;
    sf::Time m_timer = sf::Time::Zero;
    int m_iter = 0;
    List<DrumNote> m_displayedActions;
    PlayerAction m_pendingAction = PlayerAction::NONE;
    const sf::Texture *m_shootIcon, *m_kickIcon;
};

} // namespace LaserWave

#endif // ACTIONINDICATOR_HPP_INCLUDED