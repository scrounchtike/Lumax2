#ifndef CUBE_HPP
#define CUBE_HPP

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

class Cube{
public:

	static void initCube();

	static void bindForRender();
	static void render();
	static void unbindForRender();

private:

	static GLuint vaoID;
	static GLuint vboID;
	static GLuint vbotID;
	static GLuint iboID;
	
};

#endif
