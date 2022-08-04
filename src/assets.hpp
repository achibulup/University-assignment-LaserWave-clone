#ifndef FILPATHS_HPP_INCLUDED
#define FILPATHS_HPP_INCLUDED

#include "commons.hpp"
#include "AssetManager.hpp"
#include "SimpleButton.hpp"
#include "ActionIndicator.hpp"

namespace LaserWave
{

extern const AssetId MADEWITH_FONT;
extern const AssetId SFML_LOGO;
extern const AssetId TITLE_FONT;
extern const AssetId LEADERBOARD_FONT;
extern const AssetId BUTTON_IMAGE;
extern const AssetId CLICK_FONT;
extern const AssetId GAMECLOCK_FONT;
extern const AssetId DEFAULT_CURSOR;
extern const AssetId PLAYING_CURSOR;
extern const AssetId SHOOT_ICON_IMAGE;
extern const AssetId KICK_ICON_IMAGE;
extern const AssetId KICK_PARTICLE_IMAGE;
extern const AssetId SHOOT_SOUND;
extern const AssetId KICK_SOUND;
extern const AssetId HIT_SOUND;
extern const AssetId HIGHSCORE_BUTTON;
extern const AssetId HIGHSCORE_BUTTON_HOVER;
extern const AssetId BACK_BUTTON;
extern const AssetId BACK_BUTTON_HOVER;
extern const AssetId EXIT_BUTTON;
extern const AssetId EXIT_BUTTON_HOVER;
extern const AssetId PAUSE_FONT;
extern const AssetId RESUME_BUTTON;
extern const AssetId RESUME_BUTTON_HOVER;
extern const AssetId RESTART_BUTTON;
extern const AssetId RESTART_BUTTON_HOVER;
extern const AssetId MENU_BUTTON;
extern const AssetId MENU_BUTTON_HOVER;
extern const AssetId GAMEOVER_FONT;
extern const AssetId NAME_FONT;
extern const AssetId SCORE_FONT;

extern const char *const ICON_PATH;

void loadAssets(AssetManager &asset);
List<DrumNote> loadActionPattern();

} // namespace LaserWave



#endif // FILPATHS_HPP_INCLUDED
