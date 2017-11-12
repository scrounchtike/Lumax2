
#ifndef RADIOSITY_HPP
#define RADIOSITY_HPP

#include "../rendering/Shader.hpp"
#include "../rendering/Camera.hpp"

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

#include "../math.hpp"

class Radiosity{
public:
	static void initRadiosity();
	static void cleanUpRadiosity();

	static void input();
	static void update();
	static void render();
	
private:
	static Shader* shader;

	static GLuint vaoID_light;
	static GLuint vboID_light;

	static GLuint vaoID_wall;
	static GLuint vboID_wall;
	static GLuint iboID_wall;

	static Mat4 transform_light;

	static Camera camera;
};

#endif
