#include "AssetManager.hpp"

namespace LaserWave
{
AssetManager::AssetManager() = default;

void AssetManager::loadTexture(std::string name, std::string fileName)
{
    sf::Texture texture;
    if (!texture.loadFromFile(fileName))
      throw std::runtime_error("Failed to load texture: " + fileName);
    this->m_textures.insert_or_assign(name, texture);
}
void AssetManager::loadTexture(std::string name, const sf::Texture &copy)
{
    this->m_textures.insert_or_assign(name, copy);
}
const sf::Texture& AssetManager::getTexture(std::string name) const
{
    return this->m_textures.at(name);
}

void AssetManager::loadCursor(std::string name, 
    std::string fileName, sf::Vector2u hotSpot)
{
    sf::Image image;
    if (!image.loadFromFile(fileName))
      throw std::runtime_error("Failed to load image: " + fileName);
    this->m_cursors[name].loadFromPixels(
        image.getPixelsPtr(), image.getSize(), hotSpot);
}
void AssetManager::loadCursor(std::string name, sf::Cursor::Type type)
{
    this->m_cursors[name].loadFromSystem(type);
}
const sf::Cursor& AssetManager::getCursor(std::string name) const
{
    return this->m_cursors.at(name);
}

void AssetManager::loadFont(std::string name, std::string fileName)
{
    sf::Font font;
    if (!font.loadFromFile(fileName))
      throw std::runtime_error("Failed to load font: " + fileName);
    this->m_fonts.insert_or_assign(name, font);
}
const sf::Font& AssetManager::getFont(std::string name) const
{
    return this->m_fonts.at(name);
}

void AssetManager::loadSound(std::string name, std::string fileName)
{
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(fileName))
      throw std::runtime_error("Failed to load sound buffer: " + fileName);
    this->m_soundBuffers.insert_or_assign(name, soundBuffer);
}
const sf::SoundBuffer& AssetManager::getSound(std::string name) const
{
    return this->m_soundBuffers.at(name);
}

}