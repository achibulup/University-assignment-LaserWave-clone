#ifndef STATEREQUEST_HPP_INCLUDED
#define STATEREQUEST_HPP_INCLUDED

#include "StateMachine.hpp"
#include "gamestates.hpp"

namespace LaserWave
{

// template<typename S, typename ...Args>
// StateRequest makePushRequest(Args&& ...args)
// {
//     return [&](StateMachine& machine)
//     {
//         machine.pushState<S>(static_cast<Args&&>(args)...);
//     };
// }
template<typename S>
StateRequest makePushRequest()
{
    return [](StateMachine& machine)
    {
        machine.pushState<S>();
    };
}
template<typename S, typename Arg>
StateRequest makePushRequest(Arg&& arg)
{
    return [arg = std::move(arg)] (StateMachine& machine) mutable
    {
        machine.pushState<S>(std::move(arg));
    };
}
template<typename S, typename Arg1, typename Arg2>
StateRequest makePushRequest(Arg1 &&arg1, Arg2 &&arg2)
{
    return [arg1 = std::move(arg1), arg2 = std::move(arg2)]
    (StateMachine& machine) mutable
    {
        machine.pushState<S>(std::move(arg1), std::move(arg2));
    };
}




StateRequest makePopRequest();
StateRequest makeClearRequest();
StateRequest makePopPastRequest(State::Id id);
StateRequest makePopToRequest(State::Id id);


extern template StateRequest makePushRequest<SplashState>();
extern template StateRequest makePushRequest<MenuState>();
extern template StateRequest makePushRequest<PlayingState>();
extern template StateRequest makePushRequest<PauseState>();

extern template StateRequest 
makePushRequest<WipeTransitionState, std::vector<StateRequest>>(
    std::vector<StateRequest>&& after_transition);


} // namespace LaserWave

#endif // STATEREQUEST_HPP_INCLUDED