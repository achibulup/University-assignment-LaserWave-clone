#include <SFML/Achibulup_dependencies/common_utils.h>
#include <fstream>
#if ACHIBULUP__before_Cpp17
#error This file requires compiler and library support \
for the ISO C++ 2017 standard. This support must be enabled \
with the -std=c++17 compiler options.
#endif
#include "Game.hpp"

bool show_hitbox = false;
std::ofstream log_file("data\\log.txt");
int main(int argc, char* argv[])
{
    if (argc != 1) show_hitbox = true;
    try {
      LaserWave::Game().run();
    }
    catch (const std::exception& e) {
      log_file << "Exception: " << e.what() << '\n';
    }
    // sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    // window.setKeyRepeatEnabled(false);
    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);

    // while (window.isOpen()) 
    // {
    //     sf::Event event;
    //     while (window.pollEvent(event)) 
    //     {
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //     } 
    //     window.clear();
    //     window.draw(shape);
    //     window.display(); 
    // }

    return 0;
}
//"D:\CODE\compilers\mingw64\bin\g++.exe" -c -std=c++17 "D:\CODE\cpp\libraries\SFML-2.5.1\include\SFML\Achibulup_dependencies\LIFOMemoryResource.cpp" -o "D:\CODE\cpp\libraries\SFML-2.5.1\lib\LIFOmemrsc.o" 
