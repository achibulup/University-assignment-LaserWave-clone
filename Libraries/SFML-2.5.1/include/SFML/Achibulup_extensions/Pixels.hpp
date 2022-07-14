#ifndef PIXELS_HPP_INCLUDED
#define PIXELS_HPP_INCLUDED
#include "../Graphics.hpp"
#include <cmath>
#include <functional>

namespace sfExt
{

static const int COLOR_MAX = 255;
static const int COLOR_MAX2 = COLOR_MAX * COLOR_MAX;

///normalize a color from [0,255] to [0,1]
inline float normColor(sf::Uint8 color)
{
    return static_cast<float>(color) / COLOR_MAX;
}
///denormalize a color from [0,1] to [0,255]
inline sf::Uint8 denormColor(float color)
{
    return std::round(color * COLOR_MAX);
}

///equivalent to std::round(float(a) / b)
inline int roundDiv(int a, int b)
{
    return (a + b / 2) / b;
}

///multiply (modulate) two colors
inline sf::Uint8 mulColor(sf::Uint8 color1, sf::Uint8 color2)
{
    return roundDiv(color1 * color2, COLOR_MAX);
}

///combine two colors as if the color \a fore is placed on top of color \a back on a screen
sf::Color combine(sf::Color fore, sf::Color back);

template<typename PixelDrawer>
void drawPixels(int width, int height, sf::Color* pixels, 
                const PixelDrawer& drawer)
{
    for (int x = 0; x < width; ++x)
    for (int y = 0; y < height; ++y)
      pixels[y * width + x] = drawer(x, y);
}
template<typename PixelDrawer>
void drawImage(sf::Image &image, const PixelDrawer& drawer)
{
    int width = image.getSize().x;
    int height = image.getSize().y;
    for (int x = 0; x < width; ++x)
    for (int y = 0; y < height; ++y)
      image.setPixel(drawer(x, y));
}

///auxiliary functions for makeTexture and makeImage
template<typename PixelDrawer>
auto makePixelsDrawer(PixelDrawer &&drawer)
{
    return [drawer=std::forward<PixelDrawer>(drawer)] 
            (int width, int height, sf::Color* pixels)
            {
                drawPixels(width, height, pixels, drawer);
            };
}
template<typename PixelDrawer>
auto makeImageDrawer(PixelDrawer &&drawer)
{
    return [drawer=std::forward<PixelDrawer>(drawer)] 
            (sf::Image& image)
            {
                drawImage(image, drawer);
            };
}

using ErasedPixelsDrawer = std::function<void(int, int, sf::Color*)>;
using ErasedImageDrawer = std::function<void(sf::Image&)>;

sf::Texture makeTextureImpl(int width, int height, ErasedPixelsDrawer drawer);
sf::Image makeImageImpl(int width, int height, ErasedPixelsDrawer drawer);


/// make a \a width * \a height texture and draw each pixel with \a pixel_drawer
template<typename PixelDrawer>
sf::Texture makeTexture(int width, int height, PixelDrawer &&pixel_drawer)
{
    return makeTextureImpl(width, height, 
                           makePixelsDrawer(std::forward<PixelDrawer>(pixel_drawer)));
}
/// make a \a width * \a height image and draw each pixel with \a pixel_drawer
template<typename PixelDrawer>
sf::Image makeImage(int width, int height, PixelDrawer &&pixel_drawer)
{
    return makeImageImpl(width, height, 
                         makePixelsDrawer(std::forward<PixelDrawer>(pixel_drawer)));
}

}

#endif // PIXELS_HPP_INCLUDED