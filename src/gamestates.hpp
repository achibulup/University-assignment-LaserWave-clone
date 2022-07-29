#ifndef GAMESTATES_HPP_INCLUDED
#define GAMESTATES_HPP_INCLUDED

#include "State.hpp"

namespace LaserWave
{

class SplashState;
class MenuState;
class PlayingState;
class PauseState;
class LeaderBoardState;
class WipeTransitionState;


extern const State::Id SPLASHSTATE_ID;
extern const State::Id MENUSTATE_ID;
extern const State::Id PLAYINGSTATE_ID;
extern const State::Id PAUSESTATE_ID;
extern const State::Id LEADERBOARDSTATE_ID;
extern const State::Id WIPETRANSITIONSTATE_ID;

} // namespace LaserWave

#endif // GAMESTATES_HPP_INCLUDED