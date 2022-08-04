#include "Game.hpp"
#include "stateRequests.hpp"
#include "EventManager.hpp"
#include "assets.hpp"
#include "debug_utils.hpp"
#include "paths.hpp"
#include <thread>
#include <fstream>

extern std::ofstream log_file;

namespace LaserWave
{

Game::Game() : m_states(this->getData())
{
    this->initWindow();
    this->load();
}
Game::~Game() {}

void Game::run()
{
    this->measureFPS();
    try {
      this->start();
      if (this->isSyncedWithScreen())
        this->runSynced();
      else
        this->runUnsynced();
    }
    catch (ExitGame) {}
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
void Game::start()
{
    this->m_states.addRequest(makePushRequest<SplashState>());
    this->m_states.processRequests();
}



void Game::measureFPS()
{
    using namespace std::chrono;
    this->m_window.clear();
    this->m_window.display();
    this->m_window.clear();
    this->m_window.display();
    auto start = steady_clock::now();
    /// display 15 times to get average refresh period.
    for (int i = 0; i < 5; ++i) {
      this->m_window.clear();
      this->m_window.display();
    }
    auto end = steady_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end - start);
    this->m_refreshPeriod = sf::microseconds(duration.count() / 5);
    WHEN_DEBUG(::log_file << "FPS: " << 1.f / this->m_refreshPeriod.asSeconds() << std::endl;)
    float ratio = this->m_refreshPeriod / this->TIME_PER_UPDATE;
    if (0.95f < ratio && ratio < 1.05f)
      this->m_refreshPeriod = sf::Time::Zero;
}
bool Game::isSyncedWithScreen() const
{
    return this->m_refreshPeriod == sf::Time::Zero;
}

const sf::Time MAX_UPDATE_LAG = sf::seconds(0.1f);
void sleepFor(sf::Time time)
{
    sf::Clock clock;
    if (time > sf::seconds(0.002f))
      sf::sleep(time - sf::seconds(0.002f));
}

void Game::runUnsynced()
{
    LOG_IF_DEBUG("Game::runUnsynced()");
    sf::Clock update_clock;
    sf::Clock render_clock;
    sf::Time update_lag = this->TIME_PER_UPDATE / 2.f;

    while (!this->m_states.empty()) {
      auto time_elapsed = update_clock.restart();
      update_lag += time_elapsed;

      auto time_until_next_render = this->m_refreshPeriod * 0.8f - render_clock.getElapsedTime();
      if (time_until_next_render < sf::Time::Zero) {
        LOG_IF_DEBUG(this->updateScreen());
        render_clock.restart();
        continue;
      }

      update_lag = std::min(update_lag, MAX_UPDATE_LAG);

      while (update_lag >= this->TIME_PER_UPDATE) {
        LOG_IF_DEBUG(this->update(this->TIME_PER_UPDATE));
        update_lag -= this->TIME_PER_UPDATE;
      }

      time_until_next_render = this->m_refreshPeriod * 0.8f - render_clock.getElapsedTime();
      auto time_until_next_update = this->TIME_PER_UPDATE - update_lag;
      sf::Time sleep_time = std::min(time_until_next_update, time_until_next_render);
      sleepFor(sleep_time);
    }
}

void Game::runSynced()
{
    LOG_IF_DEBUG("Game::runSynced()");
    sf::Clock clock;

    while (!this->m_states.empty()) {
      /// lag will be approximately an integer multiple of TIME_PER_UPDATE (ideally 1).
      sf::Time lag = clock.restart(); 

      WHEN_DEBUG(if (lag > this->TIME_PER_UPDATE * 2.f)
          ::log_file << this->m_states.getTopState().m_id.getString() 
                      << " " << lag.asSeconds() << std::endl;)

      lag = std::min(lag, MAX_UPDATE_LAG);

      int frames_behind = std::round(lag / this->TIME_PER_UPDATE);
      if (frames_behind > 1) ::log_file << frames_behind;;
      WHEN_DEBUG(if (frames_behind > 1)
          ::log_file << "Lag: " << lag.asSeconds() << " " << frames_behind << std::endl;)
      while (frames_behind --> 1) 
        LOG_IF_DEBUG(this->update(this->TIME_PER_UPDATE));


      sf::Time update_time = clock.getElapsedTime();
      sf::Time sleep_time = this->TIME_PER_UPDATE * 0.5f - update_time;
      if (sleep_time > sf::Time::Zero)
        sleepFor(sleep_time);
      WHEN_DEBUG(else
        ::log_file << "!Lag: " << clock.getElapsedTime().asSeconds() << std::endl;)
      
      LOG_IF_DEBUG(this->update(this->TIME_PER_UPDATE));


      if (this->m_states.empty()) break;
      update_time = clock.getElapsedTime();
      sleep_time = this->TIME_PER_UPDATE * 0.8f - update_time;
      if (sleep_time > sf::Time::Zero)
        sleepFor(sleep_time);
      WHEN_DEBUG(else
        ::log_file << "!Lag: " << clock.getElapsedTime().asSeconds() << std::endl;)

      WHEN_DEBUG(if (clock.getElapsedTime() > this->TIME_PER_UPDATE * 0.9f)
          ::log_file << "!Lag: " << clock.getElapsedTime().asSeconds() << std::endl;)

      LOG_IF_DEBUG(this->updateScreen());
    }
}


EventManager Game::getEvents()
{
  ///TODO
    EventManager events(&this->m_window);
    if (events.isCloseRequested())
      throw ExitGame();
    return events;
}

void Game::update(sf::Time dt)
{
    EventManager events(this->getEvents());
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