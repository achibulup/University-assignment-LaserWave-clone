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

    template<typename S, typename ...Args>
    void pushState(Args&&...);

    void popState()
    {
        if (this->empty()) 
          throw std::runtime_error("StateMachine::popState(): empty stack");
        m_states.pop();
    }

    void clearStates()
    {
        this->m_states.clear();
    }

    // void pushState(State::Id id);

    void addRequest(StateRequest request)
    {
        this->m_requests.push_back(std::move(request));
    }

    void processRequests();

    State& getTopState()
    {
        if (this->empty())
            throw std::runtime_error("StateMachine::getTopState(): empty stack");
        return this->m_states.top();
    }

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
    void processRequest(const StateRequest &request);

    Achibulup::PolymorphicStack<State> m_states;
    GameDataRef m_data;
    List<StateRequest> m_requests;
};

template<typename S, typename ...Args>
void StateMachine::pushState(Args&& ...args)
{
    this->m_states.emplace<S>(this->m_data, static_cast<Args&&>(args)...);
}

} // namespace LaserWave

#endif // STATE_MACHINE_HPP_INCLUDED