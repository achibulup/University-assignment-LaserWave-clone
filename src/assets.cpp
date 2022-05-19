#include "assets.hpp"
#include "constants.hpp"

namespace LaserWave
{

const std::string MADEWITH_FONT = "MADEWITH-FONT";
const std::string SFML_LOGO = "SFML-LOGO";
const std::string TITLE_FONT = "TITLE-FONT";
const std::string BUTTON_IMAGE = "BUTTON-IMAGE";
const std::string GAMECLOCK_FONT = "GAMECLOCK-FONT";
const std::string CLICK_FONT = "CLICK-FONT";
const std::string DEFAULT_CURSOR = "DEFAULT-CURSOR";
const std::string PLAYING_CURSOR = "PLAYING-CURSOR";
const std::string KICK_PARTICLE_IMAGE = "KICK-PARTICLE-IMAGE";
const std::string SHOOT_SOUND = "SHOOT-SOUND";
const std::string KICK_SOUND = "KICK-SOUND";
const std::string HIT_SOUND = "HIT-SOUND";
const std::string EXIT_BUTTON = "EXIT-BUTTON";
const std::string PAUSE_FONT = "PAUSE-FONT";
const std::string RESUME_BUTTON = "RESUME-BUTTON";
const std::string RESTART_BUTTON = "RESTART-BUTTON";
const std::string MENU_BUTTON = "MENU-BUTTON";

const char *const ICON_PATH = "assets/images/SFML-icon.png";

void loadAssets(AssetManager &asset)
{
    asset.loadTexture(SFML_LOGO, "assets/images/SFML-logo.png");
    asset.loadTexture(KICK_PARTICLE_IMAGE, "assets/images/kick.png");
    asset.loadTexture(EXIT_BUTTON, "assets/images/exit-button.png");
    asset.loadTexture(RESUME_BUTTON, "assets/images/resume-button.png");
    asset.loadTexture(RESTART_BUTTON, "assets/images/restart-button.png");
    asset.loadTexture(MENU_BUTTON, "assets/images/menu-button.png");
    asset.loadCursor(DEFAULT_CURSOR, sf::Cursor::Arrow); 
    asset.loadCursor(PLAYING_CURSOR, "assets/images/crosshair2.png", {64, 64});
    asset.loadFont(MADEWITH_FONT, "assets/fonts/PilotCommand.otf");
    asset.loadFont(GAMECLOCK_FONT, "assets/fonts/Zealot.ttf");
    asset.loadFont(TITLE_FONT, "assets/fonts/SportypoReguler.ttf");
    asset.loadFont(CLICK_FONT, "assets/fonts/Cyberjunkies.ttf");
    asset.loadFont(PAUSE_FONT, "assets/fonts/PilotCommand.otf");
    asset.loadSound(KICK_SOUND, "assets/sounds/bigbassfoot-kick.wav");
    asset.loadSound(SHOOT_SOUND, "assets/sounds/lotruesn.wav");
    asset.loadSound(HIT_SOUND, "assets/sounds/Tom7.wav");
}

SimpleButton makeButton(const sf::Texture *texture, sf::Vector2f pos,
                        SimpleButton::OnClickFunctor on_click)
{
    sf::Vector2f size = sf::Vector2f(texture->getSize());
    SimpleButton button(sf::FloatRect(pos, size));
    button.setDefaultDraw(
        [texture](sf::FloatRect rect, sf::RenderTarget &window, 
                  sf::RenderStates states = {}) {
            sf::Sprite sprite = makeSprite(texture, rect);
            sprite.setColor(BUTTON_COLOR);
            window.draw(sprite, states);
        }
    );
    button.setHoverDraw(
        [texture](sf::FloatRect rect, sf::RenderTarget &window, 
                  sf::RenderStates states = {}) {
            sf::Sprite sprite = makeSprite(texture, rect);
            sprite.setColor(BUTTON_HOVER_COLOR);
            window.draw(sprite, states);
        }
    );
    if (on_click)
      button.setOnClick(std::move(on_click));
    return button;
}

} // namespace LaserWave