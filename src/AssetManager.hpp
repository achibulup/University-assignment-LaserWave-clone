#ifndef ASSETMANAGER_HPP_INCLUDED
#define ASSETMANAGER_HPP_INCLUDED

#include "commons.hpp"
#include "IdTemplate.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
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
    using TextureInit = std::function<void(sf::Texture&)>;
    using CursorInit = std::function<void(sf::Cursor&)>;
    using FontInit = std::function<void(sf::Font&)>;
    using SoundInit = std::function<void(sf::SoundBuffer&)>;

    AssetManager();

    void loadTexture(AssetId, std::string fileName, const TextureInit &init = {});
    void loadTexture(AssetId, const sf::Texture &copy, const TextureInit &init = {});
    const sf::Texture& getTexture(AssetId) const;

    void loadCursor(AssetId, std::string fileName, sf::Vector2u hotSpot, const CursorInit &init = {});
    void loadCursor(AssetId, sf::Cursor::Type, const CursorInit &init = {});
    const sf::Cursor& getCursor(AssetId) const;

    void loadFont(AssetId, std::string fileName, const FontInit &init = {});
    const sf::Font& getFont(AssetId) const;

    void loadSound(AssetId, std::string fileName, const SoundInit &init = {});
    const sf::SoundBuffer& getSound(AssetId) const;

  private:
    std::map<AssetId, sf::Texture> m_textures;
    std::map<AssetId, sf::Cursor> m_cursors;
    std::map<AssetId, sf::Font> m_fonts;
    std::map<AssetId, sf::SoundBuffer> m_soundBuffers;
};

}

#endif // ASSETMANAGER_HPP_INCLUDED