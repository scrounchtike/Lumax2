
#ifndef APP2_HPP
#define APP2_HPP

#include <iostream>

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif
#include "GLFW/glfw3.h"

#include "../rendering/BasicShader.hpp"

void initApp2();
void cleanUpApp2();

void inputApp2();
void updateApp2();
void renderApp2();

#endif
