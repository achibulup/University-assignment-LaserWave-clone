#ifndef SPLASHSTATE_HPP_INCLUDED
#define SPLASHSTATE_HPP_INCLUDED

#include "State.hpp"
#include "constants.hpp"

namespace LaserWave
{

class SplashState : public State
{
  public:
    static const State::Id ID;

    explicit SplashState(GameDataRef data);
    SplashState(const SplashState&) = delete;
    void operator = (SplashState) = delete;
    ~SplashState();

    State::Id getId() const override
    {
        return ID;
    }

    // void init() override;
    std::vector<StateRequest> update(sf::Time dt, EventManager&) override;
    void render() const override;

  private:
    enum Phase {
      START_DELAY,
      FADE_IN,
      HANG,
      FADE_OUT,
      END_DELAY,
      QUICK_END
    };

    Phase m_phase = Phase::START_DELAY;
    sf::Uint8 m_alpha = 0;
    sf::Time m_phase_time = sf::Time::Zero;
    sf::Text m_text;
    sf::Sprite m_logo;
};

} // namespace LaserWave

#endif // SPLASHSTATE_HPP_INCLUDED