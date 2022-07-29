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

    inline static const sf::Time TIME_PER_UPDATE = sf::seconds(1.f / UPDATES_PER_SECOND);

  private:
    GameDataRef getData()
    {
        GameDataRef data{&m_window, &m_asset, &m_states, &m_scoreSystem};
        return data;
    }

    void initWindow();
    void load();
    void start();
    
    /// measure FPS of the system and calculate m_refreashPeriod.
    /// if the FPS is approximately UPDATES_PER_SECOND (60), 
    /// m_refreshPeriod is set to 0 and the game with try to sync updates with the screen.
    void measureFPS();
    bool isSyncedWithScreen() const;

    void runUnsynced();
    void runSynced();

    void update(sf::Time dt);
    void updateScreen();

    EventManager getEvents();
    void processChangeRequests(std::vector<StateRequest> requests);

    State& getActiveState();

    sf::RenderWindow m_window;
    AssetManager m_asset;
    StateMachine m_states;
    ScoreSystem m_scoreSystem;
    sf::Time m_refreshPeriod;
}; 

} // namespace LaserWave

#endif // GAME_HPP_INCLUDED