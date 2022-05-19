#include <SFML/Graphics.hpp>
#include <iostream>
#include <initializer_list>
#include <SFML/Achibulup_extensions/Pixels.hpp>
#include "GameMaths.hpp"

int main()
{
    static const char *const BUTTON_FONT_PATH = "assets/fonts/PilotCommand.otf";
    static const int FONT_SIZE = 30;
    sf::Font font;
    font.loadFromFile(BUTTON_FONT_PATH);
    sf::Texture button_texture;
    button_texture.loadFromFile("assets/images/button.png");

    sf::Sprite button_sprite;
    button_sprite.setTexture(button_texture, true);
    button_sprite.setPosition({0, 0});
    sf::Text text("EXIT", font, FONT_SIZE);
    text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
    text.setPosition(button_sprite.getGlobalBounds().width / 2, button_sprite.getGlobalBounds().height / 2 - 10);

    sf::RenderTexture rend;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    rend.create(button_sprite.getGlobalBounds().width, button_sprite.getGlobalBounds().height, settings);
    rend.clear();
    rend.draw(button_sprite);
    rend.draw(text);
    rend.display();
    auto raw = rend.getTexture().copyToImage();
    sfExt::makeImage(raw.getSize().x, raw.getSize().y, [&](int x, int y) {
      if (raw.getPixel(x, y) == sf::Color::Black)
        return sf::Color(0, 0, 0, 0);
      return raw.getPixel(x, y);
    }).saveToFile("assets/images/exit-button.png");
}

// COMPILER = "D:\CODE\compilers\mingw64\bin\g++.exe"
// g++ -g -std=c++17 -o "bin\Debug\a.exe" "src\Edit.cpp" -Wall -Wextra -Wpedantic -Wno-unused-function -Wno-unused-parameter -I"D:\CODE\cpp\libraries\SFML-2.5.1\include" -L"D:\CODE\cpp\libraries\SFML-2.5.1\lib" -lLIFOmemrsc -lsfmlext-tilemap -lsfmlext-pixels -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 

