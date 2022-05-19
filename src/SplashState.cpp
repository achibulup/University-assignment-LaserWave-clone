#include "SplashState.hpp"
#include "constants.hpp"
#include "assets.hpp"
#include "PlayingState.hpp"
#include "MenuState.hpp"
#include "GameMaths.hpp"
#include "randoms.hpp"
#include <SFML/Achibulup_extensions/Pixels.hpp>

namespace LaserWave
{

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
    this->m_text.setColor(sf::Color(255, 255, 255, 0));
    
    this->getWindow().setMouseCursorVisible(false);
}

SplashState::~SplashState() = default;

std::vector<StackRequest> SplashState::update(sf::Time dt, EventManager &event)
{
    std::vector<StackRequest> requests;
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
          requests.push_back({StackRequest::POP});
          requests.push_back({StackRequest::PUSH, MenuState::ID});
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
    this->m_text.setColor(MADEWITH_COLOR * sf::Color(255, 255, 255, this->m_alpha));
    this->m_logo.setColor(sf::Color(255, 255, 255, this->m_alpha));
    return requests;
}

void SplashState::render() const
{
    auto &target = this->getWindow();
    target.clear();
    target.draw(this->m_text);
    target.draw(this->m_logo);
}

} // namespace LaserWave