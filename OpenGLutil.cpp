
#include "OpenGLutil.hpp"

#include <iostream>

void printExtensions(){
	GLint n, i;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	for (i = 0; i < n; i++) {
		std::cout << glGetStringi(GL_EXTENSIONS, i) << std::endl;
	}
}
