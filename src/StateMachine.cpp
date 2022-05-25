#include "StateMachine.hpp"
#include "PlayingState.hpp"
#include "SplashState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"
#include <stdexcept>

namespace LaserWave
{

StateMachine::StateMachine(GameDataRef data) : m_data(std::move(data)) {}

// void StateMachine::pushState(State::Id id)
// {
// ///TODO
//     switch (id) {
//       case SplashState::ID :
//         this->m_states.emplace<SplashState>(this->m_data);
//       break;
//       case MenuState::ID :
//         this->m_states.emplace<MenuState>(this->m_data);
//       break;
//       case PlayingState::ID :
//         this->m_states.emplace<PlayingState>(this->m_data);
//       break;
//       case PauseState::ID :
//         this->m_states.emplace<PauseState>(this->m_data);
//       break;
//       default:
//         throw std::runtime_error("Unknown state id");
//     }
// }




void StateMachine::processRequest(StateRequest request)
{
    request(*this);
}

} // namespace LaserWave