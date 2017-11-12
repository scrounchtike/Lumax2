#ifndef BAKE_LIGHT_HPP
#define BAKE_LIGHT_HPP

#include <iostream>
#include <vector>

#include "../math.hpp"
#include "../OBJ_Loader.hpp"

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

struct Triangle{
	Vec3 A, B, C;
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3){
		A = v1; B = v2; C = v3;
	}
};

class BakeLight{
public:
	BakeLight();
	GLuint bake(Vec3 A, Vec3 B, Vec3 C, Vec3 normal);
	//NOTE: Not indexed:
	GLuint bake_model(RawModel model, std::vector<float>& bakedTexCoords);
	void bake_triangle(Vec3 A, Vec3 B, Vec3 C, Vec2 T1, Vec2 T2, Vec2 T3, Vec3 normal, unsigned char* data);
private:
	Vec3 texCoordToPos(Triangle triangle, Vec2 T1, Vec2 T2, Vec2 T3, Vec2 P);

	static const int LIGHTMAP_SIZE;
};

#endif
