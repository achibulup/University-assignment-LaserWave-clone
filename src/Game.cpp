#include "Game.hpp"
#include "stateRequests.hpp"
#include "EventManager.hpp"
#include "assets.hpp"
#include "debug_utils.hpp"
#include "paths.hpp"
#include <fstream>

extern std::ofstream log_file;

namespace LaserWave
{

Game::Game() : m_states(this->getData())
{
    this->initWindow();
    this->load();
    this->m_states.addRequest(makePushRequest<SplashState>());
    this->m_states.processRequests();
}
Game::~Game() {}

void Game::run()
{
    sf::Clock clock;

    try {
      while (!this->m_states.empty()) {
        sf::Time lag = clock.restart();

        WHEN_DEBUG(if (lag > this->TIME_PER_FRAME * 2.f)
            ::log_file << this->m_states.getTopState().m_id.getString() 
                       << " " << lag.asSeconds() << std::endl;)

        if (lag > sf::seconds(0.15f)) {
          lag = sf::seconds(0.15f);
        }
        int frames_behind = std::round(lag / this->TIME_PER_FRAME);
        while (frames_behind--) {
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
    this->m_scoreSystem.load(std::string(SCORE_PATH));
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
    this->getActiveState().update(dt, events);
    this->m_states.processRequests();
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
    return this->m_states.getTopState();
}

void Game::processChangeRequests(std::vector<StateRequest> requests)
{
    this->m_states.processRequests();
}

} // namespace LaserWave