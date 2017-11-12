
#ifndef QUAD_HPP
#define QUAD_HPP

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

void initAppQuad();

void renderAppQuad();

void cleanUpAppQuad();

#endif
