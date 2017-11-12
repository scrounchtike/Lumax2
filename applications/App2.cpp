
#include "App2.hpp"

/*
This app demonstrates indexed draw calls and objects, commonly referred to as elements. 
Like the previous app, this app shows the multiple ways to draw primitives but this time
with indexed data. Indexed primitives may save up to 50% of memory space in a best case scenario.
More often than not, they are required to save memory and procesing time in large collection
of primitives; like meshes. 
*/

GLuint vaoIDtriangles2;
GLuint vboIDtriangles2;
GLuint iboIDtriangles2;

GLuint vaoIDlines2;
GLuint vboIDlines2;
GLuint iboIDlines2;

GLuint vaoIDlineStrips2;
GLuint vboIDlineStrips2;
GLuint iboIDlineStrips2;

GLuint vaoIDlineLoop2;
GLuint vboIDlineLoop2;
GLuint iboIDlineLoop2;

void initApp2(){
	//Init BasicShader
	initBasicShader();
	//Init data for indexed triangles rendering
	float verticesTriangles[8] = {-0.5f, 0.25f, -0.25f, 0.25f, -0.25f, 0.5f, -0.5f, 0.5f};
	int indicesTriangles[6] = {0,1,2,2,3,0};

	glGenBuffers(1, &vboIDtriangles2);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDtriangles2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, verticesTriangles, GL_STATIC_DRAW);

	glGenBuffers(1, &iboIDtriangles2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDtriangles2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6, indicesTriangles, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDtriangles2);
	glBindVertexArray(vaoIDtriangles2);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Init data for indexed lines rendering
	float verticesLines[8] = {0.5f, 0.25f, 0.25f, 0.25f, 0.25f, 0.5f, 0.5f, 0.5f};
	int indicesLines[8] = {0,1,1,2,2,3,3,0};

	glGenBuffers(1, &vboIDlines2);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDlines2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, verticesLines, GL_STATIC_DRAW);

	glGenBuffers(1, &iboIDlines2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDlines2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*8, indicesLines, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDlines2);
	glBindVertexArray(vaoIDlines2);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Init data for indexed line strips rendering
	float verticesLineStrips[8] = {-0.5f, -0.25f, -0.25f, -0.25f, -0.25f, -0.5f, -0.5f, -0.5f};
	int indicesLineStrips[6] = {0,1,2,3,0,2};

	glGenBuffers(1, &vboIDlineStrips2);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDlineStrips2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, verticesLineStrips, GL_STATIC_DRAW);

	glGenBuffers(1, &iboIDlineStrips2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDlineStrips2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6, indicesLineStrips, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDlineStrips2);
	glBindVertexArray(vaoIDlineStrips2);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Init data for indexed line loop rendering
	float verticesLineLoop[10] = {0.25f, -0.5f, 0.5f, -0.5f, 0.5f, -0.25f, 0.25f, -0.25f, 0.375f, -0.12f};
	int indicesLineLoop[9] = {0,1,2,4,3,1,0,2,3};

	glGenBuffers(1, &vboIDlineLoop2);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDlineLoop2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*10, verticesLineLoop, GL_STATIC_DRAW);

	glGenBuffers(1, &iboIDlineLoop2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDlineLoop2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*9, indicesLineLoop, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDlineLoop2);
	glBindVertexArray(vaoIDlineLoop2);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void inputApp2(){

}

void updateApp2(){

}

void renderApp2(){
	bindBasicShader();

	//Render indexed triangles
	glBindVertexArray(vaoIDtriangles2);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDtriangles2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	//Render indexed lines
	glBindVertexArray(vaoIDlines2);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDlines2);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	//Render indexed line strips
	glBindVertexArray(vaoIDlineStrips2);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDlineStrips2);
	glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	//Render indexed line loop
	glBindVertexArray(vaoIDlineLoop2);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDlineLoop2);
	glDrawElements(GL_LINE_LOOP, 9, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void cleanUpApp2(){
	//Clean up basic shader
	cleanUpBasicShader();
	//Delete buffers
	glDeleteVertexArrays(1, &vaoIDtriangles2);
	glDeleteBuffers(1, &vboIDtriangles2);
	glDeleteBuffers(1, &iboIDtriangles2);
	glDeleteVertexArrays(1, &vaoIDlines2);
	glDeleteBuffers(1, &vboIDlines2);
	glDeleteBuffers(1, &iboIDlines2);
	glDeleteVertexArrays(1, &vaoIDlineStrips2);
	glDeleteBuffers(1, &vboIDlineStrips2);
	glDeleteBuffers(1, &iboIDlineStrips2);
}
