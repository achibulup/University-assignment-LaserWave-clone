#ifndef STATEREQUEST_HPP_INCLUDED
#define STATEREQUEST_HPP_INCLUDED

#include "StateMachine.hpp"

namespace LaserWave
{

template<typename S, typename ...Args>
StateRequest makePushRequest(Args&& ...args)
{
    return [&](StateMachine& machine)
    {
        machine.pushState<S>(static_cast<Args&&>(args)...);
    };
}
StateRequest makePopRequest();
StateRequest makeClearRequest();
StateRequest makePopPastRequest(State::Id id);
StateRequest makePopToRequest(State::Id id);

} // namespace LaserWave

#endif // STATEREQUEST_HPP_INCLUDED