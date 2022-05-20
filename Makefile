build: bin\LaserWave.exe
build_and_run: build
	(cd "bin" && ."\LaserWave.exe")

run_with_hitbox: build
	(cd "bin" && ."\LaserWave.exe" -hitbox)

clear:
	rm -rf obj/*.o

COMPILER = "g++.exe"
INCLUDE_PATHS = -I"Libraries\SFML-2.5.1\include"
LIBRARY_PATHS = -L"Libraries\SFML-2.5.1\lib"
LIBRARIES = -lLIFOmemrsc -lsfmlext-tilemap -lsfmlext-pixels -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
COMPILER_FLAGS = -g -std=c++17 -Wall -Wextra -Wpedantic -Wno-unused-function -Wno-unused-parameter

bin\LaserWave.exe: \
    obj\main.o obj\AssetManager.o obj\BasicEnemy.o obj\constants.o\
    obj\Enemies.o obj\Entity.o obj\EventManager.o obj\assets.o obj\Game.o\
    obj\GameClock.o obj\HealthBar.o obj\KickParticle.o obj\LaserBeam.o\
    obj\MenuState.o obj\Particles.o obj\PauseState.o obj\Player.o\
    obj\PlayingState.o obj\randoms.o obj\SimpleGUI.o obj\SplashState.o\
    obj\StateMachine.o
	$(COMPILER) -o "bin\Debug\LaserWave.exe"\
    "obj\main.o" "obj\AssetManager.o" "obj\BasicEnemy.o" "obj\constants.o"\
    "obj\Entity.o" "obj\Enemies.o" "obj\EventManager.o" "obj\assets.o"\
    "obj\Game.o" "obj\GameClock.o" "obj\HealthBar.o" "obj\KickParticle.o"\
		"obj\Laserbeam.o" "obj\MenuState.o" "obj\Particles.o" "obj\PauseState.o"\
    "obj\Player.o" "obj\PlayingState.o"\
    "obj\randoms.o" "obj\SimpleGUI.o" "obj\SplashState.o" "obj\StateMachine.o"\
    $(LIBRARY_PATHS) $(LIBRARIES) -Wl,-subsystem,windows


UNIT_NAME = main

compile:
	$(COMPILER) -c -o "obj\$(UNIT_NAME).o"\
    "src\$(UNIT_NAME).cpp" $(COMPILER_FLAGS) $(INCLUDE_PATHS)

obj\main.o: src\main.cpp
	$(MAKE) compile UNIT_NAME=main

obj\AssetManager.o: src\AssetManager.cpp
	$(MAKE) compile UNIT_NAME=AssetManager

obj\BasicEnemy.o: src\BasicEnemy.cpp
	$(MAKE) compile UNIT_NAME=BasicEnemy

obj\constants.o: src\constants.cpp
	$(MAKE) compile UNIT_NAME=constants

obj\Enemies.o: src\Enemies.cpp
	$(MAKE) compile UNIT_NAME=Enemies

obj\Entity.o: src\Entity.cpp
	$(MAKE) compile UNIT_NAME=Entity

obj\EventManager.o: src\EventManager.cpp
	$(MAKE) compile UNIT_NAME=EventManager

obj\assets.o: src\assets.cpp
	$(MAKE) compile UNIT_NAME=assets

obj\Game.o: src\Game.cpp
	$(MAKE) compile UNIT_NAME=Game

obj\GameClock.o: src\GameClock.cpp
	$(MAKE) compile UNIT_NAME=GameClock

obj\HealthBar.o: src\HealthBar.cpp
	$(MAKE) compile UNIT_NAME=HealthBar

obj\KickParticle.o: src\KickParticle.cpp
	$(MAKE) compile UNIT_NAME=KickParticle
	
obj\LaserBeam.o: src\LaserBeam.cpp
	$(MAKE) compile UNIT_NAME=LaserBeam

obj\MenuState.o: src\MenuState.cpp
	$(MAKE) compile UNIT_NAME=MenuState

obj\Particles.o: src\Particles.cpp
	$(MAKE) compile UNIT_NAME=Particles

obj\PauseState.o: src\PauseState.cpp
	$(MAKE) compile UNIT_NAME=PauseState

obj\Player.o: src\Player.cpp
	$(MAKE) compile UNIT_NAME=Player

obj\PlayingState.o: src\PlayingState.cpp
	$(MAKE) compile UNIT_NAME=PlayingState

obj\randoms.o: src\randoms.cpp
	$(MAKE) compile UNIT_NAME=randoms

obj\SimpleGUI.o: src\SimpleGUI.cpp
	$(MAKE) compile UNIT_NAME=SimpleGUI

obj\SplashState.o: src\SplashState.cpp
	$(MAKE) compile UNIT_NAME=SplashState

obj\StateMachine.o: src\StateMachine.cpp
	$(MAKE) compile UNIT_NAME=StateMachine


src\main.cpp: src\Game.hpp
	touch src\main.cpp

src\AssetManager.cpp: src\AssetManager.hpp
	touch src\AssetManager.cpp

src\AssetManager.hpp:

src\BasicEnemy.cpp: src\BasicEnemy.hpp
	touch src\BasicEnemy.cpp

src\BasicEnemy.hpp: src\Enemy.hpp src\GameMaths.hpp
	touch src\BasicEnemy.hpp

src\constants.cpp: src\constants.hpp
	touch src\constants.cpp

src\constants.hpp:

src\Enemies.cpp: src\Enemies.hpp src\GameMaths.hpp src\BasicEnemy.hpp
	touch src\Enemies.cpp

src\Enemies.hpp: src\Enemy.hpp
	touch src\Enemies.hpp

src\Entity.cpp: src\Entity.hpp
	touch src\Entity.cpp

src\Entity.hpp:

src\EventManager.cpp: src\EventManager.hpp
	touch src\EventManager.cpp

src\EventManager.hpp:

src\assets.cpp: src\assets.hpp
	touch src\assets.cpp

src\assets.hpp: src\AssetManager.hpp
	touch src\assets.hpp

src\Game.cpp: src\Game.hpp src\EventManager.hpp src\assets.hpp 
	touch src\Game.cpp

src\Game.hpp: src\State.hpp src\StateMachine.hpp src\constants.hpp src\AssetManager.hpp
	touch src\Game.hpp

src\GameClock.cpp: src\GameClock.hpp
	touch src\GameClock.cpp

src\GameClock.hpp:

src\HealthBar.cpp: src\HealthBar.hpp
	touch src\HealthBar.cpp

src\HealthBar.hpp:

src\KickParticle.cpp: src\KickParticle.hpp
	touch src\KickParticle.cpp

src\KickParticle.hpp: src\Particle.hpp src\GameMaths.hpp
	touch src\KickParticle.hpp

src\LaserBeam.cpp: src\LaserBeam.hpp
	touch src\LaserBeam.cpp

src\LaserBeam.hpp: src\Particle.hpp src\GameMaths.hpp
	touch src\LaserBeam.hpp

src\MenuState.cpp: src\MenuState.hpp
	touch src\MenuState.cpp

src\MenuState.hpp: src\State.hpp src\SimpleGUI.hpp
	touch src\MenuState.hpp

src\Particles.cpp: src\Particles.hpp src\GameMaths.hpp
	touch src\Particles.cpp

src\Particles.hpp: src\Particle.hpp
	touch src\Particles.hpp

src\PauseState.cpp: src\PauseState.hpp
	touch src\PauseState.cpp

src\PauseState.hpp: src\State.hpp src\SimpleGUI.hpp
	touch src\PauseState.hpp

src\Player.cpp: src\Player.hpp src\GameMaths.hpp 
	touch src\Player.cpp

src\Player.hpp: src\Entity.hpp src\AssetManager.hpp
	touch src\Player.hpp

src\PlayingState.cpp: src\PlayingState.hpp src\GameMaths.hpp src\constants.hpp\
    src\randoms.hpp src\KickParticle.hpp src\Particles.hpp\
    src\LaserBeam.hpp src\Enemies.hpp src\BasicEnemy.hpp src\Player.hpp
	touch src\PlayingState.cpp

src\PlayingState.hpp: src\State.hpp src\Player.hpp src\HealthBar.hpp
	touch src\PlayingState.hpp

src\randoms.cpp: src\randoms.hpp
	touch src\randoms.cpp

src\randoms.hpp:

src\SimpleGUI.cpp: src\SimpleGUI.hpp
	touch src\SimpleGUI.cpp

src\SimpleGUI.hpp: src\EventManager.hpp src\SimpleButton.hpp
	touch src\SimpleGUI.hpp

src\SplashState.cpp: src\SplashState.hpp src\GameMaths.hpp src\PlayingState.hpp 
	touch src\SplashState.cpp

src\SplashState.hpp: src\State.hpp src\constants.hpp
	touch src\SplashState.hpp

src\StateMachine.cpp: src\StateMachine.hpp src\State.hpp
	touch src\StateMachine.cpp

src\StateMachine.hpp: src\State.hpp src\AssetManager.hpp
	touch src\StateMachine.hpp


