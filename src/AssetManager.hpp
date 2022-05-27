#ifndef ASSETMANAGER_HPP_INCLUDED
#define ASSETMANAGER_HPP_INCLUDED

#include "commons.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>

namespace LaserWave
{


class AssetManager
{
  public:
    AssetManager();

    void loadTexture(std::string, std::string fileName);
    void loadTexture(std::string, const sf::Texture &copy);
    const sf::Texture& getTexture(std::string) const;

    void loadCursor(std::string, std::string fileName, sf::Vector2u hotSpot);
    void loadCursor(std::string, sf::Cursor::Type);
    const sf::Cursor& getCursor(std::string) const;

    void loadFont(std::string, std::string fileName);
    const sf::Font& getFont(std::string) const;

    void loadSound(std::string, std::string fileName);
    const sf::SoundBuffer& getSound(std::string) const;

  private:
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::Cursor> m_cursors;
    std::map<std::string, sf::Font> m_fonts;
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
};
}

#endif // ASSETMANAGER_HPP_INCLUDED