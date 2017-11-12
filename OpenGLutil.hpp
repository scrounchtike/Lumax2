
#ifndef OPENGL_UTIL_HPP
#define OPENGL_UTIL_HPP

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

void printExtensions();

#endif
