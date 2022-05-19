#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include "EventManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace LaserWave
{
struct StackRequest;

class AssetManager;
class StateMachine;

struct GameDataRef
{
    sf::RenderWindow *window;
    const AssetManager *assets;
    StateMachine *states;
};

class State
{
public:
    enum class Id : int {NONE = 0, SPLASH, MENU, PLAYING, PAUSED, GAMEOVER};

    State(GameDataRef data) : m_data(data) {}
    virtual ~State() = default;

    // virtual void init() = 0;
    virtual std::vector<StackRequest> update(sf::Time dt, EventManager &event) = 0;
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

struct StackRequest
{
    enum Action
    {
        NONE,
        PUSH,
        POP,
        CLEAR,
        QUIT
    };

    StackRequest(Action action = Action::NONE, State::Id id = State::Id::NONE)
    noexcept : action(action), id(id) {}

    Action action = NONE;
    State::Id id;
};

} // namespace LaserWave

#endif // STATE_HPP_INCLUDED