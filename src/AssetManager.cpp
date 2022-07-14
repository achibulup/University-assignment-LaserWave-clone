#include "AssetManager.hpp"

namespace LaserWave
{
AssetManager::AssetManager() = default;

void AssetManager::loadTexture(AssetId id, std::string fileName, 
                               const TextureInit &init)
{
    sf::Texture tmp;
    if (!tmp.loadFromFile(fileName))
      throw std::runtime_error("Failed to load texture: " + fileName);
    auto &texture = this->m_textures
        .insert_or_assign(id, tmp).first->second;
    if (init) init(texture);
}
void AssetManager::loadTexture(AssetId id, const sf::Texture &copy, 
                               const TextureInit &init)
{
    auto &texture = this->m_textures.insert_or_assign(id, copy).first->second;
    if (init) init(texture);
}
const sf::Texture& AssetManager::getTexture(AssetId id) const
{
    return this->m_textures.at(id);
}

void AssetManager::loadCursor(AssetId id, std::string fileName, 
                              sf::Vector2u hotSpot, const CursorInit &init)
{
    sf::Image image;
    if (!image.loadFromFile(fileName))
      throw std::runtime_error("Failed to load image: " + fileName);
    auto &cursor = this->m_cursors[id];
    cursor.loadFromPixels(image.getPixelsPtr(), image.getSize(), hotSpot);
    if (init) init(cursor);
}
void AssetManager::loadCursor(AssetId id, sf::Cursor::Type type, 
                              const CursorInit &init)
{
    auto &cursor = this->m_cursors[id];
    cursor.loadFromSystem(type);
    if (init) init(cursor);
}
const sf::Cursor& AssetManager::getCursor(AssetId id) const
{
    return this->m_cursors.at(id);
}

void AssetManager::loadFont(AssetId id, std::string fileName, 
                            const FontInit &init)
{
    sf::Font tmp;
    if (!tmp.loadFromFile(fileName))
      throw std::runtime_error("Failed to load font: " + fileName);
    auto &font = this->m_fonts.insert_or_assign(id, tmp).first->second;
    if (init) init(font);
}
const sf::Font& AssetManager::getFont(AssetId id) const
{
    return this->m_fonts.at(id);
}

void AssetManager::loadSound(AssetId id, std::string fileName, 
                             const SoundInit &init)
{
    sf::SoundBuffer tmp;
    if (!tmp.loadFromFile(fileName))
      throw std::runtime_error("Failed to load sound: " + fileName);
    auto &soundBuffer = this->m_soundBuffers
        .insert_or_assign(id, tmp).first->second;
    if (init) init(soundBuffer);
}
const sf::SoundBuffer& AssetManager::getSound(AssetId id) const
{
    return this->m_soundBuffers.at(id);
}

}