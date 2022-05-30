#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include "commons.hpp"
#include "EventManager.hpp"
#include "GameDataRef.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string_view>

namespace LaserWave
{

class State
{
  public:
    class Id : public std::string_view
    {
      public:
        using std::string_view::string_view;

        friend bool operator == (Id lhs, Id rhs)
        {
            return lhs.data() == rhs.data() && lhs.size() == rhs.size();
        }
        friend bool operator != (Id lhs, Id rhs)
        {
            return !(lhs == rhs);
        }
    };

    explicit State(GameDataRef data) : m_data(data) {}
    virtual ~State() = default;

    virtual Id getId() const = 0;

    // virtual void init() = 0;
    virtual void update(sf::Time dt, EventManager&) = 0;

    virtual void render() const = 0;

    void pause() { this->m_paused = true; }
    void resume() { this-> m_paused = false; }

    virtual void asTopState() {}

    virtual void onPause() {}
    virtual void onResume() {}

    bool isPaused() const { return this->m_paused; }

    // Context getContext() const;

  protected:
    sf::RenderWindow &getWindow() const { return this->m_data.getWindow(); }
    const AssetManager &getAssets() const { return this->m_data.getAssets(); }
    void addStateRequest(StateRequest request) const
    {
        this->m_data.addStateRequest(std::move(request));
    }

    GameDataRef m_data;

  private:
    bool m_paused = false;
};

template<typename ...Reqs>
List<StateRequest> makeRequests(Reqs... reqs)
{
    StateRequest *requests[] = { &reqs... };
    List<StateRequest> result;
    for (auto *req : requests)
        result.push_back(std::move(*req));
    return result;
}

} // namespace LaserWave

#endif // STATE_HPP_INCLUDED