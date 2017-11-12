
#include "Quad.hpp"

GLuint vaoIDAppQuad;
GLuint vboIDAppQuad;

void initAppQuad(){
	float vertices[12] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
												0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f};
	
	glGenBuffers(1, &vboIDAppQuad);
	glBindBuffer(GL_ARRAY_BUFFER, vboIDAppQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoIDAppQuad);
	glBindVertexArray(vaoIDAppQuad);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void renderAppQuad(){
	glBindVertexArray(vaoIDAppQuad);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void cleanUpAppQuad(){
	glDeleteBuffers(1, &vboIDAppQuad);
	glDeleteVertexArrays(1, &vaoIDAppQuad);
}
