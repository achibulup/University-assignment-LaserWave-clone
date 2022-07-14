#ifndef PLAYINGSTATE_HPP_INCLUDED
#define PLAYINGSTATE_HPP_INCLUDED

#include "State.hpp"
#include "Player.hpp"
#include "Enemies.hpp"
#include "GameClock.hpp"
#include "HealthBar.hpp"
#include "Particles.hpp"
#include "ActionIndicator.hpp"

#include <SFML/Audio.hpp>

namespace LaserWave
{

struct LaserEndPoint
{
    sf::Vector2f position;
    const Enemy *hitEnemy;
};

class PlayingState : public State INIT_DEBUG_ID(PlayingState)
{
  public:
    static const State::Id ID;

    explicit PlayingState(GameDataRef data);
    PlayingState(const PlayingState&) = delete;
    void operator = (PlayingState) = delete;
    ~PlayingState();

    State::Id getId() const noexcept override
    {
        return ID;
    }

    // void init() override;
    void update(sf::Time dt, EventManager &event) override;
    void render() const override;

    void asTopState() override;

  private:
    void randomlySpawnEnemy();
    void filterOffScreenEnemies();
    void checkPlayerCollisions();
    void playHitSound();
    void action();
    void processInput(sf::Time dt, EventManager &event);
    void requestPause();
    void requestRestart();
    void requestMenu();
    float getEnemySpawnProbability();
    sf::Vector2f getRandomSpawnPosition() const;
    void spawnEnemy();
    bool isFiltered(const Entity&) const;
    LaserEndPoint castLaser(sf::Vector2f position, sf::Vector2f direction) const;
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
    HealthBar m_healthBar;
    Player m_player;
    Enemies m_enemies;
    const sf::Cursor *m_cursor;
    sf::Time m_enemySpawnTimer;
    sf::Sound m_shootSound;
    sf::Sound m_kickSound;
    sf::Sound m_hitSound;
    sf::Time m_soundTimer = sf::Time::Zero;
    int m_pendingHitSound = 0;
    ActionIndicator m_actionIndicator;
    sf::Text m_gameoverText;
    sf::Text m_gameoverSub1Text;
    sf::Text m_gameoverSub2Text;
};

} // namespace LaserWave

#endif // PLAYINGSTATE_HPP_INCLUDED