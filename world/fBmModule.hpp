
#ifndef FBM_MODULE_HPP
#define FBM_MODULE_HPP

#include "PerlinModule.hpp"

class fBmModule{
public:
	fBmModule(int octaves, float roughness, float maxHeight);
	
	float** generate(int size);
	float getInterpolatedHeight(int x, int z);
private:
	PerlinModule* noise;
	
	int octaves;
	float roughness;
	float maxHeight;
	float m_d;
};

#endif
