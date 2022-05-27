#ifndef FILPATHS_HPP_INCLUDED
#define FILPATHS_HPP_INCLUDED

#include "AssetManager.hpp"
#include "SimpleButton.hpp"

namespace LaserWave
{

extern const std::string MADEWITH_FONT;
extern const std::string SFML_LOGO;
extern const std::string TITLE_FONT;
extern const std::string BUTTON_IMAGE;
extern const std::string CLICK_FONT;
extern const std::string GAMECLOCK_FONT;
extern const std::string DEFAULT_CURSOR;
extern const std::string PLAYING_CURSOR;
extern const std::string KICK_PARTICLE_IMAGE;
extern const std::string SHOOT_SOUND;
extern const std::string KICK_SOUND;
extern const std::string HIT_SOUND;
extern const std::string EXIT_BUTTON;
extern const std::string EXIT_BUTTON_HOVER;
extern const std::string PAUSE_FONT;
extern const std::string RESUME_BUTTON;
extern const std::string RESUME_BUTTON_HOVER;
extern const std::string RESTART_BUTTON;
extern const std::string RESTART_BUTTON_HOVER;
extern const std::string MENU_BUTTON;
extern const std::string MENU_BUTTON_HOVER;
extern const std::string GAMEOVER_FONT;

extern const char *const ICON_PATH;

void loadAssets(AssetManager &asset);

SimpleButton makeButton(const sf::Texture *texture, sf::Vector2f pos = {},
                        SimpleButton::OnClickFunctor on_click = {},
                        const sf::Texture *hover_texture = nullptr);

struct DrumNote
{
    enum Type
    {
        KICK,
        SNARE,
    }type;
    float delay;
};

extern const float LOOP_LENGTH;
extern const std::vector<DrumNote> PATTERN;

} // namespace LaserWave



#endif // FILPATHS_HPP_INCLUDED
