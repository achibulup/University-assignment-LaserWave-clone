#include "GameDataRef.hpp"
#include "StateMachine.hpp"

namespace LaserWave
{

GameDataRef::GameDataRef(sf::RenderWindow* window, const AssetManager* assets, 
            StateMachine* states, ScoreSystem *score_system)
: m_window(window), m_assets(assets), 
  m_states(states), m_scoreSystem(score_system) {}

sf::RenderWindow& GameDataRef::getWindow() const
{
    return *this->m_window;
}
const AssetManager& GameDataRef::getAssets() const
{
    return *this->m_assets;
}
void GameDataRef::addStateRequest(StateRequest request) const
{
    this->m_states->addRequest(std::move(request));
}
ScoreSystem& GameDataRef::getScoreSystem() const
{
    return *this->m_scoreSystem;
}

} // namespace LaserWave
