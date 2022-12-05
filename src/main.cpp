#include <SFML/Achibulup_dependencies/common_utils.hpp>
#include <fstream>
#if ACHIBULUP__before_Cpp17
#error This file requires compiler and library support \
for the ISO C++ 2017 standard. This support must be enabled \
with the -std=c++17 compiler options.
#endif
#include "Game.hpp"

bool show_hitbox = false;
std::ofstream log_file("data/log.txt");
int main(int argc, const char* argv[])
{
    for (int i = 0; i < argc; ++i) {
      if (std::string_view(argv[i]) == "-hitbox")
        show_hitbox = true;
    }
    try {
      LaserWave::Game().run();
    }
    catch (const std::exception& e) {
      log_file << "Exception: " << e.what() << '\n';
    }

    return 0;
}

//"D:\CODE\compilers\mingw64\bin\g++.exe" -c -std=c++17 "D:\CODE\cpp\libraries\SFML-2.5.1\include\SFML\Achibulup_dependencies\LIFOMemoryResource.cpp" -o "D:\CODE\cpp\libraries\SFML-2.5.1\lib\LIFOmemrsc.o" 
