#ifndef GAMEDATAREF_HPP_INCLUDED
#define GAMEDATAREF_HPP_INCLUDED

#include <functional>
#include <SFML/Graphics.hpp>

namespace LaserWave
{
   
class StateMachine;
class AssetManager;
class ScoreSystem;
using StateRequest = std::function<void(StateMachine&)>;

class GameDataRef
{
  public:
    GameDataRef(sf::RenderWindow* window, const AssetManager* assets, 
                StateMachine* states, ScoreSystem* scoreSystem);

    sf::RenderWindow& getWindow() const;
    const AssetManager& getAssets() const;
    void addStateRequest(StateRequest request) const;
    ScoreSystem& getScoreSystem() const;
  
  private:
    sf::RenderWindow* m_window;
    const AssetManager* m_assets;
    StateMachine* m_states;
    ScoreSystem* m_scoreSystem;
};

} // namespace LaserWave

#endif // GAMEDATAREF_HPP_INCLUDED