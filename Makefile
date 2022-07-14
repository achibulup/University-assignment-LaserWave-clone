build: bin\LaserWave.exe
build_and_run: build run

run:
	(cd "bin" && ."\LaserWave.exe")

run_with_hitbox: build
	(cd "bin" && ."\LaserWave.exe" -hitbox)

clear:
	rm -rf obj/*.o

COMPILER = "g++.exe"
INCLUDE_PATHS = -I"Libraries\SFML-2.5.1\include"
LIBRARY_PATHS = -L"Libraries\SFML-2.5.1\lib"
LIBRARIES = -lLIFOmemrsc -lsfmlext-tilemap -lsfmlext-pixels -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
COMPILER_FLAGS = -g -DDEBUG -std=c++17 -Wall -Wextra -Wpedantic -Wno-unused-function -Wno-unused-parameter -Wno-narrowing

bin\LaserWave.exe: \
    obj\main.o obj\ActionIndicator.o obj\AssetManager.o obj\assets.o obj\BasicEnemy.o obj\collisions.o obj\constants.o \
    obj\draw_convex.o obj\Enemies.o obj\Entity.o obj\EventManager.o\
    obj\Game.o obj\GameClock.o obj\GameDataRef.o\
    obj\geometry.o obj\geometry_impl.o obj\HealthBar.o obj\Hitbox.o obj\KickParticle.o obj\LaserBeam.o\
    obj\MenuState.o obj\Particles.o obj\PauseState.o obj\Player.o\
    obj\PlayingState.o obj\randoms.o obj\SimpleGUI.o obj\SplashState.o\
    obj\StateMachine.o obj\stateRequests.o obj\WipeTransitionState.o
	$(COMPILER) -o "bin\LaserWave.exe"\
    "obj\main.o" "obj\ActionIndicator.o" "obj\AssetManager.o" "obj\assets.o" "obj\BasicEnemy.o" "obj\collisions.o" "obj\constants.o"\
    "obj\draw_convex.o"\
    "obj\Entity.o" "obj\Enemies.o" "obj\EventManager.o"\
    "obj\Game.o" "obj\GameClock.o" "obj\GameDataRef.o" "obj\geometry.o" "obj\geometry_impl.o" "obj\HealthBar.o"\
    "obj\Hitbox.o" "obj\KickParticle.o" "obj\Laserbeam.o" "obj\MenuState.o" "obj\Particles.o"\
    "obj\PauseState.o" "obj\Player.o" "obj\PlayingState.o" "obj\randoms.o"\
    "obj\SimpleGUI.o" "obj\SplashState.o" "obj\StateMachine.o"\
    "obj\stateRequests.o" "obj\WipeTransitionState.o"\
    $(LIBRARY_PATHS) $(LIBRARIES) -Wl,-subsystem,windows


UNIT_NAME = main

compile:
	$(COMPILER) -c -o "obj\$(UNIT_NAME).o"\
    "src\$(UNIT_NAME).cpp" $(COMPILER_FLAGS) $(INCLUDE_PATHS)

obj\main.o: src\main.cpp
	$(MAKE) compile UNIT_NAME=main

obj\ActionIndicator.o: src\ActionIndicator.cpp
	$(MAKE) compile UNIT_NAME=ActionIndicator

obj\assets.o: src\assets.cpp
	$(MAKE) compile UNIT_NAME=assets

obj\AssetManager.o: src\AssetManager.cpp
	$(MAKE) compile UNIT_NAME=AssetManager

obj\BasicEnemy.o: src\BasicEnemy.cpp
	$(MAKE) compile UNIT_NAME=BasicEnemy

obj\collisions.o: src\collisions.cpp
	$(MAKE) compile UNIT_NAME=collisions

obj\constants.o: src\constants.cpp
	$(MAKE) compile UNIT_NAME=constants

obj\draw_convex.o : src\draw_convex.cpp
	$(MAKE) compile UNIT_NAME=draw_convex

obj\Enemies.o: src\Enemies.cpp
	$(MAKE) compile UNIT_NAME=Enemies

obj\Entity.o: src\Entity.cpp
	$(MAKE) compile UNIT_NAME=Entity

obj\EventManager.o: src\EventManager.cpp
	$(MAKE) compile UNIT_NAME=EventManager

obj\Game.o: src\Game.cpp
	$(MAKE) compile UNIT_NAME=Game

obj\GameClock.o: src\GameClock.cpp
	$(MAKE) compile UNIT_NAME=GameClock

obj\GameDataRef.o: src\GameDataRef.cpp
	$(MAKE) compile UNIT_NAME=GameDataRef

obj\geometry.o : src\geometry.cpp
	$(MAKE) compile UNIT_NAME=geometry

obj\geometry_impl.o : src\geometry_impl.cpp
	$(MAKE) compile UNIT_NAME=geometry_impl

obj\HealthBar.o: src\HealthBar.cpp
	$(MAKE) compile UNIT_NAME=HealthBar

obj\Hitbox.o: src\Hitbox.cpp
	$(MAKE) compile UNIT_NAME=Hitbox

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

obj\stateRequests.o: src\stateRequests.cpp
	$(MAKE) compile UNIT_NAME=stateRequests

obj\WipeTransitionState.o: src\WipeTransitionState.cpp
	$(MAKE) compile UNIT_NAME=WipeTransitionState


src\main.cpp: src\Game.hpp
	touch src\main.cpp

src\ActionIndicator.cpp: src\ActionIndicator.hpp src\draw_convex.hpp
	touch src\ActionIndicator.cpp

src\ActionIndicator.hpp: src\commons.hpp
	touch src\ActionIndicator.hpp

src\assets.cpp: src\assets.hpp
	touch src\assets.cpp

src\assets.hpp: src\commons.hpp src\AssetManager.hpp src\SimpleButton.hpp\
                src\ActionIndicator.hpp
	touch src\assets.hpp

src\AssetManager.cpp: src\AssetManager.hpp
	touch src\AssetManager.cpp

src\AssetManager.hpp: src\commons.hpp src\IdTemplate.hpp
	touch src\AssetManager.hpp

src\BasicEnemy.cpp: src\BasicEnemy.hpp
	touch src\BasicEnemy.cpp

src\BasicEnemy.hpp: src\Enemy.hpp src\draw_convex.hpp
	touch src\BasicEnemy.hpp

src\collisions.cpp: src\collisions.hpp src\geometry_impl.hpp
	touch src\collisions.cpp

src\collisions.hpp: src\Hitbox.hpp
	touch src\collisions.hpp

src\constants.cpp: src\constants.hpp
	touch src\constants.cpp

src\constants.hpp:

src\draw_convex.cpp: src\draw_convex.hpp
	touch src\draw_convex.cpp

src\draw_convex.hpp: src\Entity.hpp
	touch src\draw_convex.hpp

src\Enemy.hpp: src\Entity.hpp
	touch src\Enemy.hpp

src\Enemies.cpp: src\Enemies.hpp src\BasicEnemy.hpp
	touch src\Enemies.cpp

src\Enemies.hpp: src\Enemy.hpp
	touch src\Enemies.hpp

src\Entity.cpp: src\Entity.hpp src\collisions.hpp
	touch src\Entity.cpp

src\Entity.hpp: src\commons.hpp src\Hitbox.hpp
	touch src\Entity.hpp

src\EventManager.cpp: src\EventManager.hpp
	touch src\EventManager.cpp

src\EventManager.hpp: src\commons.hpp
	touch src\EventManager.hpp

src\Game.cpp: src\Game.hpp src\EventManager.hpp src\assets.hpp src\SplashState.hpp 
	touch src\Game.cpp

src\Game.hpp: src\State.hpp src\StateMachine.hpp src\AssetManager.hpp
	touch src\Game.hpp

src\GameClock.cpp: src\GameClock.hpp
	touch src\GameClock.cpp

src\GameClock.hpp:

src\GameDataRef.cpp: src\GameDataRef.hpp src\StateMachine.hpp
	touch src\GameDataRef.cpp

src\GameDataRef.hpp:

src\geometry.cpp: src\geometry.hpp
	touch src\geometry.cpp

src\geometry.hpp: src\commons.hpp src\geometry.inline.hpp
	touch src\geometry.hpp

src\geometry_impl.cpp: src\geometry_impl.hpp
	touch src\geometry_impl.cpp

src\geometry_impl.hpp: src\geometry.hpp src\commons.hpp
	touch src\geometry_impl.hpp

src\HealthBar.cpp: src\HealthBar.hpp src\draw_convex.hpp
	touch src\HealthBar.cpp

src\HealthBar.hpp:

src\Hitbox.cpp: src\Hitbox.hpp
	touch src\Hitbox.cpp

src\Hitbox.hpp: src\geometry.hpp src\Hitbox.inline.hpp
	touch src\Hitbox.hpp

src\KickParticle.cpp: src\KickParticle.hpp
	touch src\KickParticle.cpp

src\KickParticle.hpp: src\Particle.hpp
	touch src\KickParticle.hpp

src\LaserBeam.cpp: src\LaserBeam.hpp src\draw_convex.hpp
	touch src\LaserBeam.cpp

src\LaserBeam.hpp: src\Particle.hpp
	touch src\LaserBeam.hpp

src\MenuState.cpp: src\MenuState.hpp src\WipeTransitionState.hpp\
                   src\stateRequests.hpp src\assets.hpp src\PlayingState.hpp
	touch src\MenuState.cpp

src\MenuState.hpp: src\State.hpp src\SimpleGUI.hpp
	touch src\MenuState.hpp

src\Particles.cpp: src\Particles.hpp
	touch src\Particles.cpp

src\Particles.hpp: src\Particle.hpp src\commons.hpp
	touch src\Particles.hpp

src\PauseState.cpp: src\PauseState.hpp src\WipeTransitionState.hpp\
                    src\stateRequests.hpp src\assets.hpp src\PlayingState.hpp\
                    src\draw_convex.hpp
	touch src\PauseState.cpp

src\PauseState.hpp: src\State.hpp src\SimpleGUI.hpp
	touch src\PauseState.hpp

src\Player.cpp: src\Player.hpp
	touch src\Player.cpp

src\Player.hpp: src\draw_convex.hpp src\Entity.hpp
	touch src\Player.hpp

src\PlayingState.cpp: src\PlayingState.hpp src\collisions.hpp\
    src\KickParticle.hpp src\Particles.hpp src\LaserBeam.hpp\
    src\Enemies.hpp src\BasicEnemy.hpp src\stateRequests.hpp src\assets.hpp\
    src\PauseState.hpp src\WipeTransitionState.hpp
	touch src\PlayingState.cpp

src\PlayingState.hpp: src\State.hpp src\Player.hpp src\HealthBar.hpp src\Enemies.hpp\
                      src\GameClock.hpp src\Particles.hpp src\ActionIndicator.hpp
	touch src\PlayingState.hpp

src\randoms.cpp: src\randoms.hpp
	touch src\randoms.cpp

src\randoms.hpp:

src\SimpleGUI.cpp: src\SimpleGUI.hpp
	touch src\SimpleGUI.cpp

src\SimpleGUI.hpp: src\EventManager.hpp src\SimpleButton.hpp
	touch src\SimpleGUI.hpp

src\SplashState.cpp: src\SplashState.hpp src\stateRequests.hpp src\assets.hpp src\MenuState.hpp
	touch src\SplashState.cpp

src\SplashState.hpp: src\State.hpp
	touch src\SplashState.hpp

src\State.hpp: src\GameDataRef.hpp src\commons.hpp src\EventManager.hpp
	touch src\State.hpp

src\StateMachine.cpp: src\StateMachine.hpp src\State.hpp
	touch src\StateMachine.cpp

src\StateMachine.hpp: src\State.hpp src\AssetManager.hpp
	touch src\StateMachine.hpp

src\stateRequests.cpp: src\stateRequests.hpp
	touch src\stateRequests.cpp

src\stateRequests.hpp: src\StateMachine.hpp
	touch src\stateRequests.hpp

src\WipeTransitionState.cpp: src\WipeTransitionState.hpp src\stateRequests.hpp
	touch src\WipeTransitionState.cpp

src\WipeTransitionState.hpp: src\State.hpp
	touch src\WipeTransitionState.hpp


