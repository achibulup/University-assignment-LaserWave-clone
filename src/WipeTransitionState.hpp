#ifndef WIPETRANSITIONSTATE_HPP_INCLUDED
#define WIPETRANSITIONSTATE_HPP_INCLUDED

#include "State.hpp"

namespace LaserWave
{

class WipeTransitionState : public State
{
  public:
    static const State::Id ID;

    WipeTransitionState(GameDataRef data, std::vector<StateRequest> after);

    State::Id getId() const override
    {
        return ID;
    }

    std::vector<StateRequest> update(sf::Time dt, EventManager&) override;
    void render() const override;

    void asTopState() override;

  private:
    sf::Texture m_capture;
    std::vector<StateRequest> m_after_requests;
    sf::Time m_timeleft;
};

} // namespace LaserWave

#endif // WIPETRANSITIONSTATE_HPP_INCLUDED