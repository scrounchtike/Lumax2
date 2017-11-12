
#ifndef BASIC_SHADER_HPP
#define BASIC_SHADER_HPP

#include <string>
#include <fstream>

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

void initBasicShader();
void cleanUpBasicShader();

void bindBasicShader();

void checkErrorShader(GLuint shader, GLenum pname);
void checkErrorProgram(GLuint program, GLenum pname);

std::string loadShaderFile(const std::string& fileName);

class BasicShader{
public:
	static GLuint program;
	static GLuint vertexShader, fragmentShader;
};

#endif

