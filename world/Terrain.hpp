
#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <vector>

#include "../math.hpp"

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

class Terrain{
public:
	Terrain(Vec3 position, int LOD);

	void generate();
	void regenerate();
	void createMesh();
	void initGlRenderData();

	void input();
	void update();
	void render();

	enum CHUNK_STATE{
		SEEN,
		UNSEEN,
		LOADING,
		ADD
	};
	CHUNK_STATE state;

	void setPosition(Vec3 position){ this->position = position; }
	Vec3 getPosition() const{ return position; }
	void setLOD(int LOD){ this->LOD = LOD; }
	int getLOD() const{ return LOD; }
private:
	//This is temporary data that gets overwritten each time a terrain is reloaded
	int numIndices;
	std::vector<Vec3> vertices;
	std::vector<int> indices;
	
	int LOD = 1;
	Vec3 position;

	GLuint vaoID;
	GLuint vboID;
	GLuint vbotID;
	GLuint vbonID;
	GLuint iboID;
};

#endif
