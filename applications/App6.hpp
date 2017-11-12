
#ifndef APP6_HPP
#define APP6_HPP

#include <string>
#include <fstream>

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

#include "../math.hpp"
#include "../rendering/Camera.hpp"
#include "../rendering/Shader.hpp"
#include "../rendering/Texture.hpp"
#include "../RL/Model3D.hpp"
#include "../rendering/BakedModel3D.hpp"

class App6{
public:
	static void init();

	static void input();
	static void update();
	static void render();

	static void cleanUp();

	static void bakeLight();
private:
	static GLuint vaoID;
	static GLuint vboID;
	static GLuint iboID;
	static GLuint vbotID;
	static GLuint vbonID;

	static int numVertices;

	static Shader* shader;
	static Shader* shader_light;
	static Camera* camera;
	static Texture* texture;
	static Texture* texture_gold;
	static GLuint   baked_lighting;
	static GLuint   baked_triangle;
	static GLuint   baked_sphere;

	static BakedModel3D* sphere;
	static BakedModel3D* cube;
	static Model3D* triangle;
};

#endif
