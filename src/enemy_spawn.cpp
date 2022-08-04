#include "enemy_spawn.hpp"
#include "constants.hpp"
#include "randoms.hpp"

namespace LaserWave
{

Point getRandomSpawnPosition(const State& state)
{
    ///get a random position outside the window
    const float TOP_Y = -ENEMY_SPAWN_MARGIN.y;
    const float BOTTOM_Y = state.getWindow().getSize().y + ENEMY_SPAWN_MARGIN.y;
    const float LEFT_X = -ENEMY_SPAWN_MARGIN.x;
    const float RIGHT_X = state.getWindow().getSize().x + ENEMY_SPAWN_MARGIN.x;

    ///decide wich side to spawn on
    enum Side : int {
        TOP_OR_LEFT,
        BOTTOM_OR_RIGHT
    };
    Side side = Side(randInt(0, 1));
    Vec2 res;
    switch (side) {
      case TOP_OR_LEFT:
        res = {LEFT_X, TOP_Y};
      break;
      case BOTTOM_OR_RIGHT:
        res = {RIGHT_X, BOTTOM_Y};
      break;
    }

    ///get a random position on the side
    float UNROLL_SIDES_LENGTH = (BOTTOM_Y - TOP_Y) + (RIGHT_X - LEFT_X);
    float pos = randFloat(0.f, UNROLL_SIDES_LENGTH, 4096);
    if (pos < (BOTTOM_Y - TOP_Y))
      res.y = TOP_Y + pos;
    else
      res.x = LEFT_X + (pos - (BOTTOM_Y - TOP_Y));

    return res;
}

bool isOffScreen(const State &state, Point pos)
{
    const float TOP_Y = -ENEMY_FILTER_MARGIN.y;
    const float BOTTOM_Y = state.getWindow().getSize().y + ENEMY_FILTER_MARGIN.y;
    const float LEFT_X = -ENEMY_FILTER_MARGIN.x;
    const float RIGHT_X = state.getWindow().getSize().x + ENEMY_FILTER_MARGIN.x;
    return pos.x < LEFT_X || pos.x > RIGHT_X || pos.y < TOP_Y || pos.y > BOTTOM_Y;
}

} // namespace LaserWave