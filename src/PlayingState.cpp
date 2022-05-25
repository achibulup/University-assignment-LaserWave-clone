#include "PlayingState.hpp"
#include "GameMaths.hpp"
#include "constants.hpp"
#include "randoms.hpp"
#include "assets.hpp"
#include "KickParticle.hpp"
#include "LaserBeam.hpp"
#include "PauseState.hpp"
#include "stateRequests.hpp"

extern bool show_hitbox;

namespace LaserWave
{

const State::Id PlayingState::ID = "Playing";

PlayingState::PlayingState(GameDataRef data) 
: State(data),
  m_clock(GAMECLOCK_POS, &this->getAssets().getFont(GAMECLOCK_FONT)),
  m_player(sf::Vector2f(this->getWindow().getSize()) / 2.f),
  m_enemies(),
  m_cursor(&this->getAssets().getCursor(PLAYING_CURSOR)),
  m_shoot_sound(this->getAssets().getSound(SHOOT_SOUND)),
  m_kick_sound(this->getAssets().getSound(KICK_SOUND)),
  m_hit_sound(this->getAssets().getSound(HIT_SOUND)),
  m_gameover_text(GAMEOVER_TEXT, this->getAssets().getFont(GAMEOVER_FONT),
                  GAMEOVER_FONT_SIZE),
  m_gameover_sub1_text(GAMEOVER_SUBTITLE1, 
      this->getAssets().getFont(GAMEOVER_FONT), GAMEOVER_SUBTITLE_FONT_SIZE),
  m_gameover_sub2_text(GAMEOVER_SUBTITLE2, 
      this->getAssets().getFont(GAMEOVER_FONT), GAMEOVER_SUBTITLE_FONT_SIZE),
  m_action_pattern(PATTERN)
{
    this->m_health_bar.setHealth(this->m_player.health());
    this->m_shoot_sound.setVolume(50);

    this->m_gameover_text.setPosition({
        this->getWindow().getSize().x / 2 - this->m_gameover_text.getGlobalBounds().width / 2,
        GAMEOVER_Y
    });
    this->m_gameover_text.setColor(GAMEOVER_COLOR);
    this->m_gameover_sub1_text.setPosition({
        this->getWindow().getSize().x / 2 - this->m_gameover_sub1_text.getGlobalBounds().width / 2,
        GAMEOVER_SUBTITLE1_Y
    });
    this->m_gameover_sub1_text.setColor(GAMEOVER_COLOR);
    this->m_gameover_sub2_text.setPosition({
        this->getWindow().getSize().x / 2 - this->m_gameover_sub2_text.getGlobalBounds().width / 2,
        GAMEOVER_SUBTITLE2_Y
    });
    this->m_gameover_sub2_text.setColor(GAMEOVER_COLOR);
}
PlayingState::~PlayingState() = default;

std::vector<StateRequest> PlayingState::update(sf::Time dt, EventManager &event)
{
    if (this->m_state == GameState::PAUSED) return {};

    if (this->m_state == GameState::PLAYING) {
      this->m_clock.add1Tick();
      this->m_enemy_spawn_timer += dt;
      this->m_sound_timer += dt;
      this->m_action_timer += dt;
      this->m_player.update(dt);
      this->m_enemies.update(dt);
      this->m_particles.update(dt);

      this->randomlySpawnEnemy();
      this->filterOffScreenEnemies();

      {
        for (auto &enemy : this->m_enemies) {
          if (isColliding(this->m_player, enemy))
            this->playerGetHit(normalize(this->m_player.getCenter() - enemy.getCenter())); 
        }
      
        auto direction = this->checkPlayerTouchEdge();
        if (direction != sf::Vector2f(0, 0)) 
          this->playerGetHit(direction);
      }

      if (this->m_pending_hit_sound > 0 
       && this->m_sound_timer.asSeconds() > 0.3f) {
        this->m_hit_sound.play();
        this->m_sound_timer = sf::Time::Zero;
        this->m_pending_hit_sound--;
      }

      this->m_health_bar.setHealth(this->m_player.health());
      if (!this->m_player.isAlive()) {
        this->m_state = GameState::OVER;
      }

      this->action();
    }
    else if (this->m_state == GameState::OVER) {
      this->m_enemies.update(dt);
      this->m_particles.update(dt);
      this->filterOffScreenEnemies();
    }
    this->processInput(dt, event);
    return std::move(this->m_requests);
}

void PlayingState::render() const
{
    auto &target = this->getWindow();
    target.clear(BACKGROUND_COLOR);
    this->m_enemies.drawTo(target);
    if (this->m_player.isAlive())
      target.draw(this->m_player);
    this->m_particles.drawTo(target);
    target.draw(this->m_clock);
    target.draw(this->m_health_bar);

    if (this->m_state == GameState::OVER) {
      target.draw(this->m_gameover_text);
      target.draw(this->m_gameover_sub1_text);
      target.draw(this->m_gameover_sub2_text);
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

template<typename> class CheckType;

void PlayingState::action()
{
  if (this->m_action_pattern.empty()) return;
  auto next_note = this->m_action_pattern[this->m_action_iter];
  auto next_action_delay = sf::seconds(next_note.delay);
  if (this->m_action_timer >= next_action_delay) {

    if (next_note.type == DrumNote::SNARE) {
      auto mouse_pos = sf::Vector2f(sf::Mouse::getPosition());
      sf::Vector2f direction = mouse_pos - m_player.getCenter();
      direction = normalize(direction);
      this->playerShoot(direction);
    }
    else if (next_note.type == DrumNote::KICK) {
      auto mouse_pos = sf::Vector2f(sf::Mouse::getPosition());
      sf::Vector2f direction = mouse_pos - m_player.getCenter();
      direction = normalize(direction);
      this->playerKick(direction);
    }

    this->m_action_timer -= next_action_delay;
    this->m_action_iter = (this->m_action_iter + 1) % this->m_action_pattern.size();
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

void PlayingState::requestPause()
{
    this->m_state = GameState::PAUSED;
    this->m_requests.push_back(makePushRequest<PauseState>());
}

void PlayingState::requestRestart()
{
    this->m_requests.push_back(makePopRequest());
    this->m_requests.push_back(makePushRequest<PlayingState>());
}

void PlayingState::requestMenu()
{
    this->m_requests.push_back(makePopRequest());
}

void PlayingState::filterOffScreenEnemies()
{
    for (auto &enemy : this->m_enemies) {
      if (this->isFiltered(enemy))
        kill(enemy);
    }
    this->m_enemies.filterDeadEnemies();
}

void PlayingState::randomlySpawnEnemy()
{
    if (decide(this->getEnemySpawnProbability())) {
      this->spawnEnemy();
    }
}

void PlayingState::gameOver()
{
    this->m_state = GameState::OVER;
    this->m_requests.clear();
    this->m_requests.push_back(makeClearRequest());
}


float PlayingState::getEnemySpawnProbability()
{
    float sin_time = sin(this->m_enemy_spawn_timer.asSeconds() * 1.8);
    return 0.002 + 0.05 * std::pow(sqr(sin_time), 3);
}

void PlayingState::spawnEnemy()
{
    sf::Vector2f spawn_pos = this->getRandomSpawnPosition();
    sf::Vector2f player_position = this->m_player.getCenter();
    this->m_enemies.addRandomEnemy(spawn_pos, player_position);
}

void PlayingState::playerShoot(sf::Vector2f direction)
{
    this->m_player.shoot(direction);
    sf::Vector2f laser_start_pos 
        = this->m_player.getCenter() + direction * PLAYER_LASER_START_OFFSET;
    auto laser_end = this->findLaserEndPoint(laser_start_pos, direction);
    if (laser_end.hitEnemy) 
      const_cast<Enemy&>(*laser_end.hitEnemy).getHit();
    this->m_particles.addParticle(std::make_unique<LaserBeam>(
        laser_start_pos, laser_end.position));
    this->m_shoot_sound.play();
    this->m_sound_timer = sf::Time::Zero;
}

void PlayingState::playerKick(sf::Vector2f direction)
{
    this->m_player.kick(direction);
    this->m_particles.addParticle(std::make_unique<KickParticle>(
        this->m_player.getCenter(), toAngle(direction), 
        &this->getAssets().getTexture(KICK_PARTICLE_IMAGE)));
    this->m_kick_sound.play();
    this->m_sound_timer = sf::Time::Zero;
}

void PlayingState::playerGetHit(sf::Vector2f direction)
{
    this->m_player.getHit(direction);
    if (this->m_pending_hit_sound < 1) ++this->m_pending_hit_sound;
}

LaserEndPoint PlayingState::
findLaserEndPoint(sf::Vector2f position, sf::Vector2f direction) const
{
    auto getHitEnemy = [&] (sf::FloatRect laser_front) {
      for (const Enemy &enemy : this->m_enemies) {
        if (laser_front.intersects(enemy.getHitbox()))
          return &enemy;
      }
      return static_cast<const Enemy*>(nullptr);
    };
    auto calcLaserFrontSize = [] (sf::Vector2f direction, float laser_width) {
        return sf::Vector2f{laser_width * 1.17f, laser_width * 1.17f};
    };
    auto getCenter = [] (sf::FloatRect rect) {
        return sf::Vector2f{rect.left + rect.width / 2.f, rect.top + rect.height / 2.f};
    };
    auto outOfScreen = [&] (sf::FloatRect rect) {
      const float TOP_Y = -ENEMY_FILTER_MARGIN.y;
      const float BOTTOM_Y = this->getWindow().getSize().y + ENEMY_SPAWN_MARGIN.y;
      const float LEFT_X = -ENEMY_FILTER_MARGIN.x;
      const float RIGHT_X = this->getWindow().getSize().x + ENEMY_FILTER_MARGIN.x;
      auto center = getCenter(rect);
      return center.x < LEFT_X
          || center.x > RIGHT_X
          || center.y < TOP_Y
          || center.y > BOTTOM_Y;
    };
    sf::FloatRect laser_front;
    sf::Vector2f front_size = calcLaserFrontSize(direction, LASER_WIDTH);
    laser_front.left = position.x - front_size.x / 2.f;
    laser_front.top = position.y - front_size.y / 2.f;
    laser_front.width = front_size.x;
    laser_front.height = front_size.y;

    while (!outOfScreen(laser_front)) {
      auto *enemy = getHitEnemy(laser_front);
      laser_front.left += direction.x;
      laser_front.top += direction.y;
      if (enemy)
        return LaserEndPoint{getCenter(laser_front) + 14.f * direction, enemy};
    }

    return {getCenter(laser_front), nullptr};
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

sf::Vector2f PlayingState::getRandomSpawnPosition() const
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
    sf::Vector2f res;
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

sf::Vector2f PlayingState::checkPlayerTouchEdge() 
{
    const float TOP_Y = 10;
    const float BOTTOM_Y = this->getWindow().getSize().y - 10;
    const float LEFT_X = 10;
    const float RIGHT_X = this->getWindow().getSize().x - 10;
    sf::FloatRect player_hitbox = this->m_player.getHitbox();
    sf::Vector2f direction = {0, 0};
    if (player_hitbox.left < LEFT_X) 
      direction += {1.0, 0.0};
    if (player_hitbox.left + player_hitbox.width > RIGHT_X) 
      direction += {-1.0, 0.0};
    if (player_hitbox.top < TOP_Y) 
      direction += {0.0, 1.0};
    if (player_hitbox.top + player_hitbox.height > BOTTOM_Y) 
      direction += {0.0, -1.0};
    if (direction != sf::Vector2f{0, 0}) return normalize(direction);
    return direction;
}

} // namespace LaserWave