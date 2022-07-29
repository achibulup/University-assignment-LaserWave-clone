#include "ActionIndicator.hpp"
#include "gamemaths.hpp"
#include "constants.hpp"
#include "draw.hpp"

namespace LaserWave
{

ActionIndicator::ActionIndicator(List<DrumNote> pattern, sf::Time display_time,
    const sf::Texture *shoot_icon, const sf::Texture *kick_icon, sf::Time delay)
: m_pattern(pattern), m_displayTime(display_time), m_timer(-delay),
  m_shootIcon(shoot_icon), m_kickIcon(kick_icon) {}

void ActionIndicator::update(sf::Time dt, bool game_over)
{
    m_timer += dt;

    for (auto &&displayed : this->m_displayedActions)
      displayed.delay -= dt.asSeconds();
    if (!this->m_displayedActions.empty() 
     && this->m_displayedActions.front().delay <= 0) {
      this->m_pendingAction = this->m_displayedActions.front().type;
      this->m_displayedActions.erase(this->m_displayedActions.begin());
    }
    else this->m_pendingAction = PlayerAction::NONE;

    if (game_over) return;
    if (this->m_pattern.empty()) return;
    auto next_note = this->m_pattern[this->m_iter];
    auto next_action_delay = sf::seconds(next_note.delay);

    if (this->m_timer >= next_action_delay) {
      auto calc_time = this->m_displayTime - (this->m_timer - next_action_delay);
      this->m_displayedActions.push_back(
        DrumNote{next_note.type, calc_time.asSeconds()});

      this->m_timer -= next_action_delay;
      this->m_iter = (this->m_iter + 1) % this->m_pattern.size();
    }
}

PlayerAction ActionIndicator::getPendingAction() const
{
    return this->m_pendingAction;
}

void ActionIndicator::drawTo(sf::RenderTarget &target, 
                             sf::RenderStates states) const
{
    // float max_radius = abs(sf::Vector2f(target.getSize())) / 2.f + 2.f;
    // ring.setOutlineThickness(10.f);
    // ring.setFillColor({0, 0, 0, 0});
    for (auto &&action : this->m_displayedActions) {
      // draw a ring
      float interpolation = action.delay / this->m_displayTime.asSeconds();
      // ring.setRadius(max_radius * interpolation);
      // ring.setOrigin({ring.getRadius(), ring.getRadius()});
      // ring.setPosition(sf::Vector2f(target.getSize()) / 2.f);
      // ring.setPointCount(30 * interpolation + 30);
      sf::Sprite icon((action.type == PlayerAction::SHOOT) ?
          *this->m_shootIcon : *this->m_kickIcon);
      auto bound = icon.getGlobalBounds();
      icon.setOrigin(bound.width / 2.f, bound.height / 2.f);
      icon.setPosition(target.getSize().x * (1 - interpolation) / 2, target.getSize().y - 70.f);
      target.draw(icon, states);
      icon.setPosition(target.getSize().x * (1 + interpolation) / 2, target.getSize().y - 70.f);
      target.draw(icon, states);
    }
}

} // namespace LaserWave

