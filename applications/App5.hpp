#ifndef APP5_HPP
#define APP5_HPP

#include "../rendering/Shader.hpp"
#include "../rendering/Camera.hpp"
#include "../rendering/Cube.hpp"

#include "../math.hpp"

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

class App5{
public:
	static void init();
	
	static void input();
	static void update();
	static void render();

	static void cleanUp();
	
private:
	static Shader* shader;
	static Camera camera;

	static GLuint textureID;

	static float temp;
	
};

#endif
