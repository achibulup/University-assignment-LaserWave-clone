#include "State.hpp"

namespace LaserWave
{

Point getRandomSpawnPosition(const State& state);
bool isOffScreen(const State &state, Point pos);

} // namespace LaserWave