
#include "Model2D.hpp"

Model2D::Model2D(const std::vector<float>& vertices) : hasTexCoords(false), isIndexed(false){
	createVertexArray();
	createVBO(vertices, 0, 2);
	unbindBuffers();
	numVertices = vertices.size();
}

Model2D::Model2D(const std::vector<float>& vertices, const std::vector<float>& texCoords) : hasTexCoords(true), isIndexed(false){
	createVertexArray();
	createVBO(vertices, 0, 2);
	createVBOT(texCoords, 1, 2);
	unbindBuffers();
	numVertices = vertices.size();
}

Model2D::Model2D(const std::vector<float>& vertices, const std::vector<int>& indices) : hasTexCoords(false), isIndexed(true){
	createVertexArray();
	createVBO(vertices, 0, 2);
	createIBO(indices);
	unbindBuffers();
	numIndices = indices.size();
}

Model2D::Model2D(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& texCoords) : hasTexCoords(true), isIndexed(true){
	createVertexArray();
	createVBO(vertices, 0, 2);
	createVBOT(texCoords, 1, 2);
	createIBO(indices);
	unbindBuffers();
	numIndices = indices.size();
}

Model2D::Model2D(float* vertices, int numVertices) : hasTexCoords(false), isIndexed(false), numVertices(numVertices){
	createVertexArray();
	createVBO(vertices, numVertices*2, 0, 2);
	unbindBuffers();
}

Model2D::Model2D(float* vertices, int numVertices, float* texCoords) : hasTexCoords(true), isIndexed(false), numVertices(numVertices){
	createVertexArray();
	createVBO(vertices, numVertices*2, 0, 2);
	createVBOT(texCoords, numVertices*2, 1, 2);
	unbindBuffers();
}

Model2D::Model2D(float* vertices, int numVertices, int* indices, int numIndices) : hasTexCoords(false), isIndexed(true), numIndices(numIndices){
	createVertexArray();
	createVBO(vertices, numVertices*2, 0, 2);
	createIBO(indices, numIndices);
	unbindBuffers();
}

Model2D::Model2D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords) : hasTexCoords(true), isIndexed(true), numIndices(numIndices){
	createVertexArray();
	createVBO(vertices, numVertices*2, 0, 2);
	createVBOT(texCoords, numVertices*2, 1, 2);
	createIBO(indices, numIndices);
	unbindBuffers();
}

void Model2D::createVertexArray(){
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
}

void Model2D::createVBO(const std::vector<float> &vertices, int index, int size){
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model2D::createVBOT(const std::vector<float> &texCoords, int index, int size){
	glGenBuffers(1, &vbotID);
	glBindBuffer(GL_ARRAY_BUFFER, vbotID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model2D::createIBO(const std::vector<int> &indices){
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Model2D::createVBO(float *vertices, int numVertices, int index, int size){
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model2D::createVBOT(float *texCoords, int numTexCoords, int index, int size){
	glGenBuffers(1, &vbotID);
	glBindBuffer(GL_ARRAY_BUFFER, vbotID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numTexCoords, texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model2D::createIBO(int *indices, int numIndices){
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*numIndices, indices, GL_STATIC_DRAW);
}

void Model2D::unbindBuffers(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model2D::bindForRender(){
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	if(hasTexCoords)
		glEnableVertexAttribArray(1);
}

void Model2D::render(){
	if(isIndexed){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}else
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Model2D::unbindForRender(){
	glDisableVertexAttribArray(0);
	if(hasTexCoords)
		glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Model2D::fullRender(){
	bindForRender();
	render();
	unbindForRender();
}
