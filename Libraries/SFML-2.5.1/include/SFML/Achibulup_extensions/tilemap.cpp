#include "Tilemap.hpp"

namespace sfExt
{

///mostly copied from SFML's tutorial
bool TileMap::load(const std::string& tileset_file, sf::Vector2u tile_size, 
                   Achibulup::Array2dRef<const int> tiles, 
                   unsigned int width, unsigned int height)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset_file))
        return false;

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    int tiles_per_row = m_tileset.getSize().x / tile_size.x;
    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = tiles[j][i];

            // find its position in the tileset texture
            int tu = tileNumber % tiles_per_row;
            int tv = tileNumber / tiles_per_row;

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = {i * tile_size.x, j * tile_size.y};
            quad[1].position = {(i + 1) * tile_size.x, j * tile_size.y};
            quad[2].position = {(i + 1) * tile_size.x, (j + 1) * tile_size.y};
            quad[3].position = {i * tile_size.x, (j + 1) * tile_size.y};

            // define its 4 texture coordinates
            quad[0].texCoords = {tu * tile_size.x, tv * tile_size.y};
            quad[1].texCoords = {(tu + 1) * tile_size.x, tv * tile_size.y};
            quad[2].texCoords = {(tu + 1) * tile_size.x, (tv + 1) * tile_size.y};
            quad[3].texCoords = {tu * tile_size.x, (tv + 1) * tile_size.y};
        }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

}