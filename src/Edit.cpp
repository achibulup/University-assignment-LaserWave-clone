#include <SFML/Graphics.hpp>
#include <iostream>
#include <initializer_list>
#include <SFML/Achibulup_extensions/Pixels.hpp>
#include "GameMaths.hpp"

struct text
{ 
    sf::String str;
    int font_size;
    sf::String default_path;
    sf::String hover_path;
};

int main()
{
    static const char *const BUTTON_FONT_PATH = "assets/fonts/PilotCommand.otf";
    sf::Font font;
    font.loadFromFile(BUTTON_FONT_PATH);
    sf::Texture button_texture;
    button_texture.loadFromFile("assets/images/button.png");

    text texts[] = {
        {"EXIT", 30, "assets/images/exit-default.png", "assets/images/exit-hover.png"},
        {"RESUME", 25, "assets/images/resume-default.png", "assets/images/resume-hover.png"},
        {"RESTART", 26, "assets/images/restart-default.png", "assets/images/restart-hover.png"},
        {"MENU", 30, "assets/images/menu-default.png", "assets/images/menu-hover.png"}
    };

    for (auto &t : texts) {
      sf::Sprite button_sprite;
      button_sprite.setTexture(button_texture, true);
      button_sprite.setPosition({0, 0});
      sf::Text text(t.str, font, t.font_size);
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
        return raw.getPixel(x, y) * sf::Color(143, 28, 175);
      }).saveToFile(t.default_path);
      sfExt::makeImage(raw.getSize().x, raw.getSize().y, [&](int x, int y) {
        if (raw.getPixel(x, y) == sf::Color::Black)
          return sf::Color(0, 0, 0, 0);
        return raw.getPixel(x, y) * sf::Color(65, 59, 131);
      }).saveToFile(t.hover_path);
    }
}

// COMPILER = "D:\CODE\compilers\mingw64\bin\g++.exe"
// "D:\CODE\compilers\mingw64\bin\g++.exe" -g -std=c++17 -o "bin\a.exe" "src\Edit.cpp" -Wall -Wextra -Wpedantic -I"D:\CODE\cpp\libraries\SFML-2.5.1\include" -L"D:\CODE\cpp\libraries\SFML-2.5.1\lib" -lLIFOmemrsc -lsfmlext-tilemap -lsfmlext-pixels -lsfml-graphics -lsfml-window -lsfml-system

