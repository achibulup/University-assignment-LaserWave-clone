#ifndef PAUSESTATE_HPP_INCLUDED
#define PAUSESTATE_HPP_INCLUDED

#include "State.hpp"
#include "SimpleGUI.hpp"

namespace LaserWave
{

class PauseState : public State
{
  public:
    static const State::Id ID = State::Id::PAUSED;

    PauseState(GameDataRef data);
    PauseState(const PauseState&) = delete;
    void operator = (PauseState) = delete;
    ~PauseState();

    std::vector<StackRequest> update(sf::Time dt, EventManager &event) override;
    void render() const override;

    void asTopState() override;

  private:
    const sf::Cursor *m_cursor;
    SimpleGUI m_GUI;
    std::vector<StackRequest> m_requests;
};


} // namespace Laserwave

#endif // PAUSESTATE_HPP_INCLUDED