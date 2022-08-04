#include "State.hpp"
#include "Player.hpp"
#include "Enemies.hpp"
#include "GameClock.hpp"
#include "HealthBar.hpp"
#include "Particles.hpp"
#include "ActionIndicator.hpp"
#include "GameMaths.hpp"
#include "assets.hpp"

#include "stateRequests.hpp"
#include "constants.hpp"
#include "randoms.hpp"
#include "collisions.hpp"
#include "debug_utils.hpp"
#include "ScoreSystem.hpp"
#include "SimpleTextBox.hpp"
#include "SimpleGUI.hpp"
#include "ExplodeParticles.hpp"
#include "enemy_spawn.hpp"
#include "KickParticle.hpp"
#include "LaserBeam.hpp"
#include "FirstBoss.hpp"
#include "BasicEnemy.hpp"
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
    enum class GameState 
    { 
        PLAYING, 
        PAUSED, 
        OVER 
    };

    void processInput(sf::Time dt, EventManager &event);
    void randomlySpawnEnemy();
    void filterDeadEnemies();
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
    void addRandomEnemy(Point position, Point player_position);
    void spawnEnemy();
    bool isOffScreen(const Entity&) const;
    LaserEndPoint castLaser(Vec2 position, Vec2 direction) const;
    Vec2 checkPlayerTouchEdge();

    GameState m_state = GameState::PLAYING;
    GameClock m_clock;
    Particles m_particles;
    HealthBar m_healthBar;
    HealthBar m_bossHealthBar;
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
    SimpleTextBox m_nameTextbox;
};
const State::Id PlayingState::ID ("PlayingState");
const State::Id PLAYINGSTATE_ID = PlayingState::ID;


template StateRequest makePushRequest<PlayingState>();




PlayingState::PlayingState(GameDataRef data) 
: State (data),
  m_clock (GAMECLOCK_POS, &this->getAssets().getFont(GAMECLOCK_FONT)),
  m_healthBar (HEALTH_BAR_POSITION, HealthBar::Direction::RIGHT, PLAYER_COLOR),
  m_bossHealthBar (BOSS_HEALTH_BAR_POSITION, HealthBar::Direction::LEFT),
  m_player (Vec2(this->getWindow().getSize().x / 2.f, 
                this->getWindow().getSize().y / 3.f)),
  m_enemies (),
  m_cursor (&this->getAssets().getCursor(PLAYING_CURSOR)),
  m_shootSound (this->getAssets().getSound(SHOOT_SOUND)),
  m_kickSound (this->getAssets().getSound(KICK_SOUND)),
  m_hitSound (this->getAssets().getSound(HIT_SOUND)),

  m_actionIndicator (
      loadActionPattern(), 
      sf::seconds(INDICATOR_DISPLAY_TIME),
      &this->getAssets().getTexture(SHOOT_ICON_IMAGE),
      &this->getAssets().getTexture(KICK_ICON_IMAGE)
  ),
  m_gameoverText (
      GAMEOVER_TEXT, 
      this->getAssets().getFont(GAMEOVER_FONT),
      GAMEOVER_FONT_SIZE
  ),
  m_gameoverSub1Text (
      GAMEOVER_SUBTITLE1, 
      this->getAssets().getFont(GAMEOVER_FONT), 
      GAMEOVER_SUBTITLE_FONT_SIZE
  ),
  m_gameoverSub2Text (
      GAMEOVER_SUBTITLE2, 
      this->getAssets().getFont(GAMEOVER_FONT), 
      GAMEOVER_SUBTITLE_FONT_SIZE
  )
{
    this->m_healthBar.setHealth(this->m_player.health());
    this->m_shootSound.setVolume(50);

    this->m_gameoverText.setPosition({
        getCenterX(this->getWindow(), this->m_gameoverText),
        GAMEOVER_Y
    });
    this->m_gameoverText.setFillColor(GAMEOVER_COLOR);

    this->m_gameoverSub1Text.setPosition({
        getCenterX(this->getWindow(), this->m_gameoverSub1Text),
        GAMEOVER_SUBTITLE1_Y
    });
    this->m_gameoverSub1Text.setFillColor(GAMEOVER_COLOR);
    
    this->m_gameoverSub2Text.setPosition({
        getCenterX(this->getWindow(), this->m_gameoverSub2Text),
        GAMEOVER_SUBTITLE2_Y
    });
    this->m_gameoverSub2Text.setFillColor(GAMEOVER_COLOR);

    this->m_nameTextbox
        .setAlignment(SimpleTextBox::Center)
        .setFont(&this->getAssets().getFont(NAME_FONT))
        .setFontSize(CLICK_FONT_SIZE)
        .setColor(sf::Color::Yellow)
        .setText(this->getScoreSystem().getLastPlayerName())
        .setOnEnter([this](const sf::String& name) {
            this->m_nameTextbox.setSelected(false);
        });
}

