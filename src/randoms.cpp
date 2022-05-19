#include "randoms.hpp"

#include <random>
#include <chrono>

static std::mt19937 generator(std::chrono::steady_clock::now().time_since_epoch().count());

int randInt(int min, int max)
{
    unsigned int range = max - min + 1;
    return generator() % range + min;
}
float randFloat(float min, float max, int precision)
{
    float chunk = (max - min) / precision;
    return min + chunk * randInt(0, precision);
}
int randSign()
{
    return randInt(0, 1) * 2 - 1;
}
bool decide(float probability)
{
    return randFloat(0.f, 1.f) <= probability;
}