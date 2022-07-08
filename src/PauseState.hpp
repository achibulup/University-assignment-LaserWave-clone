#ifndef PAUSESTATE_HPP_INCLUDED
#define PAUSESTATE_HPP_INCLUDED

#include "State.hpp"
#include "SimpleGUI.hpp"

namespace LaserWave
{

class PauseState : public State INIT_DEBUG_ID(PauseState)
{
  public:
    static const State::Id ID;

    explicit PauseState(GameDataRef data);
    PauseState(const PauseState&) = delete;
    void operator = (PauseState) = delete;
    ~PauseState();

    virtual State::Id getId() const noexcept override
    {
        return ID;
    }

    void update(sf::Time dt, EventManager &event) override;
    void render() const override;

    void asTopState() override;

  private:
    const sf::Cursor *m_cursor;
    SimpleGUI m_GUI;
    sf::Text m_pause_text;
};


} // namespace Laserwave

#endif // PAUSESTATE_HPP_INCLUDED