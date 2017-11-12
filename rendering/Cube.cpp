
#include "Cube.hpp"

GLuint Cube::vaoID;
GLuint Cube::vboID;
GLuint Cube::vbotID;
GLuint Cube::iboID;

void Cube::initCube(){
	//Cube data
	float vertices[24] = {-0.5f, 0.5f,-0.5f,
												 0.5f, 0.5f,-0.5f,
												 0.5f,-0.5f,-0.5f,
												-0.5f,-0.5f,-0.5f,
												-0.5f, 0.5f, 0.5f,
												 0.5f, 0.5f, 0.5f,
												 0.5f,-0.5f, 0.5f,
												-0.5f,-0.5f, 0.5f};

	float texCoords[16] = {0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0};

	int indices[36] = {3,2,1,1,0,3,   //Front face
										 0,1,5,5,4,0,   //Top face
										 2,6,5,5,1,2,   //Right face
										 7,3,0,0,4,7,   //Left face
										 6,7,4,4,5,6,   //Back face
										 7,6,2,2,3,7};  //Bottom face

	//Init cube buffers
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*24, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbotID);
	glBindBuffer(GL_ARRAY_BUFFER, vbotID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, texCoords, GL_STATIC_DRAW);

	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*36, indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbotID);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cube::bindForRender(){
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
}

void Cube::render(){
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Cube::unbindForRender(){
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}
