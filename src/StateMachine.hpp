#ifndef STATE_MACHINE_HPP_INCLUDED
#define STATE_MACHINE_HPP_INCLUDED

#include "State.hpp"
#include <SFML/Achibulup_dependencies/PolymorphicStack.hpp>

namespace LaserWave
{

class StateMachine
{
  public:
    StateMachine(GameDataRef);

    bool empty() const noexcept
    {
        return this->m_states.empty();
    }

    template<typename S>
    void pushState(S &&state);

    void popState();
    void clearStates()
    {
        this->m_states.clear();
    }

    void pushState(State::Id id);

    State& getActiveState();

    int size() const noexcept
    {
        return this->m_states.size();
    }

    State& operator [] (int index)
    {
        return this->m_states[index];
    }
    const State& operator [] (int index) const
    {
        return this->m_states[index];
    }

  private:
    Achibulup::PolymorphicStack<State> m_states;
    GameDataRef m_data;
};

template<typename S>
void StateMachine::pushState(S &&state)
{
    this->m_states.push(std::forward<S>(state));
}

} // namespace LaserWave

#endif // STATE_MACHINE_HPP_INCLUDED