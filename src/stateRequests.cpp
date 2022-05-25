#include "stateRequests.hpp"

namespace LaserWave
{

StateRequest makePopRequest()
{
    return [](StateMachine& machine)
    {
        machine.popState();
    };
}
StateRequest makeClearRequest()
{
    return [](StateMachine& machine)
    {
        machine.clearStates();
    };
}
StateRequest makePopPastRequest(State::Id id)
{
    return [id](StateMachine& machine)
    {
        while (machine.getTopState().getId() != id)
          machine.popState();
        machine.popState();
    };
}
StateRequest makePopToRequest(State::Id id)
{
    return [id](StateMachine& machine)
    {
        while (machine.getTopState().getId() != id)
          machine.popState();
    };

} 

}// namespace LaserWave