#ifndef PLAYINGSTATE_HPP_INCLUDED
#define PLAYINGSTATE_HPP_INCLUDED

#include "State.hpp"
#include "Player.hpp"
#include "Enemies.hpp"
#include "BasicEnemy.hpp"
#include "GameClock.hpp"
#include "HealthBar.hpp"
#include "Particles.hpp"

namespace LaserWave
{

struct LaserEndPoint
{
    sf::Vector2f position;
    const Enemy *hitEnemy;
};

class PlayingState : public State
{
  public:
    static const State::Id ID = State::Id::PLAYING;

    PlayingState(GameDataRef data);
    PlayingState(const PlayingState&) = delete;
    void operator = (PlayingState) = delete;
    ~PlayingState();

    // void init() override;
    std::vector<StackRequest> update(sf::Time dt, EventManager &event) override;
    void render() const override;

    void asTopState() override;

  private:
    void randomlySpawnEnemy();
    void filterOffScreenEnemies();
    void gameOver();
    void processInput(sf::Time dt, EventManager &event);
    void requestPause();
    void requestRestart();
    void requestMenu();
    float getEnemySpawnProbability();
    sf::Vector2f getRandomSpawnPosition() const;
    void spawnEnemy();
    bool isFiltered(const Entity&) const;
    LaserEndPoint findLaserEndPoint(sf::Vector2f position, sf::Vector2f direction) const;
    sf::Vector2f checkPlayerTouchEdge();
    void playerShoot(sf::Vector2f direction);
    void playerKick(sf::Vector2f direction);
    void playerGetHit(sf::Vector2f direction);

    enum class GameState {
      PLAYING,
      PAUSED,
      OVER,
    }m_state = GameState::PLAYING;
    GameClock m_clock;
    Particles m_particles;
    HealthBar m_health_bar;
    Player m_player;
    Enemies m_enemies;
    const sf::Cursor *m_cursor;
    sf::Time m_enemy_spawn_timer;
    sf::Sound m_shoot_sound;
    sf::Sound m_kick_sound;
    sf::Sound m_hit_sound;
    sf::Time m_sound_timer = sf::Time::Zero;
    std::vector<StackRequest> m_requests;
};

} // namespace LaserWave

#endif // PLAYINGSTATE_HPP_INCLUDED