
#ifndef APP3_HPP
#define APP3_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

class App3{
public:
	static GLuint program;
	static GLuint vertexShader;
	static GLuint fragmentShader;

	//uniform locations
	static GLint uniformTranslationLocation;
	static GLint uniformZLocation;
	
	static void init();

	static void input();
	static void update();
	static void render();

	static void cleanUp();

	static GLint addUniform(const GLchar* name);

	static void createProgramFromBinaries(GLuint prevProgram);
	
	static std::string loadShader(const std::string& fileName);
	static void checkErrorShader(GLuint shader, GLenum pname);
	static void checkErrorProgram(GLuint program, GLenum pname);
};

#endif
