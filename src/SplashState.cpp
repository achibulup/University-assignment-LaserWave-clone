#include "State.hpp"
#include "constants.hpp"

#include "constants.hpp"
#include "assets.hpp"
#include "GameMaths.hpp"
#include "randoms.hpp"
#include "stateRequests.hpp"
#include <SFML/Achibulup_extensions/Pixels.hpp>


namespace LaserWave
{

class SplashState : public State INIT_DEBUG_ID(SplashState)
{
  public:
    static const State::Id ID;

    explicit SplashState(GameDataRef data);
    SplashState(const SplashState&) = delete;
    void operator = (SplashState) = delete;
    ~SplashState();

    State::Id getId() const noexcept override
    {
        return ID;
    }

    // void init() override;
    void update(sf::Time dt, EventManager&) override;
    void render() const override;

  private:
    enum Phase {
      START_DELAY,
      FADE_IN,
      HANG,
      FADE_OUT,
      END_DELAY,
      QUICK_END
    };

    Phase m_phase = Phase::START_DELAY;
    sf::Uint8 m_alpha = 0;
    sf::Time m_phase_time = sf::Time::Zero;
    sf::Text m_text;
    sf::Sprite m_logo;
};
const State::Id SplashState::ID("SplashState");
const State::Id SPLASHSTATE_ID = SplashState::ID;


template StateRequest makePushRequest<SplashState>();



SplashState::SplashState(GameDataRef data)
: State(data),
  m_text(MADEWITH_MESSAGE, this->getAssets().getFont(MADEWITH_FONT), MADEWITH_FONT_SIZE), 
  m_logo(this->getAssets().getTexture(SFML_LOGO)) 
{
    this->m_logo.setScale({0.5f, 0.5f});
    sf::Vector2f logo_size = {
        this->m_logo.getGlobalBounds().width, 
        this->m_logo.getGlobalBounds().height
    };
    this->m_logo.setPosition(this->getWindow().getSize().x / 2.f - logo_size.x / 2.f, LOGO_Y);
    this->m_logo.setColor(sf::Color(255, 255, 255, 0));

    sf::Vector2f text_size = {
        m_text.getGlobalBounds().width, 
        m_text.getLocalBounds().height
    };
    this->m_text.setPosition(this->m_logo.getPosition().x, MADEWITH_Y);
    this->m_text.setFillColor(sf::Color(255, 255, 255, 0));
    
    this->getWindow().setMouseCursorVisible(false);
}

SplashState::~SplashState() = default;

void SplashState::update(sf::Time dt, EventManager &event)
{
    this->m_phase_time += dt;
    if (event.isMouseButtonPressed(sf::Mouse::Left) 
     && this->m_phase != Phase::QUICK_END)
      this->m_phase = Phase::QUICK_END;
    switch (this->m_phase) {
      case Phase::START_DELAY: {
        if (this->m_phase_time.asSeconds() >= SPLASH_SCREEN_START_DELAY) {
          this->m_phase = Phase::FADE_IN;
          this->m_phase_time = sf::Time::Zero;
        }
        else this->m_alpha = 0;
      }break;
      case Phase::FADE_IN: {
        if (this->m_phase_time.asSeconds() >= SPLASH_SCREEN_FADE_IN) {
          this->m_phase = Phase::HANG;
          this->m_phase_time = sf::Time::Zero;
        }
        else {
          float interpolation = this->m_phase_time.asSeconds() / SPLASH_SCREEN_FADE_IN;
          this->m_alpha = round(sfExt::COLOR_MAX * interpolation);
        }
      }break;
      case Phase::HANG: {
        if (this->m_phase_time.asSeconds() >= SPLASH_SCREEN_HANG) {
          this->m_phase = Phase::FADE_OUT;
          this->m_phase_time = sf::Time::Zero;
        }
        this->m_alpha = sfExt::COLOR_MAX;
      }break;
      case Phase::FADE_OUT: {
        if (this->m_phase_time.asSeconds() >= SPLASH_SCREEN_FADE_OUT) {
          this->m_phase = Phase::END_DELAY;
          this->m_phase_time = sf::Time::Zero;
        }
        else {
          float interpolation = this->m_phase_time.asSeconds() / SPLASH_SCREEN_FADE_OUT;
          this->m_alpha = round(sfExt::COLOR_MAX * (1.f - interpolation));
        }
      }break;
      case Phase::END_DELAY: {
        if (this->m_phase_time.asSeconds() >= SPLASH_SCREEN_END_DELAY) {
          this->addStateRequest(makePopRequest());
          this->addStateRequest(makePushRequest<MenuState>());
        }
        else this->m_alpha = 0;
      }break;
      case Phase::QUICK_END: {
        if (this->m_alpha > 12) {
          this->m_alpha -= 12;
        }
        else {
          this->m_phase = Phase::END_DELAY;
          this->m_phase_time = sf::Time::Zero;
        }
      }break;
    }
    this->m_text.setFillColor(MADEWITH_COLOR * sf::Color(255, 255, 255, this->m_alpha));
    this->m_logo.setColor(sf::Color(255, 255, 255, this->m_alpha));
}

void SplashState::render() const
{
    auto &target = this->getWindow();
    target.clear();
    target.draw(this->m_text);
    target.draw(this->m_logo);
}

} // namespace LaserWave