PlayingState::~PlayingState()
{
    if (this->m_state == GameState::OVER)
      this->getScoreSystem().addScore({
          this->m_nameTextbox.text(), this->m_clock.toString()});
}

void PlayingState::update(sf::Time dt, EventManager &event)
{//WHEN_DEBUG(Benchmarker bench("PlayingState::update");)
    if (this->m_state == GameState::PAUSED) return;

    if (this->m_state == GameState::PLAYING) {
      this->m_clock.update(dt);
      this->m_enemySpawnTimer += dt;
      this->m_soundTimer += dt;
      this->m_actionIndicator.update(dt);
      this->m_player.update(dt);
      this->m_enemies.update(dt);
      this->m_particles.update(dt);

      this->randomlySpawnEnemy();
      this->filterDeadEnemies();
      this->checkPlayerCollisions();

      this->playHitSound();

      this->m_healthBar.setHealth(this->m_player.health());

      if (auto *boss = this->m_enemies.getBossPtr()) {
        this->m_bossHealthBar.setHealth(boss->getHealth());
        this->m_bossHealthBar.setColor(boss->getSignatureColor());
      }

      if (!this->m_player.isAlive()) {
        auto explod = explode(this->m_player.getCenter(), PLAYER_COLOR, 
            AVERAGE_EXPLODE_PARTICLE_SIZE,
            AVERAGE_EXPLODE_RADIUS,
            AVERAGE_EXPLODE_PARTICLE_LIFETIME,
            10);
        for (auto &particle : explod)
          this->m_particles.addParticle(std::move(particle));
        this->triggerGameOver();
      }

      this->action();
    }
    else if (this->m_state == GameState::OVER) {
      this->m_enemies.update(dt);
      this->m_particles.update(dt);
      this->m_actionIndicator.update(dt, true);
      this->filterDeadEnemies();
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
    if (this->m_enemies.haveBoss())
      target.draw(this->m_bossHealthBar);

    this->m_actionIndicator.drawTo(target);

    if (this->m_state == GameState::OVER) {
      target.draw(this->m_gameoverText);
      target.draw(this->m_gameoverSub1Text);
      target.draw(this->m_gameoverSub2Text);
      if (this->m_nameTextbox.isSelected())
        target.draw(this->m_nameTextbox);
    }

    if (::show_hitbox)
      for (const auto *const &entity : Entity::allEntities())
        entity->showHitBoxTo(target);
}

void PlayingState::asTopState()
{
    if (this->m_state == GameState::PAUSED)
      this->m_state = GameState::PLAYING;
    if (this->m_state == GameState::PLAYING) {
      this->getWindow().setMouseCursorVisible(true);
      this->getWindow().setMouseCursor(*this->m_cursor);
    }
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
      direction = normalize0(direction);
      this->playerShoot(direction);
    }
    else if (get_action == PlayerAction::KICK) {
      auto mouse_pos = Vec2(sf::Mouse::getPosition());
      Vec2 direction = mouse_pos - this->m_player.getCenter();
      direction = normalize0(direction);
      this->playerKick(direction);
    }
}

