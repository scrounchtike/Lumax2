#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

#include "Model3D.hpp"

class Rectangle{
public:
	static void initRectangle(bool calcTangents = false);

	static Model3D* getModel();
private:
	static Model3D* model;
};

#endif
