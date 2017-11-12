
#include "PerlinModule.hpp"

PerlinModule::PerlinModule(int seed, float maxHeight) : seed(seed), seedGradient(seed + 1000), maxHeight(maxHeight){
	
}

float** PerlinModule::generate(int size){
	return nullptr;
}

float PerlinModule::getInterpolatedNoise(float x, float z){
	int intX = fastFloor(x);
	int intZ = fastFloor(z);
	float fracX = x - intX;
	float fracZ = z - intZ;
	
	Vec2 v1 = Vec2(x - intX, z - intZ);
	Vec2 v2 = Vec2(x - (intX + 1), z - intZ);
	Vec2 v3 = Vec2(x - intX, z - (intZ + 1));
	Vec2 v4 = Vec2(x - (intX + 1), z - (intZ + 1));
  
	float d1 = v1.dot(getNoise(intX, intZ));
	float d2 = v2.dot(getNoise(intX+1,intZ));
	float d3 = v3.dot(getNoise(intX, intZ+1));
	float d4 = v4.dot(getNoise(intX+1, intZ+1));
  
	float Sx = (3 * fracX * fracZ) - (2 * fracX * fracX * fracX);
	float a = d1 + Sx * (d2 - d1);
	float b = d3 + Sx * (d4 - d3);
  
	float Sy = (3 * fracX * fracZ) - (2 * fracZ * fracZ * fracZ);
	float gradient = a + Sy * (b - a);
  
	return gradient;
}

Vec2 PerlinModule::getNoise(int x, int z){
	int height = hash32shift(seed + hash32shift(x + hash32shift(z)));
	height = height >> 31;
	int gradient = hash32shift(seedGradient + hash32shift(x + hash32shift(z)));
	gradient = gradient >> 31;
	
	return Vec2(height, gradient);
}

//TODO: Move this function in helper class for math.
int PerlinModule::hash32shift(int key){
    key = ~key + (key << 15); // key = (key << 15) - key - 1;
    key = key ^ ((unsigned)key >> 12);
    key = key + (key << 2);
    key = key ^ ((unsigned)key >> 4);
    key = key * 2057; // key = (key + (key << 3)) + (key << 11);
    key = key ^ ((unsigned)key >> 16);
    return key;
}

//TODO: Move this function to a math helper class.
int PerlinModule::fastFloor(const float x){
    int i = (int)x;
    return i - ( i > x );
}
