
#ifndef PERLIN_MODULE_HPP
#define PERLIN_MODULE_HPP

#include "../math.hpp"

class PerlinModule{
public:
	PerlinModule(int seed, float maxHeight);

	float** generate(int size);
	float getInterpolatedNoise(float x, float z);

	int hash32shift(int key);
	int fastFloor(const float x);
	Vec2 getNoise(int x, int z);
private:
	float seed;
	float seedGradient;
	float maxHeight;
};

#endif
