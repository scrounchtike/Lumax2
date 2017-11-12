
#ifndef SHADER_UTILS_HPP
#define SHADER_UTILS_HPP

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

#include <string>

void printAttribute(GLuint program, GLuint index);
void printAttributes(GLuint program);

void printInfoUniformsARCH();
void printUniforms(GLuint program);

std::string resolveType(GLenum type);

#endif
