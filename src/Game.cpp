#include "Game.hpp"
#include "EventManager.hpp"
#include "SplashState.hpp"
#include "assets.hpp"
#include <fstream>
namespace LaserWave
{

Game::Game() : m_states(this->getData())
{
    this->initWindow();
    this->load();
    this->m_states.pushState(SplashState::ID);
}
Game::~Game() {}

void Game::run()
{
    sf::Clock clock;
    sf::Time lag_accumulator = sf::Time::Zero;
    try {
      while (!this->m_states.empty()) {
        sf::Time elapsed_time = clock.restart();
        lag_accumulator += elapsed_time;
        if (lag_accumulator > sf::seconds(0.25f)) {
          lag_accumulator = sf::seconds(0.25f);
        }
        while (lag_accumulator > this->TIME_PER_FRAME) {
          lag_accumulator -= this->TIME_PER_FRAME;
          EventManager events(this->getEvents());
          this->update(this->TIME_PER_FRAME, events);
        }
        if (this->m_states.empty()) break;
        this->updateScreen();
      }
    }
    catch (ExitGame) {}
    this->m_window.close();
}

void Game::initWindow()
{
///TODO
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    this->m_window.create(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), 
                          "LaserWave", 
                          sf::Style::Close | sf::Style::Fullscreen, settings);
    this->m_window.setVerticalSyncEnabled(true);
    sf::Image image;
    image.loadFromFile(ICON_PATH);
    this->m_window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}

void Game::load()
{
    loadAssets(this->m_asset);
}

EventManager Game::getEvents()
{
  ///TODO
    EventManager events(&this->m_window);
    if (events.isCloseRequested())
      throw ExitGame();
    return events;
}

void Game::update(sf::Time dt, EventManager &events)
{
    if (this->m_states.empty()) return;
    auto requests = this->getActiveState().update(dt, events);
    this->processChangeRequests(std::move(requests));
}

void Game::updateScreen()
{
    this->m_window.clear();
    for (int i = 0; i < this->m_states.size(); ++i) {
      auto &state = this->m_states[i];
      if (!state.isPaused()) {
        state.render();
      }
    }
    this->m_window.display();
}

State& Game::getActiveState()
{
    return this->m_states.getActiveState();
}

void Game::processChangeRequests(std::vector<StackRequest> requests)
{
    for (auto &request : requests) {
      switch (request.action) {
        case StackRequest::Action::PUSH:
          this->m_states.pushState(request.id);
          break;
        case StackRequest::Action::POP:
          this->m_states.popState();
          break;
        case StackRequest::Action::CLEAR:
          this->m_states.clearStates();
        break;
        case StackRequest::Action::QUIT:
          throw ExitGame();
        break;
        default:
          break;
      }
    }
    if (!requests.empty() && !this->m_states.empty())
      this->m_states.getActiveState().asTopState();
}

} // namespace LaserWave