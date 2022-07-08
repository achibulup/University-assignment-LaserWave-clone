#ifndef ASSETMANAGER_HPP_INCLUDED
#define ASSETMANAGER_HPP_INCLUDED

#include "commons.hpp"
#include "IdTemplate.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>

namespace LaserWave
{

class AssetId : public IdTemplate<AssetId>
{
  public:
    using IdTemplate<AssetId>::IdTemplate;
};

class AssetManager
{
  public:
    AssetManager();

    void loadTexture(AssetId, std::string fileName);
    void loadTexture(AssetId, const sf::Texture &copy);
    const sf::Texture& getTexture(AssetId) const;

    void loadCursor(AssetId, std::string fileName, sf::Vector2u hotSpot);
    void loadCursor(AssetId, sf::Cursor::Type);
    const sf::Cursor& getCursor(AssetId) const;

    void loadFont(AssetId, std::string fileName);
    const sf::Font& getFont(AssetId) const;

    void loadSound(AssetId, std::string fileName);
    const sf::SoundBuffer& getSound(AssetId) const;

  private:
    std::map<AssetId, sf::Texture> m_textures;
    std::map<AssetId, sf::Cursor> m_cursors;
    std::map<AssetId, sf::Font> m_fonts;
    std::map<AssetId, sf::SoundBuffer> m_soundBuffers;
};

}

#endif // ASSETMANAGER_HPP_INCLUDED