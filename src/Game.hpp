#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "State.hpp"
#include "constants.hpp"
#include "StateMachine.hpp"
#include "AssetManager.hpp"
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

  private:
    GameDataRef getData()
    {
        GameDataRef data;
        data.window = &m_window;
        data.assets = &m_asset;
        data.states = &m_states;
        return data;
    }

    void initWindow();
    void load();

    void update(sf::Time dt, EventManager &events);
    void updateScreen();

    EventManager getEvents();
    void processChangeRequests(std::vector<StackRequest> requests);

    State& getActiveState();

    inline static const sf::Time TIME_PER_FRAME = sf::seconds(1.f / FPS);

    sf::RenderWindow m_window;
    AssetManager m_asset;
    StateMachine m_states;
}; 

} // namespace LaserWave

#endif // GAME_HPP_INCLUDED