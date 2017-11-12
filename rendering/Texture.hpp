
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

#include "stb_image.h"

class Texture{
public:
	Texture(GLuint id);
	Texture(const std::string& fileName);

	void bind();
private:
	GLuint texture_ID;
};

#endif
