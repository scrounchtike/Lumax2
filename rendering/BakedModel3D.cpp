
#include "BakedModel3D.hpp"

BakedModel3D::BakedModel3D(float* vertices, int numVertices, float* texCoords, float* normals, float* bakedTexCoords) : Model3DGL(vertices, numVertices, texCoords, normals){
	glBindVertexArray(vaoID);
	std::cout << "NUM VERTICES = " << numVertices << std::endl;
	createVBOBT(bakedTexCoords, numVertices*2, 3, 2);
	glBindVertexArray(0);
}

BakedModel3D::BakedModel3D(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<float>& bakedTexCoords) : Model3DGL(vertices, texCoords, normals){
	glBindVertexArray(vaoID);
	createVBOBT(bakedTexCoords, 3, 2);
	glBindVertexArray(0);
}

void BakedModel3D::addBakedTexCoords(float *bakedTexCoords){
	glBindVertexArray(vaoID);
	createVBOBT(bakedTexCoords, numVertices*2, 3, 2);
	glBindVertexArray(0);
}

void BakedModel3D::addBakedTexCoords(const std::vector<float> &bakedTexCoords){
	glBindVertexArray(vaoID);
	createVBOBT(bakedTexCoords, 3, 2);
	glBindVertexArray(0);
}

void BakedModel3D::createVBOBT(float *bakedTexCoords, int numTexCoords, int index, int size){
	glGenBuffers(1, &vbobtID);
	glBindBuffer(GL_ARRAY_BUFFER, vbobtID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numTexCoords, bakedTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BakedModel3D::createVBOBT(const std::vector<float> &bakedTexCoords, int index, int size){
	glGenBuffers(1, &vbobtID);
	glBindBuffer(GL_ARRAY_BUFFER, vbobtID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*bakedTexCoords.size(), &bakedTexCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BakedModel3D::bindForRender() const {
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	if(hasTexCoords)
		glEnableVertexAttribArray(1);
	if(hasNormals)
		glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}

void BakedModel3D::renderBuffersOnly() const {
	if(isIndexed){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}else{
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void BakedModel3D::unbindForRender() const {
	glDisableVertexAttribArray(0);
	if(hasTexCoords)
		glDisableVertexAttribArray(1);
	if(hasNormals)
		glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glBindVertexArray(0);
}

void BakedModel3D::render() const{
	bindForRender();
	renderBuffersOnly();
	unbindForRender();
}