void PlayingState::processInput(sf::Time dt, EventManager &em)
{
    auto &&events = em.getPendingEvents();
    if (this->m_state == GameState::PLAYING) {
      for (const auto &event : events) {
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::P 
           || event.key.code == sf::Keyboard::Escape) {
            this->requestPause();
            break;
          }
        }
        else if (event.type == sf::Event::LostFocus) {
          this->requestPause();
          break;
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
          auto click = event.mouseButton;
          if (click.button == sf::Mouse::Right) {
            this->requestPause();
            break;
          }
        }
      }
    }

    else if (this->m_state == GameState::OVER) {
      for (const auto &event : events) {
        if (event.type == sf::Event::MouseButtonPressed) {
          auto click = event.mouseButton;
          if (click.button == sf::Mouse::Left) {
            this->requestRestart();
            break;
          }
          if (click.button == sf::Mouse::Right) {
            this->requestMenu();
            break;
          }
        }
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Escape) {
            this->requestMenu();
            break;
          }
        }
        this->m_nameTextbox.respondTo(event, false);
      }
      this->m_nameTextbox.setPosition(
          sf::Vector2f(sf::Mouse::getPosition()) 
        - this->m_nameTextbox.getSize() / 2.f);
    }
}

void PlayingState::triggerGameOver()
{
    this->m_state = GameState::OVER;
    this->m_nameTextbox.setSelected(true);
    this->getWindow().setMouseCursorVisible(false);
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

void PlayingState::filterDeadEnemies()
{
    bool boss_dead = false;
    for (auto &enemy : this->m_enemies) {
      if (boss_dead) kill(enemy);
      if (!enemy.isAlive()) {
        if (auto *boss = dynamic_cast<Boss*>(&enemy)) {
          boss_dead = true;
          auto explod = explode(boss->getCenter(), boss->getSignatureColor(), 
              20.f, 300.f, AVERAGE_EXPLODE_PARTICLE_LIFETIME, 30);
          for (auto &particle : explod)
            this->m_particles.addParticle(std::move(particle));
        }
        else {
          auto explod = explode(enemy.getCenter(), enemy.getSignatureColor());
          for (auto &particle : explod)
            this->m_particles.addParticle(std::move(particle));
        }
      }
      else if (this->isOffScreen(enemy))
        kill(enemy);
    }
    this->m_enemies.filterDeadEnemies();
}

void PlayingState::randomlySpawnEnemy()
{
    if (decide(this->getEnemySpawnProbability()))
      this->spawnEnemy();
}


float PlayingState::getEnemySpawnProbability()
{
    using std::sin;
    float sin_time = sin(this->m_enemySpawnTimer.asSeconds() * 1.8);
    float prob = 0.012 + 0.08 * std::pow(std::max(sin_time, 0.f), 6);
    if (this->m_enemies.haveBoss()) prob /= 2;
    return prob;
}

void PlayingState::spawnEnemy()
{
    Point spawn_pos = this->getRandomSpawnPosition();
    if (!randInt(0, 50) && !this->m_enemies.haveBoss())
      this->m_enemies.addBoss(
          makeUnique<FirstBoss>(spawn_pos, &this->m_player));
    else {
      Point player_pos = this->m_player.getCenter();
      Angle rand_angle;
      if (player_pos == spawn_pos) 
        rand_angle = Angle::fromDegrees(randFloat(0, 360)); 
      else {
        Vec2 direction = normalize(player_pos - spawn_pos);
        rand_angle = toAngle(direction) + Angle::fromDegrees(randInt(-10, 10));
      }
      Vec2 velocity = toDirection(rand_angle) * BASIC_ENEMY_SPEED;
      this->m_enemies.addEnemy(makeUnique<BasicEnemy>(spawn_pos, velocity));
    }
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
        return LaserEndPoint{laser_front.getCenter() + 5.f * direction, enemy};
    }

    return {laser_front.getCenter(), nullptr};
}

bool PlayingState::isOffScreen(const Entity &entity) const
{
    return LaserWave::isOffScreen(*this, entity.getCenter());
}

Vec2 PlayingState::getRandomSpawnPosition() const
{
    return LaserWave::getRandomSpawnPosition(*this);
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
    return normalize0(direction);
}

void PlayingState::checkPlayerCollisions()
{
    for (auto &enemy : this->m_enemies) {
      if (isColliding(this->m_player, enemy)) {
        // ::log_file << "Player got hit by enemy " << &enemy << std::endl;
        this->playerGetHit(normalize0(this->m_player.getCenter() - enemy.getCenter())); 
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