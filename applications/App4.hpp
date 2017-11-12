
#ifndef APP4_HPP
#define APP4_HPP

#include "../rendering/Shader.hpp"

#include <cmath>
#include "../math.hpp"

#include "../rendering/Camera.hpp"

class App4{
public:
	static void init();
	
	static void input();
	static void update();
	static void render();
	
	static void cleanUp();

	//private:
	
	static Shader* shader;

	static GLuint vaoID;
	static GLuint vboID;
	static GLuint iboID;

	//Uniforms
	static float transform[16];
	static float view[16];
  static float projection[16];

	//Util mathods
	static float* mul(float* mat1, float* mat2);

	static Camera camera;

	static float temp;
};

#endif
