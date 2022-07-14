#include "assets.hpp"
#include "constants.hpp"

namespace LaserWave
{

const AssetId MADEWITH_FONT ("MADEWITH-FONT");
const AssetId SFML_LOGO ("SFML-LOGO");
const AssetId TITLE_FONT ("TITLE-FONT");
const AssetId BUTTON_IMAGE ("BUTTON-IMAGE");
const AssetId GAMECLOCK_FONT ("GAMECLOCK-FONT");
const AssetId CLICK_FONT ("CLICK-FONT");
const AssetId DEFAULT_CURSOR ("DEFAULT-CURSOR");
const AssetId PLAYING_CURSOR ("PLAYING-CURSOR");
const AssetId SHOOT_ICON_IMAGE ("SHOOT-ICON-IMAGE");
const AssetId KICK_ICON_IMAGE ("KICK-ICON-IMAGE");
const AssetId KICK_PARTICLE_IMAGE ("KICK-PARTICLE-IMAGE");
const AssetId SHOOT_SOUND ("SHOOT-SOUND");
const AssetId KICK_SOUND ("KICK-SOUND");
const AssetId HIT_SOUND ("HIT-SOUND");
const AssetId EXIT_BUTTON ("EXIT-BUTTON");
const AssetId EXIT_BUTTON_HOVER ("EXIT-BUTTON-HOVER");
const AssetId PAUSE_FONT ("PAUSE-FONT");
const AssetId RESUME_BUTTON ("RESUME-BUTTON");
const AssetId RESUME_BUTTON_HOVER ("RESUME-BUTTON-HOVER");
const AssetId RESTART_BUTTON ("RESTART-BUTTON");
const AssetId RESTART_BUTTON_HOVER ("RESTART-BUTTON-HOVER");
const AssetId MENU_BUTTON ("MENU-BUTTON");
const AssetId MENU_BUTTON_HOVER ("MENU-BUTTON-HOVER");
const AssetId GAMEOVER_FONT ("GAMEOVER-FONT");



const char *const ICON_PATH = "assets/images/SFML-icon.png";

void loadAssets(AssetManager &asset)
{
    asset.loadTexture(SFML_LOGO, "assets/images/SFML-logo.png");
    asset.loadTexture(SHOOT_ICON_IMAGE, "assets/images/shoot-icon.png");
    asset.loadTexture(KICK_ICON_IMAGE, "assets/images/kick-icon.png");
    asset.loadTexture(KICK_PARTICLE_IMAGE, "assets/images/kick.png", 
        [](sf::Texture &texture){
            texture.setSmooth(true);
        });
    asset.loadTexture(EXIT_BUTTON, "assets/images/exit-default.png");
    asset.loadTexture(EXIT_BUTTON_HOVER, "assets/images/exit-hover.png");
    asset.loadTexture(RESUME_BUTTON, "assets/images/resume-default.png");
    asset.loadTexture(RESUME_BUTTON_HOVER, "assets/images/resume-hover.png");
    asset.loadTexture(RESTART_BUTTON, "assets/images/restart-default.png");
    asset.loadTexture(RESTART_BUTTON_HOVER, "assets/images/restart-hover.png");
    asset.loadTexture(MENU_BUTTON, "assets/images/menu-default.png");
    asset.loadTexture(MENU_BUTTON_HOVER, "assets/images/menu-hover.png");
    asset.loadCursor(DEFAULT_CURSOR, sf::Cursor::Arrow); 
    asset.loadCursor(PLAYING_CURSOR, "assets/images/crosshair2.png", {64, 64});
    asset.loadFont(MADEWITH_FONT, "assets/fonts/PilotCommand.otf");
    asset.loadFont(GAMECLOCK_FONT, "assets/fonts/Zealot.ttf");
    asset.loadFont(TITLE_FONT, "assets/fonts/SportypoReguler.ttf");
    asset.loadFont(CLICK_FONT, "assets/fonts/Cyberjunkies.ttf");
    asset.loadFont(PAUSE_FONT, "assets/fonts/PilotCommand.otf");
    asset.loadFont(GAMEOVER_FONT, "assets/fonts/PilotCommand.otf");
    asset.loadSound(KICK_SOUND, "assets/sounds/bigbassfoot-kick.wav");
    asset.loadSound(SHOOT_SOUND, "assets/sounds/lotruesn.wav");
    asset.loadSound(HIT_SOUND, "assets/sounds/Tom7.wav");
}

SimpleButton makeButton(const sf::Texture *texture, sf::Vector2f pos,
                        SimpleButton::OnClickFunctor on_click,
                        const sf::Texture *hover_texture)
{
    sf::Vector2f size = sf::Vector2f(texture->getSize());
    SimpleButton button(sf::FloatRect(pos, size));
    button.setDefaultDraw(
        [texture](sf::FloatRect rect, sf::RenderTarget &window, 
                  sf::RenderStates states = {}) {
            sf::Sprite sprite = makeSprite(texture, rect);
            window.draw(sprite, states);
        }
    );
    if (on_click)
      button.setOnClick(std::move(on_click));
    if (hover_texture) button.setHoverDraw(
        [hover_texture](sf::FloatRect rect, sf::RenderTarget &window, 
                  sf::RenderStates states = {}) {
            sf::Sprite sprite = makeSprite(hover_texture, rect);
            window.draw(sprite, states);
        }
    );
    return button;
}

const List<DrumNote> PATTERN = {
  {PlayerAction::KICK, 0.4f},
  {PlayerAction::KICK, 0.65f},
  {PlayerAction::SHOOT, 0.2f},
  {PlayerAction::KICK, 1.3f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::KICK, 0.9f},
  {PlayerAction::KICK, 0.65f},
  {PlayerAction::SHOOT, 0.2f},
  {PlayerAction::KICK, 1.3f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::KICK, 0.9f},
  {PlayerAction::KICK, 0.65f},
  {PlayerAction::SHOOT, 0.2f},
  {PlayerAction::KICK, 1.3f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::KICK, 0.8f},
  {PlayerAction::KICK, 0.2f},
  {PlayerAction::KICK, 0.2f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::KICK, 0.6f},
  {PlayerAction::KICK, 0.4f},
  {PlayerAction::KICK, 0.2f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::KICK, 0.8f},
  {PlayerAction::KICK, 0.65f},
  {PlayerAction::SHOOT, 0.2f},
  {PlayerAction::KICK, 1.3f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::KICK, 0.9f},
  {PlayerAction::KICK, 0.65f},
  {PlayerAction::SHOOT, 0.2f},
  {PlayerAction::KICK, 1.3f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::KICK, 0.9f},
  {PlayerAction::KICK, 0.65f},
  {PlayerAction::SHOOT, 0.2f},
  {PlayerAction::KICK, 1.3f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::KICK, 0.8f},
  {PlayerAction::KICK, 0.2f},
  {PlayerAction::KICK, 0.2f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::SHOOT, 0.4f},
  {PlayerAction::KICK, 0.2f},
  {PlayerAction::KICK, 0.4f},
  {PlayerAction::KICK, 0.4f},
  {PlayerAction::SHOOT, 0.2f},
  {PlayerAction::SHOOT, 0.2f},
  {PlayerAction::SHOOT, 0.2f},
};

} // namespace LaserWave