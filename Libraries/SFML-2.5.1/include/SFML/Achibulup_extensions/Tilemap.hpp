#include "../Graphics.hpp"
#include "../Achibulup_dependencies/ArrayRef.hpp"

namespace sfExt
{

/// based on SFML's tutorial
class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    /// load the tile map from a file
    bool load(const std::string& tileset_file, sf::Vector2u tile_size, 
              Achibulup::Array2dRef<const int> tiles, 
              unsigned int width, unsigned int height);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

}