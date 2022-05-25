#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include "EventManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string_view>
#include <functional>

namespace LaserWave
{

class AssetManager;
class StateMachine;

using StateRequest = std::function<void(StateMachine&)>;

struct GameDataRef
{
    sf::RenderWindow *window;
    const AssetManager *assets;
    StateMachine *states;
};

class State
{
public:
    using Id = std::string_view;

    explicit State(GameDataRef data) : m_data(data) {}
    virtual ~State() = default;

    virtual Id getId() const = 0;

    // virtual void init() = 0;
    virtual std::vector<StateRequest> update(sf::Time dt, EventManager&) = 0;

    virtual void render() const = 0;

    void pause() { this->m_paused = true; }
    void resume() { this-> m_paused = false; }

    virtual void asTopState() {}

    virtual void onPause() {}
    virtual void onResume() {}

    bool isPaused() const { return this->m_paused; }

    // Context getContext() const;

protected:
    sf::RenderWindow &getWindow() const { return *this->m_data.window; }
    const AssetManager &getAssets() const { return *this->m_data.assets; }
    StateMachine &getStates() const { return *this->m_data.states; }

    GameDataRef m_data;

private:
    bool m_paused = false;
};

template<typename ...Reqs>
std::vector<StateRequest> makeRequests(Reqs... reqs)
{
    StateRequest *requests[] = { &reqs... };
    std::vector<StateRequest> result;
    for (auto *req : requests)
        result.push_back(std::move(*req));
    return result;
}

} // namespace LaserWave

#endif // STATE_HPP_INCLUDED