
#ifndef VOXEL_TERRAIN_HPP
#define VOXEL_TERRAIN_HPP

#include <vector>

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif
#include "../math.hpp"

class VoxelTerrain{
public:
	VoxelTerrain();

	void checkCollisions(Vec3 oldPos, Vec3& posDelta);

	void generateRenderData();

	void input();
	void update();
	void render();
private:
	unsigned int height;
	unsigned int size;

	void addIndices(std::vector<int>& indices, int& index);
	void addNormals(std::vector<Vec3>& normals, Vec3 normal);
	void addAO(std::vector<float>& AO, float occlusion);

	float calcAO(int side1, int side2, int corner);
	int testForBlock(int h, int x, int z);
	
	unsigned int*** blocks;

	unsigned int numIndices;
	GLuint vaoID;
	GLuint vboID;
	GLuint vbonID;
	GLuint vboaoID;
	GLuint iboID;
};

#endif
