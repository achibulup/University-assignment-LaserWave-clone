#include "Pixels.hpp"

namespace sfExt
{

sf::Color combine(sf::Color fore, sf::Color back)
{
    if (fore.a == COLOR_MAX) return fore;
    if (fore.a == 0) return back;
    ///a = 1 - (1 - a1) * (1 - a2)
    ///A = 255 - (255 - A1) * (255 - A2) / 255
    ///c = (c1 * a1 + c2 * a2 * (1 - a1)) / a
    ///C = (C1 * A1 + C2 * A2 * (255 - A1) / 255) / A
    ///  = (C1 * A1 * 255 + C2 * A2 * (255 - A1)) / (255^2 - (255 - A1) * (255 - A2))
    int a_225 = COLOR_MAX2 - (COLOR_MAX - fore.a) * (COLOR_MAX - back.a);
    sf::Color result;
    result.r = roundDiv(fore.r * fore.a * COLOR_MAX + back.r * back.a * (COLOR_MAX - fore.a), a_225);
    result.g = roundDiv(fore.g * fore.a * COLOR_MAX + back.g * back.a * (COLOR_MAX - fore.a), a_225);
    result.b = roundDiv(fore.b * fore.a * COLOR_MAX + back.b * back.a * (COLOR_MAX - fore.a), a_225);
    result.a = roundDiv(a_225, COLOR_MAX);
    return result;
}

sf::Texture makeTextureImpl(int width, int height, ErasedPixlesDrawer drawer)
{
    std::vector<sf::Color> pixels(width * height);
    drawer(width, height, pixels.data());
    sf::Texture texture;
    texture.create(width, height);
    texture.update(reinterpret_cast<sf::Uint8*>(pixels.data()));
    return texture;
}
sf::Image makeImageImpl(int width, int height, ErasedPixlesDrawer drawer)
{
    std::vector<sf::Color> pixels(width * height);
    drawer(width, height, pixels.data());
    sf::Image image;
    image.create(width, height, reinterpret_cast<sf::Uint8*>(pixels.data()));
    return image;
}

}