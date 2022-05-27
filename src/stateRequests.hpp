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
template<typename S>
StateRequest makePushRequestV()
{
    return [](StateMachine& machine)
    {
        machine.pushState<S>();
    };
}
template<typename S, typename Arg>
StateRequest makePushRequestV(Arg&& arg)
{
    return [arg = std::move(arg)](StateMachine& machine)
    {
        machine.pushState<S>(std::move(arg));
    };
}
template<typename S, typename Arg1, typename Arg2>
StateRequest makePushRequestV(Arg1 &&arg1, Arg2 &&arg2)
{
    return 
    [arg1 = std::move(arg1), arg2 = std::move(arg2)] (StateMachine& machine)
    {
        machine.pushState<S>(std::move(arg1), std::move(arg2));
    };
}
StateRequest makePopRequest();
StateRequest makeClearRequest();
StateRequest makePopPastRequest(State::Id id);
StateRequest makePopToRequest(State::Id id);

} // namespace LaserWave

#endif // STATEREQUEST_HPP_INCLUDED