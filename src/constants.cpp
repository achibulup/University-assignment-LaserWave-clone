#include "constants.hpp"

namespace LaserWave
{ 

inline namespace Constants
{

const sf::Vector2i WINDOW_SIZE = {1920, 1080};


const float SPLASH_SCREEN_START_DELAY = 0.15f;
const float SPLASH_SCREEN_FADE_IN = 0.7f;
const float SPLASH_SCREEN_FADE_OUT = 0.7f;
const float SPLASH_SCREEN_HANG = 1.1f;
const float SPLASH_SCREEN_END_DELAY = 0.3f;
const int MADEWITH_FONT_SIZE = 25;
const float MADEWITH_Y = 420.f;
const sf::String MADEWITH_MESSAGE = "MADE WITH";
const sf::Color MADEWITH_COLOR = sf::Color::White;
const float LOGO_Y = 470.f;


const sf::Color BACKGROUND_COLOR = sf::Color(11, 2, 18);
const sf::Color PAUSE_FILTER = sf::Color(0, 0, 0, 128);


const sf::String TITLE = "LASERWAVE";
const int TITLE_FONT_SIZE = 90;
const float TITLE_Y = 45.f;
const sf::Color TITLE_COLOR = sf::Color(110, 30, 150);
const sf::String CLICK_MESSAGE = "CLICK TO PLAY";
const int CLICK_FONT_SIZE = 50;
const float CLICK_Y = 550.f;
const sf::Color CLICK_COLOR = sf::Color::White;
const sf::Color BUTTON_COLOR = sf::Color(143, 28, 175);
const sf::Color BUTTON_HOVER_COLOR = sf::Color(65, 59, 131);
const Vec2 HIGHSCORE_BUTTON_POS = {100.f, 950.f};
const Vec2 EXIT_BUTTON_POS = {1700.f, 950.f};
const float RESUME_BUTTON_Y = 400.f;
const float RESTART_BUTTON_Y = 520.f;
const float MENU_BUTTON_Y = 640.f;

const float TRANSITION_DURATION = 0.25f;
const float TRANSITION_END_DELAY = 0.25f;
const Angle WIPE_ANGLE = Angle::fromDegrees(30);


extern const sf::Color LEADERBOARD_COLOR = sf::Color(155, 47, 144);
extern const float LEADERBOARD_NAME_X = 500.f;
extern const float LEADERBOARD_SCORE_X = 1200.f;


const int GAMECLOCK_FONT_SIZE = 30;
const Vec2 GAMECLOCK_POS = {WINDOW_SIZE.x / 2, 30.f};
const sf::Color GAMECLOCK_COLOR = sf::Color::White;

const Vec2 HEALTH_BAR_POSITION = {60.f, 60.f};
const Vec2 HEALTH_BAR_UNIT_SIZE = {30.f, 75.f};
const float HEALTH_BAR_UNIT_SPACING = 16.f;
const Vec2 BOSS_HEALTH_BAR_POSITION = {WINDOW_SIZE.x - 60.f, 60.f};

const float AVERAGE_EXPLODE_PARTICLE_SIZE = 15.f;
const float AVERAGE_EXPLODE_RADIUS = 250.f;
const float AVERAGE_EXPLODE_PARTICLE_LIFETIME = 0.5f;
const float AVERAGE_EXPLODE_PARTICLE_COUNT = 15;
const Vec2 EXPLODE_VELOCITY_ADJUSTMENT = {-10.f, 0};
const float EXPLODE_PARTICLE_GRAVITY = 450.f;

const float GRAVITY = 260.f;

const sf::Color PLAYER_COLOR = sf::Color(122, 49, 200);
const sf::Color PLAYER_INVINCIBLE_COLOR = sf::Color::White;
const Vec2 PLAYER_SIZE = {40, 64};
const float PLAYER_SPEED = 320.f;
const float PLAYER_HIT_SPEED = 200.f;
const float PLAYER_SHOOT_SPEED = 100.f;
const float PLAYER_MIN_DRAG = 120.f;
const float PLAYER_MAX_FALL = 150.f;
const int PLAYER_MAX_HEALTH = 5;
const float PLAYER_INVINCIBILITY_DURATION = 1.f;

const float KICK_PARTICLE_LIFETIME = 0.5f;

const float LASER_BEAM_LIFETIME = 0.5f;
const float LASER_WIDTH = 10.f;
const float FIRECIRCLE_RADIUS = 25.f;
const float PLAYER_LASER_START_OFFSET = 13.f;
const float LASER_BEAM_OUTLINE_WIDTH = 0;
const sf::Color LASER_BEAM_COLOR = sf::Color(255, 30, 30);
const sf::Color LASER_BEAM_OUTLINE_COLOR = sf::Color(255, 30, 30);//fsf::Color(0xb0dbffu);//sf::Color(250, 160, 255);
const sf::Color LASER_BEAM_CIRCLE_COLOR = sf::Color(255, 80, 80);//sf::Color(255, 68, 0);//sf::Color(0xd7ecfeu);
const sf::Color KICK_COLOR = sf::Color::Yellow;//sf::Color(232, 149, 255);

const float INDICATOR_DISPLAY_TIME = 1.3f;

const Vec2 ENEMY_SPAWN_MARGIN = {60.f, 60.f};
const Vec2 ENEMY_FILTER_MARGIN = {65.f, 65.f};
const float ENEMY_FREEZE_TIME = 0.06f;
const sf::Color ENEMY_HIT_COLOR = sf::Color::White;

const float ENEMY_SPAWN_INTERVAL = 0.75f;

const sf::Color BASIC_ENEMY_COLOR = sf::Color(80, 110, 150);
const float BASIC_ENEMY_RADIUS = 60.f;
const float BASIC_ENEMY_SPEED = 150.f;
const Angle BASIC_ENEMY_MAX_ROTATION_SPEED = Angle::fromDegrees(60.f);
const Angle BASIC_ENEMY_MIN_ROTATION_SPEED = Angle::fromDegrees(5.f);

const sf::Color FIRST_BOSS_COLOR = sf::Color(30, 100, 222);
const float FIRST_BOSS_SIZE = 150.f;

const sf::String PAUSE_TITLE = "PAUSED";
const int PAUSE_TITLE_FONT_SIZE = 90;
const float PAUSE_TITLE_Y = 45.f;
const sf::Color PAUSE_TITLE_COLOR = sf::Color(77, 35, 113);

const sf::Color GAMEOVER_COLOR = sf::Color(65, 59, 131);
const sf::String GAMEOVER_TEXT = "GAME OVER";
const int GAMEOVER_FONT_SIZE = 40;
const float GAMEOVER_Y = 420.f;
const sf::String GAMEOVER_SUBTITLE1 = "LEFT CLICK TO RESTART";
const float GAMEOVER_SUBTITLE1_Y = 500.f;
const sf::String GAMEOVER_SUBTITLE2 = "RIGHT CLICK TO GO TO MENU";
const float GAMEOVER_SUBTITLE2_Y = 530.f;
const int GAMEOVER_SUBTITLE_FONT_SIZE = 20;

} // namespace Constants

} // namespace LaserWave