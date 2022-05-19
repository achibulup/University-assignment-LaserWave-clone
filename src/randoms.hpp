#ifndef RANDOMS_HPP_INCLUDED
#define RANDOMS_HPP_INCLUDED

int randInt(int min, int max);
float randFloat(float min, float max, int precision = 1024);
int randSign();
bool decide(float probability);

#endif // RANDOMS_HPP_INCLUDED
