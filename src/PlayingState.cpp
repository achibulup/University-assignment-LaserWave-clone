#include "State.hpp"
#include "Player.hpp"
#include "Enemies.hpp"
#include "GameClock.hpp"
#include "HealthBar.hpp"
#include "Particles.hpp"
#include "ActionIndicator.hpp"
#include "GameMaths.hpp"
#include "assets.hpp"
#include "KickParticle.hpp"
#include "LaserBeam.hpp"

#include "stateRequests.hpp"
#include "constants.hpp"
#include "randoms.hpp"
#include "collisions.hpp"
#include "debug_utils.hpp"
#include "ScoreSystem.hpp"
#include <SFML/Audio.hpp>

#include <fstream>
extern bool show_hitbox;
extern std::ofstream log_file;

namespace LaserWave
{

struct LaserEndPoint
{
    Vec2 position;
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

    sf::String getPlayingTime() const;

  private:
    void processInput(sf::Time dt, EventManager &event);
    void randomlySpawnEnemy();
    void filterOffScreenEnemies();
    void checkPlayerCollisions();
    void action();
    void triggerGameOver();
    void requestPause();
    void requestRestart();
    void requestMenu();
    void playerShoot(Vec2 direction);
    void playerKick(Vec2 direction);
    void playerGetHit(Vec2 direction);
    void playHitSound();
    float getEnemySpawnProbability();
    Vec2 getRandomSpawnPosition() const;
    void spawnEnemy();
    bool isFiltered(const Entity&) const;
    LaserEndPoint castLaser(Vec2 position, Vec2 direction) const;
    Vec2 checkPlayerTouchEdge();

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
const State::Id PlayingState::ID("PlayingState");
const State::Id PLAYINGSTATE_ID = PlayingState::ID;


template StateRequest makePushRequest<PlayingState>();



PlayingState::PlayingState(GameDataRef data) 
: State(data),
  m_clock(GAMECLOCK_POS, &this->getAssets().getFont(GAMECLOCK_FONT)),
  m_player(Vec2(this->getWindow().getSize().x / 2.f, 
                        this->getWindow().getSize().y / 3.f)),
  m_enemies(),
  m_cursor(&this->getAssets().getCursor(PLAYING_CURSOR)),
  m_shootSound(this->getAssets().getSound(SHOOT_SOUND)),
  m_kickSound(this->getAssets().getSound(KICK_SOUND)),
  m_hitSound(this->getAssets().getSound(HIT_SOUND)),
  m_actionIndicator(PATTERN, sf::seconds(INDICATOR_DISPLAY_TIME),
      &this->getAssets().getTexture(SHOOT_ICON_IMAGE),
      &this->getAssets().getTexture(KICK_ICON_IMAGE)),
  m_gameoverText(GAMEOVER_TEXT, this->getAssets().getFont(GAMEOVER_FONT),
                  GAMEOVER_FONT_SIZE),
  m_gameoverSub1Text(GAMEOVER_SUBTITLE1, 
      this->getAssets().getFont(GAMEOVER_FONT), GAMEOVER_SUBTITLE_FONT_SIZE),
  m_gameoverSub2Text(GAMEOVER_SUBTITLE2, 
      this->getAssets().getFont(GAMEOVER_FONT), GAMEOVER_SUBTITLE_FONT_SIZE)
{
    this->m_healthBar.setHealth(this->m_player.health());
    this->m_shootSound.setVolume(50);

    this->m_gameoverText.setPosition({
        this->getWindow().getSize().x / 2 - this->m_gameoverText.getGlobalBounds().width / 2,
        GAMEOVER_Y
    });
    this->m_gameoverText.setFillColor(GAMEOVER_COLOR);
    this->m_gameoverSub1Text.setPosition({
        this->getWindow().getSize().x / 2 - this->m_gameoverSub1Text.getGlobalBounds().width / 2,
        GAMEOVER_SUBTITLE1_Y
    });
    this->m_gameoverSub1Text.setFillColor(GAMEOVER_COLOR);
    this->m_gameoverSub2Text.setPosition({
        this->getWindow().getSize().x / 2 - this->m_gameoverSub2Text.getGlobalBounds().width / 2,
        GAMEOVER_SUBTITLE2_Y
    });
    this->m_gameoverSub2Text.setFillColor(GAMEOVER_COLOR);
}
PlayingState::~PlayingState() = default;

void PlayingState::update(sf::Time dt, EventManager &event)
{//WHEN_DEBUG(Benchmarker bench("PlayingState::update");)
    if (this->m_state == GameState::PAUSED) return;

    if (this->m_state == GameState::PLAYING) {
      this->m_clock.add1Tick();
      this->m_enemySpawnTimer += dt;
      this->m_soundTimer += dt;
      this->m_actionIndicator.update(dt);
      this->m_player.update(dt);
      this->m_enemies.update(dt);
      this->m_particles.update(dt);

      this->randomlySpawnEnemy();
      this->filterOffScreenEnemies();

      this->m_enemies.filterDeadEnemies();

      this->checkPlayerCollisions();

      this->playHitSound();

      this->m_healthBar.setHealth(this->m_player.health());
      if (!this->m_player.isAlive()) {
        this->triggerGameOver();
      }

      this->action();
    }
    else if (this->m_state == GameState::OVER) {
      this->m_enemies.update(dt);
      this->m_particles.update(dt);
      this->m_actionIndicator.update(dt, true);
      this->filterOffScreenEnemies();
    }
    this->processInput(dt, event);
}

void PlayingState::render() const
{//WHEN_DEBUG(Benchmarker bench("PlayingState::render");)
    auto &target = this->getWindow();
    target.clear(BACKGROUND_COLOR);

    this->m_enemies.drawTo(target);

    if (this->m_player.isAlive())
      target.draw(this->m_player);

    this->m_particles.drawTo(target);

    target.draw(this->m_clock);
    target.draw(this->m_healthBar);

    this->m_actionIndicator.drawTo(target);

    if (this->m_state == GameState::OVER) {
      target.draw(this->m_gameoverText);
      target.draw(this->m_gameoverSub1Text);
      target.draw(this->m_gameoverSub2Text);
    }

    if (::show_hitbox)
      for (const auto *const &entity : Entity::allEntities())
        entity->showHitBoxTo(target);
}

void PlayingState::asTopState()
{
    if (this->m_state == GameState::PAUSED)
      this->m_state = GameState::PLAYING;
    this->getWindow().setMouseCursorVisible(true);
    this->getWindow().setMouseCursor(*this->m_cursor);
}

sf::String PlayingState::getPlayingTime() const
{
    return this->m_clock.toString();
}







template<typename> class CheckType;

void PlayingState::action()
{
    auto get_action = this->m_actionIndicator.getPendingAction();
    if (get_action == PlayerAction::SHOOT) {
      auto mouse_pos = Vec2(sf::Mouse::getPosition());
      Vec2 direction = mouse_pos - this->m_player.getCenter();
      direction = normalize(direction);
      this->playerShoot(direction);
    }
    else if (get_action == PlayerAction::KICK) {
      auto mouse_pos = Vec2(sf::Mouse::getPosition());
      Vec2 direction = mouse_pos - this->m_player.getCenter();
      direction = normalize(direction);
      this->playerKick(direction);
    }
}

void PlayingState::processInput(sf::Time dt, EventManager &event)
{
    auto &&events = event.getPendingEvents();
    if (this->m_state == GameState::PLAYING) {
      for (const auto &e : events) {
        if (e.type == sf::Event::KeyPressed) {
          if (e.key.code == sf::Keyboard::P 
           || e.key.code == sf::Keyboard::Escape)
            this->requestPause();
        }
        if (e.type == sf::Event::LostFocus) {
          this->requestPause();
        }
      }
    }

    else if (this->m_state == GameState::OVER) {
      for (const auto &e : events) {
        if (e.type == sf::Event::MouseButtonPressed) {
          auto click = e.mouseButton;
          if (click.button == sf::Mouse::Left) {
            this->requestRestart();
          }
          if (click.button == sf::Mouse::Right) {
            this->requestMenu();
          }
        }
      }
    }
}

void PlayingState::triggerGameOver()
{
    this->m_state = GameState::OVER;
    this->getScoreSystem().addScore(this->m_clock.toString());
}

void PlayingState::requestPause()
{
    this->m_state = GameState::PAUSED;
    this->addStateRequest(makePushRequest<PauseState>());
}

void PlayingState::requestRestart()
{          
    auto restart_request = makeRequests(makePopRequest(),
                                        makePushRequest<PlayingState>());
    this->addStateRequest(
        makePushRequest<WipeTransitionState>(std::move(restart_request)));
}

void PlayingState::requestMenu()
{
    this->addStateRequest(makePopRequest());
}

void PlayingState::filterOffScreenEnemies()
{
    for (auto &enemy : this->m_enemies) {
      if (this->isFiltered(enemy))
        kill(enemy);
    }
}

void PlayingState::randomlySpawnEnemy()
{
    if (decide(this->getEnemySpawnProbability()))
      this->spawnEnemy();
}


float PlayingState::getEnemySpawnProbability()
{
    float sin_time = sin(this->m_enemySpawnTimer.asSeconds() * 1.8);
    return 0.008 + 0.09 * std::pow(sqr(sin_time), 3);
}

void PlayingState::spawnEnemy()
{
    Vec2 spawn_pos = this->getRandomSpawnPosition();
    Vec2 player_position = this->m_player.getCenter();
    this->m_enemies.addRandomEnemy(spawn_pos, player_position);
}

void PlayingState::playerShoot(Vec2 direction)
{
    this->m_player.shoot(direction);
    Vec2 laser_start_pos 
        = this->m_player.getCenter() + direction * PLAYER_LASER_START_OFFSET;
    auto laser_end = this->castLaser(laser_start_pos, direction);
    if (laser_end.hitEnemy) 
      const_cast<Enemy&>(*laser_end.hitEnemy).getHit();
    this->m_particles.addParticle(makeUnique<LaserBeam>(
        laser_start_pos, laser_end.position));
    this->m_shootSound.play();
    this->m_soundTimer = sf::Time::Zero;
}

void PlayingState::playerKick(Vec2 direction)
{
    this->m_player.kick(direction);
    this->m_particles.addParticle(makeUnique<KickParticle>(
        this->m_player.getCenter(), toAngle(direction), 
        &this->getAssets().getTexture(KICK_PARTICLE_IMAGE)));
    this->m_kickSound.play();
    this->m_soundTimer = sf::Time::Zero;
}

void PlayingState::playerGetHit(Vec2 direction)
{
    this->m_player.getHit(direction);
    if (this->m_pendingHitSound < 1) ++this->m_pendingHitSound;
}

LaserEndPoint PlayingState::castLaser(
                              Vec2 position, Vec2 direction) const
{
    using LaserFront = RegularHitboxConvex;
    auto getHitEnemy = [&] (const LaserFront &laser_front) {
      auto laser_hitbox = BasicHitboxRef(laser_front);
      for (const Enemy &enemy : this->m_enemies) {
        if (collides(laser_hitbox, enemy.getHitbox()))
          return &enemy;
      }
      return static_cast<const Enemy*>(nullptr);
    };
    auto calcLaserFrontSize = [] (Vec2 direction, float laser_width) {
        return laser_width / 2;
    };
    auto outOfScreen = [&] (const LaserFront &laser_front) {
      const float TOP_Y = -ENEMY_FILTER_MARGIN.y;
      const float BOTTOM_Y = this->getWindow().getSize().y + ENEMY_SPAWN_MARGIN.y;
      const float LEFT_X = -ENEMY_FILTER_MARGIN.x;
      const float RIGHT_X = this->getWindow().getSize().x + ENEMY_FILTER_MARGIN.x;
      auto center = laser_front.getCenter();
      return center.x < LEFT_X
          || center.x > RIGHT_X
          || center.y < TOP_Y
          || center.y > BOTTOM_Y;
    };
    LaserFront laser_front(4);
    float front_size = calcLaserFrontSize(direction, LASER_WIDTH);
    laser_front.setRadius(front_size * 1.4f);
    laser_front.setCenter(position);
    laser_front.setAngle(toAngle(direction) + Angle::fromDegrees(45));

    while (!outOfScreen(laser_front)) {
      auto *enemy = getHitEnemy(laser_front);
      laser_front.translate(direction * 1.f);
      if (enemy)
        return LaserEndPoint{laser_front.getCenter() + 14.f * direction, enemy};
    }

    return {laser_front.getCenter(), nullptr};
}

bool PlayingState::isFiltered(const Entity &entity) const
{
    const float TOP_Y = -ENEMY_FILTER_MARGIN.y;
    const float BOTTOM_Y = this->getWindow().getSize().y + ENEMY_SPAWN_MARGIN.y;
    const float LEFT_X = -ENEMY_FILTER_MARGIN.x;
    const float RIGHT_X = this->getWindow().getSize().x + ENEMY_FILTER_MARGIN.x;
    return entity.getCenter().x < LEFT_X
        || entity.getCenter().x > RIGHT_X
        || entity.getCenter().y < TOP_Y
        || entity.getCenter().y > BOTTOM_Y;
}

Vec2 PlayingState::getRandomSpawnPosition() const
{
    ///get a random position outside the window
    const float TOP_Y = -ENEMY_FILTER_MARGIN.y;
    const float BOTTOM_Y = this->getWindow().getSize().y + ENEMY_FILTER_MARGIN.y;
    const float LEFT_X = -ENEMY_FILTER_MARGIN.x;
    const float RIGHT_X = this->getWindow().getSize().x + ENEMY_FILTER_MARGIN.x;

    ///decide wich side to spawn on
    enum Side : int {
        TOP_OR_LEFT,
        BOTTOM_OR_RIGHT
    };
    Side side = Side(randInt(0, 1));
    Vec2 res;
    switch (side) {
      case TOP_OR_LEFT:
        res = {LEFT_X, TOP_Y};
      break;
      case BOTTOM_OR_RIGHT:
        res = {RIGHT_X, BOTTOM_Y};
      break;
    }

    ///get a random position on the side
    float UNROLL_SIDES_LENGTH = (BOTTOM_Y - TOP_Y) + (RIGHT_X - LEFT_X);
    float pos = randFloat(0.f, UNROLL_SIDES_LENGTH, 4096);
    if (pos < (BOTTOM_Y - TOP_Y))
      res.y = TOP_Y + pos;
    else
      res.x = LEFT_X + (pos - (BOTTOM_Y - TOP_Y));

    return res;
}

Vec2 PlayingState::checkPlayerTouchEdge() 
{
    const float TOP_Y = 10;
    const float BOTTOM_Y = this->getWindow().getSize().y - 10;
    const float LEFT_X = 10;
    const float RIGHT_X = this->getWindow().getSize().x - 10;

    auto &&player_box = this->m_player.getHitbox().getBoundingBox();
    
    bool touch_left = player_box.getLeft() < LEFT_X;
    bool touch_right = player_box.getRight() > RIGHT_X;
    bool touch_top = player_box.getTop() < TOP_Y;
    bool touch_bottom = player_box.getBottom() > BOTTOM_Y;
      
    Vec2 direction = {0, 0};
    if (touch_left) direction += {1.f, 0.f};
    if (touch_right) direction += {-1.f, 0.f};
    if (touch_top) direction += {0.f, 1.f};
    if (touch_bottom) direction += {0.f, -1.f};
    if (direction != Vec2{0, 0}) {
      // ::log_file << player_box; 
      return normalize(direction);
    }
    return direction;
}

void PlayingState::checkPlayerCollisions()
{
    for (auto &enemy : this->m_enemies) {
      if (isColliding(this->m_player, enemy)) {
        // ::log_file << "Player got hit by enemy " << &enemy << std::endl;
        this->playerGetHit(normalize(this->m_player.getCenter() - enemy.getCenter())); 
      }
    }
  
    auto direction = this->checkPlayerTouchEdge();
    if (direction != Vec2(0, 0)) {
      // ::log_file << "Player touched edge" << std::endl;
      this->playerGetHit(direction);
    }
}

void PlayingState::playHitSound()
{
    if (this->m_pendingHitSound > 0 
      && this->m_soundTimer.asSeconds() > 0.3f) {
      this->m_hitSound.play();
      this->m_soundTimer = sf::Time::Zero;
      this->m_pendingHitSound--;
    }
}


} // namespace LaserWave