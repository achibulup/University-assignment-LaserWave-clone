#include "State.hpp"

#include "stateRequests.hpp"
#include "constants.hpp"
#include <iterator>

namespace LaserWave
{

class WipeTransitionState : public State INIT_DEBUG_ID(WipeTransitionState)
{
  public:
    static const State::Id ID;

    WipeTransitionState(GameDataRef data, std::vector<StateRequest> after);

    State::Id getId() const noexcept override { return ID; }

    void update(sf::Time dt, EventManager&) override;
    void render() const override;

    void asTopState() override;

  private:
    sf::Texture m_capture;
    std::vector<StateRequest> m_after_requests;
    sf::Time m_timeleft;
};
const State::Id WipeTransitionState::ID ("WipeTransitionState");
const State::Id WIPETRANSITIONSTATE_ID = WipeTransitionState::ID;


template StateRequest makePushRequest<WipeTransitionState, std::vector<StateRequest>>(std::vector<StateRequest>&&);



WipeTransitionState::WipeTransitionState(GameDataRef data, 
    std::vector<StateRequest> after)
: State(data), 
  m_timeleft(sf::seconds(TRANSITION_DURATION + TRANSITION_END_DELAY))
{
    this->m_capture.create(this->getWindow().getSize().x, this->getWindow().getSize().y);
    this->m_capture.update(this->getWindow());
    
    this->m_after_requests.push_back(makePopRequest());
    this->m_after_requests.insert(this->m_after_requests.end(), 
        std::move_iterator(after.begin()), std::move_iterator(after.end()));
}

void WipeTransitionState::update(sf::Time dt, EventManager&)
{
    m_timeleft -= dt;
    if (m_timeleft <= sf::Time::Zero) {
      for (auto &request : this->m_after_requests)
        this->addStateRequest(std::move(request));
    }
}

void WipeTransitionState::render() const
{
    if (this->m_timeleft.asSeconds() >= TRANSITION_END_DELAY) {
      // ------------------  
      // |////////        |
      // |///////         |
      // |//////          |
      // ------------------
      float interpolation = 1 - 
          (m_timeleft.asSeconds() - TRANSITION_END_DELAY) / TRANSITION_DURATION;

      float delta_x = this->getWindow().getSize().x * sin(WIPE_ANGLE);
      float top_y = -1;
      float bottom_y = this->getWindow().getSize().y + 1;
      float total_wipe_distance = this->getWindow().getSize().x + delta_x;
      float left_x = -delta_x - 1;
      float top_right_x = total_wipe_distance * interpolation;
      float bottom_right_x = top_right_x - delta_x;

      sf::ConvexShape cover(4);
      cover.setPoint(0, sf::Vector2f(left_x, top_y));
      cover.setPoint(1, sf::Vector2f(top_right_x, top_y));
      cover.setPoint(2, sf::Vector2f(bottom_right_x, bottom_y));
      cover.setPoint(3, sf::Vector2f(left_x, bottom_y));
      cover.setFillColor(sf::Color::Black);

      sf::Sprite old_screen(this->m_capture);

      this->getWindow().clear();
      this->getWindow().draw(old_screen);
      this->getWindow().draw(cover);
    }
    else {
      this->getWindow().clear(sf::Color::Black);
    }
}

void WipeTransitionState::asTopState()
{
    this->getWindow().setMouseCursorVisible(false);
}

} // namespace LaserWave