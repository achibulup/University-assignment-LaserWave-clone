build: bin/LaserWave
build_and_run: build run

run:
	(cd "bin" && ./run.sh)

run-with-hitbox: build
	(cd "bin" && ./run-with-hitbox.sh)

clear:
	rm -rf obj/*.o

COMPILER = "g++.exe"
INCLUDE_PATHS = -I"Libraries/SFML-2.5.1/include"
LIBRARY_PATHS = -L"Libraries/SFML-2.5.1/lib"
LIBRARIES = -lLIFOmemrsc -lsfmlext-tilemap -lsfmlext-pixels -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
COMPILER_FLAGS = -g -DDEBUG -std=c++17 -Wall -Wextra -Wpedantic -Wno-unused-function -Wno-unused-parameter -Wno-narrowing

bin/LaserWave: obj/main.o\
    obj/game.a obj/state.a obj/GUI.a obj/asset.a obj/entity.a obj/particle.a obj/geometry.a obj/misc.a
	$(COMPILER) -o "bin/LaserWave" "obj/main.o"\
    "obj/game.a" "obj/state.a" "obj/GUI.a" "obj/asset.a"\
    "obj/entity.a" "obj/particle.a" "obj/geometry.a" "obj/misc.a"\
    $(LIBRARY_PATHS) $(LIBRARIES) -Wl,-subsystem,windows


obj/game.a: obj/Game.o obj/GameDataRef.o obj/EventManager.o
	rm -f "obj/game.a"
	ar rs "obj/game.a" "obj/Game.o" "obj/GameDataRef.o" "obj/EventManager.o"

obj/state.a: obj/StateMachine.o obj/stateRequests.o obj/SplashState.o obj/MenuState.o\
             obj/PlayingState.o obj/PauseState.o obj/LeaderBoardState.o obj/WipeTransitionState.o
	rm -f "obj/state.a"
	ar rs "obj/state.a"\
    "obj/StateMachine.o" "obj/stateRequests.o" "obj/SplashState.o" "obj/MenuState.o"\
    "obj/PlayingState.o" "obj/PauseState.o" "obj/LeaderBoardState.o" "obj/WipeTransitionState.o"

obj/GUI.a: obj/SimpleGUI.o obj/SimpleButton.o obj/GameClock.o obj/SimpleTextBox.o\
           obj/HealthBar.o obj/ActionIndicator.o obj/ScoreSystem.o
	rm -f "obj/GUI.a"
	ar rs "obj/GUI.a"\
    "obj/SimpleGUI.o" "obj/SimpleButton.o" "obj/GameClock.o" "obj/SimpleTextBox.o"\
    "obj/HealthBar.o" "obj/ActionIndicator.o" "obj/ScoreSystem.o"

obj/asset.a: obj/assets.o obj/AssetManager.o
	rm -f "obj/asset.a"
	ar rs "obj/asset.a" "obj/assets.o" "obj/AssetManager.o"

obj/entity.a: obj/Entity.o obj/Player.o obj/Enemies.o obj/enemy_spawn.o obj/BasicEnemy.o obj/FirstBoss.o
	rm -f "obj/entity.a"
	ar rs "obj/entity.a" "obj/Entity.o" "obj/Player.o" "obj/Enemies.o" "obj/enemy_spawn.o" "obj/BasicEnemy.o" "obj/FirstBoss.o"

obj/particle.a: obj/Particles.o obj/KickParticle.o obj/LaserBeam.o obj/ExplodeParticles.o
	rm -f "obj/particle.a"
	ar rs "obj/particle.a" "obj/Particles.o" "obj/KickParticle.o" "obj/LaserBeam.o" "obj/ExplodeParticles.o"

obj/geometry.a: obj/geometry.o obj/geometry_impl.o obj/collisions.o obj/Hitbox.o obj/draw.o
	rm -f "obj/geometry.a"
	ar rs "obj/geometry.a"\
    "obj/geometry.o" "obj/geometry_impl.o" "obj/collisions.o" "obj/Hitbox.o" "obj/draw.o"

obj/misc.a: obj/constants.o obj/paths.o obj/randoms.o
	rm -f "obj/misc.a"
	ar rs "obj/misc.a" "obj/constants.o" "obj/paths.o" "obj/randoms.o"



UNIT_NAME = main

compile:
	$(COMPILER) -c -o "obj/$(UNIT_NAME).o"\
    "src/$(UNIT_NAME).cpp" $(COMPILER_FLAGS) $(INCLUDE_PATHS)


obj/main.o: src/main.cpp
	$(MAKE) compile UNIT_NAME=main

obj/ActionIndicator.o: src/ActionIndicator.cpp
	$(MAKE) compile UNIT_NAME=ActionIndicator

obj/assets.o: src/assets.cpp
	$(MAKE) compile UNIT_NAME=assets

obj/AssetManager.o: src/AssetManager.cpp
	$(MAKE) compile UNIT_NAME=AssetManager

obj/BasicEnemy.o: src/BasicEnemy.cpp
	$(MAKE) compile UNIT_NAME=BasicEnemy

obj/collisions.o: src/collisions.cpp
	$(MAKE) compile UNIT_NAME=collisions

obj/constants.o: src/constants.cpp
	$(MAKE) compile UNIT_NAME=constants

obj/draw.o : src/draw.cpp
	$(MAKE) compile UNIT_NAME=draw

obj/Enemies.o: src/Enemies.cpp
	$(MAKE) compile UNIT_NAME=Enemies

obj/enemy_spawn.o: src/enemy_spawn.cpp
	$(MAKE) compile UNIT_NAME=enemy_spawn

obj/Entity.o: src/Entity.cpp
	$(MAKE) compile UNIT_NAME=Entity

obj/EventManager.o: src/EventManager.cpp
	$(MAKE) compile UNIT_NAME=EventManager

obj/ExplodeParticles.o: src/ExplodeParticles.cpp
	$(MAKE) compile UNIT_NAME=ExplodeParticles

obj/FirstBoss.o: src/FirstBoss.cpp
	$(MAKE) compile UNIT_NAME=FirstBoss

obj/Game.o: src/Game.cpp
	$(MAKE) compile UNIT_NAME=Game

obj/GameClock.o: src/GameClock.cpp
	$(MAKE) compile UNIT_NAME=GameClock

obj/GameDataRef.o: src/GameDataRef.cpp
	$(MAKE) compile UNIT_NAME=GameDataRef

obj/geometry.o : src/geometry.cpp
	$(MAKE) compile UNIT_NAME=geometry

obj/geometry_impl.o : src/geometry_impl.cpp
	$(MAKE) compile UNIT_NAME=geometry_impl

obj/HealthBar.o: src/HealthBar.cpp
	$(MAKE) compile UNIT_NAME=HealthBar

obj/Hitbox.o: src/Hitbox.cpp
	$(MAKE) compile UNIT_NAME=Hitbox

obj/KickParticle.o: src/KickParticle.cpp
	$(MAKE) compile UNIT_NAME=KickParticle
	
obj/LaserBeam.o: src/LaserBeam.cpp
	$(MAKE) compile UNIT_NAME=LaserBeam

obj/LeaderBoardState.o: src/LeaderBoardState.cpp
	$(MAKE) compile UNIT_NAME=LeaderBoardState

obj/MenuState.o: src/MenuState.cpp
	$(MAKE) compile UNIT_NAME=MenuState

obj/Particles.o: src/Particles.cpp
	$(MAKE) compile UNIT_NAME=Particles

obj/paths.o: src/paths.cpp
	$(MAKE) compile UNIT_NAME=paths

obj/PauseState.o: src/PauseState.cpp
	$(MAKE) compile UNIT_NAME=PauseState

obj/Player.o: src/Player.cpp
	$(MAKE) compile UNIT_NAME=Player

obj/PlayingState.o: src/PlayingState.cpp
	$(MAKE) compile UNIT_NAME=PlayingState

obj/randoms.o: src/randoms.cpp
	$(MAKE) compile UNIT_NAME=randoms

obj/ScoreSystem.o: src/ScoreSystem.cpp
	$(MAKE) compile UNIT_NAME=ScoreSystem

obj/SimpleButton.o: src/SimpleButton.cpp
	$(MAKE) compile UNIT_NAME=SimpleButton

obj/SimpleGUI.o: src/SimpleGUI.cpp
	$(MAKE) compile UNIT_NAME=SimpleGUI

obj/SimpleTextBox.o: src/SimpleTextBox.cpp
	$(MAKE) compile UNIT_NAME=SimpleTextBox

obj/SplashState.o: src/SplashState.cpp
	$(MAKE) compile UNIT_NAME=SplashState

obj/StateMachine.o: src/StateMachine.cpp
	$(MAKE) compile UNIT_NAME=StateMachine

obj/stateRequests.o: src/stateRequests.cpp
	$(MAKE) compile UNIT_NAME=stateRequests

obj/WipeTransitionState.o: src/WipeTransitionState.cpp
	$(MAKE) compile UNIT_NAME=WipeTransitionState


src/main.cpp: src/Game.hpp
	touch src/main.cpp

src/ActionIndicator.cpp: src/ActionIndicator.hpp src/draw.hpp
	touch src/ActionIndicator.cpp

src/ActionIndicator.hpp: src/commons.hpp
	touch src/ActionIndicator.hpp

src/assets.cpp: src/assets.hpp
	touch src/assets.cpp

src/assets.hpp: src/commons.hpp src/AssetManager.hpp src/SimpleButton.hpp\
                src/ActionIndicator.hpp
	touch src/assets.hpp

src/AssetManager.cpp: src/AssetManager.hpp
	touch src/AssetManager.cpp

src/AssetManager.hpp: src/commons.hpp src/IdTemplate.hpp
	touch src/AssetManager.hpp

src/BasicEnemy.cpp: src/BasicEnemy.hpp src/draw.hpp
	touch src/BasicEnemy.cpp

src/BasicEnemy.hpp: src/Enemy.hpp
	touch src/BasicEnemy.hpp

src/collisions.cpp: src/collisions.hpp src/geometry_impl.hpp
	touch src/collisions.cpp

src/collisions.hpp: src/Hitbox.hpp
	touch src/collisions.hpp

src/constants.cpp: src/constants.hpp
	touch src/constants.cpp

src/constants.hpp:

src/draw.cpp: src/draw.hpp src/geometry.hpp
	touch src/draw.cpp

src/draw.hpp: src/commons.hpp
	touch src/draw.hpp

src/Enemy.hpp: src/Entity.hpp
	touch src/Enemy.hpp

src/Enemies.cpp: src/Enemies.hpp
	touch src/Enemies.cpp

src/Enemies.hpp: src/Enemy.hpp src/Boss.hpp
	touch src/Enemies.hpp

src/enemy_spawn.cpp: src/enemy_spawn.hpp src/randoms.hpp src/constants.hpp
	touch src/enemy_spawn.cpp

src/enemy_spawn.hpp: src/Enemies.hpp
	touch src/enemy_spawn.hpp

src/Entity.cpp: src/Entity.hpp src/collisions.hpp
	touch src/Entity.cpp

src/Entity.hpp: src/commons.hpp src/Hitbox.hpp
	touch src/Entity.hpp

src/EventManager.cpp: src/EventManager.hpp
	touch src/EventManager.cpp

src/EventManager.hpp: src/commons.hpp
	touch src/EventManager.hpp

src/ExplodeParticles.cpp: src/ExplodeParticles.hpp src/hitbox.hpp
	touch src/ExplodeParticles.cpp

src/ExplodeParticles.hpp: src/commons.hpp src/Particles.hpp
	touch src/ExplodeParticles.hpp

src/FirstBoss.cpp: src/FirstBoss.hpp src/draw.hpp
	touch src/FirstBoss.cpp

src/FirstBoss.hpp: src/Enemy.hpp
	touch src/FirstBoss.hpp

src/Game.cpp: src/Game.hpp src/EventManager.hpp src/assets.hpp
	touch src/Game.cpp

src/Game.hpp: src/State.hpp src/StateMachine.hpp src/AssetManager.hpp src/ScoreSystem.hpp
	touch src/Game.hpp

src/GameClock.cpp: src/GameClock.hpp
	touch src/GameClock.cpp

src/GameClock.hpp:

src/GameDataRef.cpp: src/GameDataRef.hpp src/StateMachine.hpp
	touch src/GameDataRef.cpp

src/GameDataRef.hpp:

src/geometry.cpp: src/geometry.hpp
	touch src/geometry.cpp

src/geometry.hpp: src/commons.hpp src/geometry.inline.hpp
	touch src/geometry.hpp

src/geometry_impl.cpp: src/geometry_impl.hpp
	touch src/geometry_impl.cpp

src/geometry_impl.hpp: src/geometry.hpp src/commons.hpp
	touch src/geometry_impl.hpp

src/HealthBar.cpp: src/HealthBar.hpp src/draw.hpp
	touch src/HealthBar.cpp

src/HealthBar.hpp:

src/Hitbox.cpp: src/Hitbox.hpp
	touch src/Hitbox.cpp

src/Hitbox.hpp: src/geometry.hpp src/Hitbox.inline.hpp
	touch src/Hitbox.hpp

src/KickParticle.cpp: src/KickParticle.hpp
	touch src/KickParticle.cpp

src/KickParticle.hpp: src/Particle.hpp
	touch src/KickParticle.hpp

src/LaserBeam.cpp: src/LaserBeam.hpp src/draw.hpp
	touch src/LaserBeam.cpp

src/LaserBeam.hpp: src/Particle.hpp
	touch src/LaserBeam.hpp

src/LeaderBoardState.cpp: src/state.hpp src/draw.hpp src/SimpleGUI.hpp src/ScoreSystem.hpp
	touch src/LeaderBoardState.cpp

src/MenuState.cpp: src/State.hpp src/SimpleGUI.hpp src/Enemies.hpp src/BasicEnemy.hpp
	touch src/MenuState.cpp

src/Particles.cpp: src/Particles.hpp
	touch src/Particles.cpp

src/Particles.hpp: src/Particle.hpp src/commons.hpp
	touch src/Particles.hpp

src/paths.cpp: src/paths.hpp
	touch src/paths.cpp

src/paths.hpp: 

src/PauseState.cpp: src/State.hpp src/SimpleGUI.hpp src/draw.hpp
	touch src/PauseState.cpp

src/Player.cpp: src/Player.hpp src/draw.hpp
	touch src/Player.cpp

src/Player.hpp: src/Entity.hpp
	touch src/Player.hpp

src/PlayingState.cpp: src/State.hpp src/Player.hpp src/HealthBar.hpp src/Enemies.hpp\
    src/GameClock.hpp src/Particles.hpp src/ActionIndicator.hpp src/collisions.hpp\
    src/KickParticle.hpp src/Particles.hpp src/LaserBeam.hpp src/ExplodeParticles.hpp\
    src/Enemies.hpp src/BasicEnemy.hpp src/ScoreSystem.hpp src/FirstBoss.hpp
	touch src/PlayingState.cpp

src/randoms.cpp: src/randoms.hpp
	touch src/randoms.cpp

src/randoms.hpp:

src/ScoreSystem.cpp: src/ScoreSystem.hpp
	touch src/ScoreSystem.cpp

src/ScoreSystem.hpp: src/commons.hpp src/Score.hpp
	touch src/ScoreSystem.hpp

src/SimpleButton.cpp: src/SimpleButton.hpp
	touch src/SimpleButton.cpp

src/SimpleButton.hpp:

src/SimpleGUI.cpp: src/SimpleGUI.hpp
	touch src/SimpleGUI.cpp

src/SimpleGUI.hpp: src/EventManager.hpp src/SimpleButton.hpp
	touch src/SimpleGUI.hpp

src/SimpleTextBox.cpp: src/SimpleTextBox.hpp
	touch src/SimpleTextBox.cpp

src/SimpleTextBox.hpp:

src/SplashState.cpp: src/State.hpp
	touch src/SplashState.cpp

src/State.hpp: src/GameDataRef.hpp src/commons.hpp src/EventManager.hpp
	touch src/State.hpp

src/StateMachine.cpp: src/StateMachine.hpp src/State.hpp
	touch src/StateMachine.cpp

src/StateMachine.hpp: src/State.hpp src/AssetManager.hpp
	touch src/StateMachine.hpp

src/stateRequests.cpp: src/stateRequests.hpp
	touch src/stateRequests.cpp

src/stateRequests.hpp: src/StateMachine.hpp
	touch src/stateRequests.hpp

src/WipeTransitionState.cpp: src/State.hpp
	touch src/WipeTransitionState.cpp

