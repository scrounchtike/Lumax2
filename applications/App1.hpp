
#ifndef APP1_HPP
#define APP1_HPP

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif
#include "GLFW/glfw3.h"

#include "../rendering/BasicShader.hpp"

#include "../main.hpp"

void initApp1();

void inputApp1();
void updateApp1();
void renderApp1();

void renderPage1();
void renderPage2();

void cleanUpApp1();

//Implementations of different primitives for render.

//Primitive types
//1. Points								 									POINTS
//2. Line Strips														LINE_STRIP
//3. Line Loops															LINE_LOOP
//4. Separate Lines													LINES
//5. Triangle Strips												TRIANGLE_STRIP
//6. Triangle Fans													TRIANGLE_FANS
//7. Separate Triangles											TRIANGLES
//8. Lines with Adjacency										LINES_ADJACENCY
//9. Line Strips with Adjencency						LINE_STRIP_ADJACENCY
//10. Triangles with Adjacency							TRIANGLES_ADJACENCY
//11. Triangle Strips with Adjacency				TRIANGLE_STRIP_ADJACENCY
//12. Separate Patches											PATCHES


//Buffer usage
//1. STREAM_DRAW
//2. STREAM_READ
//3. STREAM_COPY
//4. STATIC_DRAW
//5. STATIC_READ
//6. STATIC_COPY
//7. DYNAMIC_DRAW
//8. DYNAMIC_READ
//9. DYNAMIC_COPY

#endif
