
#include "App1.hpp"

#include "../RAL/Window.hpp"

#include <iostream>
#include <cmath>

GLuint vaoIDpoints, vboIDpoints;
GLuint vaoIDlineStrips, vboIDlineStrips;
GLuint vaoIDlineLoop, vboIDlineLoop;
GLuint vaoIDlines, vboIDlines;
GLuint vaoIDtriangleStrips, vboIDtriangleStrips;
GLuint vaoIDtriangleFan, vboIDtriangleFan;

int pageNumber;

void initApp1(){
	//Init Basic Shader
	initBasicShader();
	//Begin with first page
	pageNumber = 1;
	//Point size
	glPointSize(10.0f);
	
	//Create buffers for points
	float verticesPoints[8] = {0.25f, 0.25f, 0.5f, 0.25f, 0.5f, 0.5f, 0.25f, 0.5f};
	
	glGenBuffers(1, &vboIDpoints);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDpoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, verticesPoints, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDpoints);
	glBindVertexArray(vaoIDpoints);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Create buffers for line strips
	float verticesLineStrips[8] = {-0.25f, -0.25f, -0.5f, -0.25f, -0.5f, -0.5f, -0.25f, -0.5f};
	
	glGenBuffers(1, &vaoIDlineStrips);
	glBindBuffer(GL_ARRAY_BUFFER, vaoIDlineStrips);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, verticesLineStrips, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDlineStrips);
	glBindVertexArray(vaoIDlineStrips);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Create buffers for line loop
	float verticesLineLoop[8] = {0.25f, -0.25f, 0.5f, -0.25f, 0.5f, -0.5f, 0.25f, -0.5f};
	
	glGenBuffers(1, &vaoIDlineLoop);
	glBindBuffer(GL_ARRAY_BUFFER, vaoIDlineLoop);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, verticesLineLoop, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDlineLoop);
	glBindVertexArray(vaoIDlineLoop);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Create buffers for lines
	float verticesLines[8] = {-0.5f, 0.5f, -0.5f, 0.25f, -0.25f, 0.5f, -0.25f, 0.25f};
	
	glGenBuffers(1, &vaoIDlines);
	glBindBuffer(GL_ARRAY_BUFFER, vaoIDlines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, verticesLines, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDlines);
	glBindVertexArray(vaoIDlines);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Create buffers for triangle strips
	float verticesTriangleStrips[10] = {0.25f, 0.25f, 0.25f, 0.5f, 0.5f, 0.25f, 0.5f, 0.5f, 0.75f, 0.25f};
	
	glGenBuffers(1, &vaoIDtriangleStrips);
	glBindBuffer(GL_ARRAY_BUFFER, vaoIDtriangleStrips);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*10, verticesTriangleStrips, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDtriangleStrips);
	glBindVertexArray(vaoIDtriangleStrips);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Create buffers for triangle strips
	float verticesTriangleFan[10] = {-0.5f, -0.5f, -0.5f, -0.25f, -0.375f, -0.28f, -0.28f, -0.375f, -0.25f, -0.5f};
	
	glGenBuffers(1, &vaoIDtriangleFan);
	glBindBuffer(GL_ARRAY_BUFFER, vaoIDtriangleFan);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*10, verticesTriangleFan, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDtriangleFan);
	glBindVertexArray(vaoIDtriangleFan);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

void inputApp1(){
	//Update page number based on key input
	if(wasKeyJustPressed(GLFW_KEY_1))
		pageNumber = 1;
	else if(wasKeyJustPressed(GLFW_KEY_2))
		pageNumber = 2;
}

void updateApp1(){
	
}

void renderApp1(){
	//TODO: Reorganize faces so that we can enable culling back on.
	glDisable(GL_CULL_FACE);
	
	bindBasicShader();
	switch (pageNumber){
	case 1:
		renderPage1();
		break;
	case 2:
		renderPage2();
		break;
	}
}

void renderPage1(){
	//First render two points without any vertex array attributes
	//For some reason, we still have to bind the vertex array object for the draw to work.
	glBindVertexArray(vaoIDpoints);
	//First Point
	glVertexAttrib2f(0, -0.5f, 0.5f);
	glDrawArrays(GL_POINTS, 0, 1);
	//Second Point
	glVertexAttrib2f(0, -0.6f, 0.5f);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);

	//Using a VAO to store attributes, we can render more than one primitive at once.
	glBindVertexArray(vaoIDpoints);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_POINTS, 0, 4);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	//Rendering line strips
	glBindVertexArray(vaoIDlineStrips);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	//Rendering line loop
	glBindVertexArray(vaoIDlineLoop);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);	
}

void renderPage2(){
	//Render lines
	glBindVertexArray(vaoIDlines);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, 4);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	
	//Render triangle strips
	glBindVertexArray(vaoIDtriangleStrips);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
	//Render the edges of the triangles so that the strip are easier to see
	//TODO: Render contour of triangles so that strips are easier to see.
	//glDrawArrays(GL_LINE_STRIP, 0, 5);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	//Render triangle fan
	glBindVertexArray(vaoIDtriangleFan);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
	//Render edges of triangles
	//TODO: Render contour of triangles so that the fan is easier to see.
	//glDrawArrays(GL_LINE_STRIP, 0, 5);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void cleanUpApp1(){
	//Clean up basic shader
	cleanUpBasicShader();
	//Delete VAOs and VBOs
	glDeleteVertexArrays(1, &vaoIDpoints);
	glDeleteBuffers(1, &vboIDpoints);
	glDeleteVertexArrays(1, &vaoIDlineStrips);
	glDeleteBuffers(1, &vboIDlineStrips);
	glDeleteVertexArrays(1, &vaoIDlineLoop);
	glDeleteBuffers(1, &vboIDlineLoop);
	glDeleteVertexArrays(1, &vaoIDlines);
	glDeleteBuffers(1, &vboIDlines);
	glDeleteVertexArrays(1, &vaoIDtriangleStrips);
	glDeleteBuffers(1, &vboIDtriangleStrips);
 	glDeleteVertexArrays(1, &vaoIDtriangleFan);
	glDeleteBuffers(1, &vboIDtriangleFan);
}
