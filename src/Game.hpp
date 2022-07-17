#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "State.hpp"
#include "constants.hpp"
#include "StateMachine.hpp"
#include "AssetManager.hpp"
#include "ScoreSystem.hpp"
#include <SFML/Graphics.hpp>
#include <stack>
#include <vector>

namespace LaserWave
{

struct ExitGame{};



class Game
{
  public:
    Game();
    ~Game();

    void run();

    inline static const sf::Time TIME_PER_FRAME = sf::seconds(1.f / FPS);

  private:
    GameDataRef getData()
    {
        GameDataRef data{&m_window, &m_asset, &m_states, &m_scoreSystem};
        return data;
    }

    void initWindow();
    void load();

    void update(sf::Time dt, EventManager &events);
    void updateScreen();

    EventManager getEvents();
    void processChangeRequests(std::vector<StateRequest> requests);

    State& getActiveState();

    sf::RenderWindow m_window;
    AssetManager m_asset;
    StateMachine m_states;
    ScoreSystem m_scoreSystem;
}; 

} // namespace LaserWave

#endif // GAME_HPP_INCLUDED