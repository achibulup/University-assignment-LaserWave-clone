#ifndef CONSTANTS_HPP_INCLUDED
#define CONSTANTS_HPP_INCLUDED

#include "commons.hpp"
#include "GameMaths.hpp"
#include <SFML/Graphics.hpp>

namespace LaserWave
{

inline namespace Constants
{

constexpr float UPDATES_PER_SECOND = 60;

extern const sf::Vector2i WINDOW_SIZE;


extern const float SPLASH_SCREEN_START_DELAY;
extern const float SPLASH_SCREEN_FADE_IN;
extern const float SPLASH_SCREEN_FADE_OUT;
extern const float SPLASH_SCREEN_HANG;
extern const float SPLASH_SCREEN_END_DELAY;
inline const float SPLASH_SCREEN_DURATION 
    = SPLASH_SCREEN_FADE_IN + SPLASH_SCREEN_HANG + SPLASH_SCREEN_FADE_OUT;
extern const int MADEWITH_FONT_SIZE;
extern const float MADEWITH_Y;
extern const sf::String MADEWITH_MESSAGE;
extern const sf::Color MADEWITH_COLOR;
extern const float LOGO_Y;


extern const sf::Color BACKGROUND_COLOR;
extern const sf::Color PAUSE_FILTER;


extern const sf::String TITLE;
extern const int TITLE_FONT_SIZE;
extern const float TITLE_Y;
extern const sf::Color TITLE_COLOR;
extern const sf::String CLICK_MESSAGE;
extern const int CLICK_FONT_SIZE;
extern const float CLICK_Y;
extern const sf::Color CLICK_COLOR;
extern const sf::Color BUTTON_COLOR;
extern const sf::Color BUTTON_HOVER_COLOR;
extern const Vec2 HIGHSCORE_BUTTON_POS;
extern const Vec2 EXIT_BUTTON_POS;
extern const float RESUME_BUTTON_Y;
extern const float RESTART_BUTTON_Y;
extern const float MENU_BUTTON_Y;


extern const sf::Color LEADERBOARD_COLOR;
extern const float LEADERBOARD_NAME_X;
extern const float LEADERBOARD_SCORE_X;


extern const float TRANSITION_DURATION;
extern const float TRANSITION_END_DELAY;
extern const Angle WIPE_ANGLE;


extern const int GAMECLOCK_FONT_SIZE;
extern const Vec2 GAMECLOCK_POS;
extern const sf::Color GAMECLOCK_COLOR;

extern const Vec2 HEALTH_BAR_POSITION;
extern const Vec2 HEALTH_BAR_UNIT_SIZE;
extern const float HEALTH_BAR_UNIT_SPACING;
extern const Vec2 BOSS_HEALTH_BAR_POSITION;

extern const float AVERAGE_EXPLODE_PARTICLE_SIZE;
extern const float AVERAGE_EXPLODE_RADIUS;
extern const float AVERAGE_EXPLODE_PARTICLE_LIFETIME;
extern const float AVERAGE_EXPLODE_PARTICLE_COUNT;
extern const Vec2 EXPLODE_VELOCITY_ADJUSTMENT;
extern const float EXPLODE_PARTICLE_GRAVITY;

extern const float GRAVITY;

extern const sf::Color PLAYER_COLOR;
extern const sf::Color PLAYER_INVINCIBLE_COLOR;
extern const Vec2 PLAYER_SIZE;
extern const float PLAYER_SPEED;
extern const float PLAYER_HIT_SPEED;
extern const float PLAYER_SHOOT_SPEED;
extern const float PLAYER_MIN_DRAG;
extern const float PLAYER_MAX_FALL;
extern const int PLAYER_MAX_HEALTH;
extern const float PLAYER_INVINCIBILITY_DURATION;

extern const float KICK_PARTICLE_LIFETIME;

extern const float LASER_BEAM_LIFETIME;
extern const float LASER_WIDTH;
extern const float FIRECIRCLE_RADIUS;
extern const float PLAYER_LASER_START_OFFSET;
extern const float LASER_BEAM_OUTLINE_WIDTH;
extern const sf::Color LASER_BEAM_COLOR;
extern const sf::Color LASER_BEAM_OUTLINE_COLOR;
extern const sf::Color LASER_BEAM_CIRCLE_COLOR;
extern const sf::Color KICK_COLOR;

extern const float INDICATOR_DISPLAY_TIME;


extern const float ENEMY_SPAWN_INTERVAL;

extern const Vec2 ENEMY_SPAWN_MARGIN;
extern const Vec2 ENEMY_FILTER_MARGIN;
extern const float ENEMY_FREEZE_TIME;
extern const sf::Color ENEMY_HIT_COLOR;

extern const sf::Color BASIC_ENEMY_COLOR;
extern const float BASIC_ENEMY_RADIUS;
extern const float BASIC_ENEMY_SPEED;
extern const Angle BASIC_ENEMY_MAX_ROTATION_SPEED;
extern const Angle BASIC_ENEMY_MIN_ROTATION_SPEED;

extern const sf::Color FIRST_BOSS_COLOR;
extern const float FIRST_BOSS_SIZE;

extern const sf::String PAUSE_TITLE;
extern const int PAUSE_TITLE_FONT_SIZE;
extern const float PAUSE_TITLE_Y;
extern const sf::Color PAUSE_TITLE_COLOR;

extern const sf::Color GAMEOVER_COLOR;
extern const sf::String GAMEOVER_TEXT;
extern const int GAMEOVER_FONT_SIZE;
extern const float GAMEOVER_Y;
extern const int GAMEOVER_SUBTITLE_FONT_SIZE;
extern const sf::String GAMEOVER_SUBTITLE1;
extern const sf::String GAMEOVER_SUBTITLE2;
extern const float GAMEOVER_SUBTITLE1_Y;
extern const float GAMEOVER_SUBTITLE2_Y;

} // namespace Constants

} // namespace LaserWave

#endif // CONSTANTS_HPP_INCLUDED
