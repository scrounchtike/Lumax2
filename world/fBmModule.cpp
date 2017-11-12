
#include "fBmModule.hpp"

#include <cmath>

fBmModule::fBmModule(int octaves, float roughness, float maxHeight) : octaves(octaves), roughness(roughness), maxHeight(maxHeight){
	m_d = (float)pow(2, octaves-1);
	noise = new PerlinModule(666, maxHeight);
}

float** fBmModule::generate(int size){
	float** heights = new float*[size];
	for(int i = 0; i < size; ++i)
		heights[i] = new float[size];
	for(int i = 0; i < size; ++i){
		for(int j = 0; j < size; ++j){
			heights[i][j] = getInterpolatedHeight(i,j);
		}
	}
	return heights;
}

float fBmModule::getInterpolatedHeight(int x, int z){
	float total = 0;
	float freq = 1.0f / m_d;
	float amp = 1;
  
	for(int k = 0; k < octaves; ++k){
		float add = noise->getInterpolatedNoise(x * freq, z * freq);
    
		total += add * amp * maxHeight;
    
		freq *= 2.0;
		amp *= roughness;
	}
	return total;
}
