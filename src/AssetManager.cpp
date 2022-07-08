#include "AssetManager.hpp"

namespace LaserWave
{
AssetManager::AssetManager() = default;

void AssetManager::loadTexture(AssetId id, std::string fileName)
{
    sf::Texture texture;
    if (!texture.loadFromFile(fileName))
      throw std::runtime_error("Failed to load texture: " + fileName);
    this->m_textures.insert_or_assign(id, texture);
}
void AssetManager::loadTexture(AssetId id, const sf::Texture &copy)
{
    this->m_textures.insert_or_assign(id, copy);
}
const sf::Texture& AssetManager::getTexture(AssetId id) const
{
    return this->m_textures.at(id);
}

void AssetManager::loadCursor(AssetId id, 
    std::string fileName, sf::Vector2u hotSpot)
{
    sf::Image image;
    if (!image.loadFromFile(fileName))
      throw std::runtime_error("Failed to load image: " + fileName);
    this->m_cursors[id].loadFromPixels(
        image.getPixelsPtr(), image.getSize(), hotSpot);
}
void AssetManager::loadCursor(AssetId id, sf::Cursor::Type type)
{
    this->m_cursors[id].loadFromSystem(type);
}
const sf::Cursor& AssetManager::getCursor(AssetId id) const
{
    return this->m_cursors.at(id);
}

void AssetManager::loadFont(AssetId id, std::string fileName)
{
    sf::Font font;
    if (!font.loadFromFile(fileName))
      throw std::runtime_error("Failed to load font: " + fileName);
    this->m_fonts.insert_or_assign(id, font);
}
const sf::Font& AssetManager::getFont(AssetId id) const
{
    return this->m_fonts.at(id);
}

void AssetManager::loadSound(AssetId id, std::string fileName)
{
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(fileName))
      throw std::runtime_error("Failed to load sound buffer: " + fileName);
    this->m_soundBuffers.insert_or_assign(id, soundBuffer);
}
const sf::SoundBuffer& AssetManager::getSound(AssetId id) const
{
    return this->m_soundBuffers.at(id);
}

}