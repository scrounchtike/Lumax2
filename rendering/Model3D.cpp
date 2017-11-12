
#include "Model3D.hpp"

Model3D::Model3D(const std::vector<float>& vertices) : hasTexCoords(false), hasNormals(false), isIndexed(false), hasTangents(false){
	createVertexArray();
	createVBO(vertices, 0, 3);
	unbindBuffers();
	numVertices = vertices.size()/3;
}

Model3D::Model3D(const std::vector<float>& vertices, const std::vector<float>& texCoords) : hasTexCoords(true), hasNormals(false), isIndexed(false), hasTangents(false){
	createVertexArray();
	createVBO(vertices, 0, 3);
	createVBOT(texCoords, 1, 2);
	unbindBuffers();
	numVertices = vertices.size()/3;
}

Model3D::Model3D(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals) : hasTexCoords(true), hasNormals(true), isIndexed(false), hasTangents(false){
	createVertexArray();
	createVBO(vertices, 0, 3);
	createVBOT(texCoords, 1, 2);
	createVBON(normals, 2, 3);
	unbindBuffers();
	numVertices = vertices.size()/3;
}

Model3D::Model3D(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<float>& tangents) : hasTexCoords(true), hasNormals(true), isIndexed(false), hasTangents(true){
	createVertexArray();
	createVBO(vertices, 0, 3);
	createVBOT(texCoords, 1, 2);
	createVBON(normals, 2, 3);
	createVBOTG(tangents, 3, 3);
	unbindBuffers();
	numVertices = vertices.size()/3;
}

Model3D::Model3D(const std::vector<float>& vertices, const std::vector<int>& indices) : hasTexCoords(false), hasNormals(false), isIndexed(true), hasTangents(false){
	createVertexArray();
	createVBO(vertices, 0, 3);
	createIBO(indices);
	unbindBuffers();
	numIndices = indices.size();
}

Model3D::Model3D(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& texCoords) : hasTexCoords(true), hasNormals(false), isIndexed(true), hasTangents(false){
	createVertexArray();
	createVBO(vertices, 0, 3);
	createIBO(indices);
	createVBOT(texCoords, 1, 2);
	unbindBuffers();
	numIndices = indices.size();
}

Model3D::Model3D(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& texCoords, const std::vector<float>& normals) : hasTexCoords(true), hasNormals(true), isIndexed(true), hasTangents(false){
	createVertexArray();
	createVBO(vertices, 0, 3);
	createIBO(indices);
	createVBOT(texCoords, 1, 2);
	createVBON(normals, 2, 3);
	unbindBuffers();
	numIndices = indices.size();
}

Model3D::Model3D(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<float>& tangents) : hasTexCoords(true), hasNormals(true), isIndexed(true), hasTangents(true){
	createVertexArray();
	createVBO(vertices, 0, 3);
	createIBO(indices);
	createVBOT(texCoords, 1, 2);
	createVBON(normals, 2, 3);
	createVBOTG(tangents, 3, 3);
	unbindBuffers();
	numIndices = indices.size();
}

Model3D::Model3D(float* vertices, int numVertices) : hasTexCoords(false), hasNormals(false), isIndexed(false), hasTangents(false), numVertices(numVertices){
	createVertexArray();
	createVBO(vertices, numVertices*3, 0, 3);
	unbindBuffers();
}

Model3D::Model3D(float* vertices, int numVertices, float* texCoords) : hasTexCoords(true), hasNormals(false), isIndexed(false), hasTangents(false), numVertices(numVertices){
	createVertexArray();
	createVBO(vertices, numVertices*3, 0, 3);
	createVBOT(texCoords, numVertices*2, 1, 2);
	unbindBuffers();
}

Model3D::Model3D(float* vertices, int numVertices, float* texCoords, float* normals) : hasTexCoords(true), hasNormals(true), isIndexed(false), hasTangents(false), numVertices(numVertices){
	createVertexArray();
	createVBO(vertices, numVertices*3, 0, 3);
	createVBOT(texCoords, numVertices*2, 1, 2);
	createVBON(normals, numVertices*3, 2, 3);
	unbindBuffers();
}

Model3D::Model3D(float* vertices, int numVertices, float* texCoords, float* normals, float* tangents) : hasTexCoords(true), hasNormals(true), isIndexed(false), hasTangents(true), numVertices(numVertices){
	createVertexArray();
	createVBO(vertices, numVertices*3, 0, 3);
	createVBOT(texCoords, numVertices*2, 1, 2);
	createVBON(normals, numVertices*3, 2, 3);
	createVBOTG(tangents, numVertices*3, 3, 3);
	unbindBuffers();
}

Model3D::Model3D(float* vertices, int numVertices, int* indices, int numIndices) : hasTexCoords(false), hasNormals(false), isIndexed(true), hasTangents(false), numIndices(numIndices){
	createVertexArray();
	createVBO(vertices, numVertices*3, 0, 3);
	createIBO(indices, numIndices);
	unbindBuffers();
}

Model3D::Model3D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords) : hasTexCoords(true), hasNormals(false), isIndexed(true), hasTangents(false), numIndices(numIndices){
	createVertexArray();
	createVBO(vertices, numVertices*3, 0, 3);
	createVBOT(texCoords, numVertices*2, 1, 2);
	createIBO(indices, numIndices);
	unbindBuffers();
}

Model3D::Model3D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals, bool withTangents) : hasTexCoords(true), hasNormals(true), isIndexed(true), hasTangents(false), numIndices(numIndices){
	createVertexArray();
	createVBO(vertices, numVertices*3, 0, 3);
	createVBOT(texCoords, numVertices*2, 1, 2);
	createVBON(normals, numVertices*3, 2, 3);
	if(withTangents){
		float* tangents = calcTangents(vertices, numVertices, indices, numIndices, texCoords);
		hasTangents = true;
		createVBOTG(tangents, numVertices*3, 3, 3);
	}
	createIBO(indices, numIndices);
	unbindBuffers();
}

Model3D::Model3D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals) : hasTexCoords(true), hasNormals(true), isIndexed(true), hasTangents(false), numIndices(numIndices){
	createVertexArray();
	createVBO(vertices, numVertices*3, 0, 3);
	createVBOT(texCoords, numVertices*2, 1, 2);
	createVBON(normals, numVertices*3, 2, 3);
	createIBO(indices, numIndices);
	unbindBuffers();
}

Model3D::Model3D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals, float* tangents) : hasTexCoords(true), hasNormals(true), isIndexed(true), hasTangents(true), numIndices(numIndices){
	createVertexArray();
	createVBO(vertices, numVertices*3, 0, 3);
	createVBOT(texCoords, numVertices*2, 1, 2);
	createVBON(normals, numVertices*3, 2, 3);
	createVBOTG(tangents, numVertices*3, 3, 3);
	createIBO(indices, numIndices);
	unbindBuffers();
}

void Model3D::createVertexArray(){
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
}

void Model3D::createVBO(const std::vector<float> &vertices, int index, int size){
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model3D::createVBOT(const std::vector<float> &texCoords, int index, int size){
	glGenBuffers(1, &vbotID);
	glBindBuffer(GL_ARRAY_BUFFER, vbotID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model3D::createVBON(const std::vector<float> &normals, int index, int size){
	glGenBuffers(1, &vbonID);
	glBindBuffer(GL_ARRAY_BUFFER, vbonID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model3D::createVBOTG(const std::vector<float> &tangents, int index, int size){
	glGenBuffers(1, &vbotgID);
	glBindBuffer(GL_ARRAY_BUFFER, vbotgID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*tangents.size(), &tangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model3D::createIBO(const std::vector<int> &indices){
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Model3D::createVBO(float* vertices, int numVertices, int index, int size){
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model3D::createVBOT(float *texCoords, int numTexCoords, int index, int size){
	glGenBuffers(1, &vbotID);
	glBindBuffer(GL_ARRAY_BUFFER, vbotID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numTexCoords, texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model3D::createVBON(float *normals, int numNormals, int index, int size){
	glGenBuffers(1, &vbonID);
	glBindBuffer(GL_ARRAY_BUFFER, vbonID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numNormals, normals, GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model3D::createVBOTG(float *tangents, int numTangents, int index, int size){
	glGenBuffers(1, &vbotgID);
	glBindBuffer(GL_ARRAY_BUFFER, vbotgID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numTangents, tangents, GL_STATIC_DRAW);
	glVertexAttribPointer(index, size, GL_FLOAT, false, 0, 0);
}

void Model3D::createIBO(int *indices, int numIndices){
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*numIndices, indices, GL_STATIC_DRAW);
}

void Model3D::unbindBuffers(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model3D::bindForRender(){
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	if(hasTexCoords)
		glEnableVertexAttribArray(1);
	if(hasNormals)
		glEnableVertexAttribArray(2);
	if(hasTangents)
		glEnableVertexAttribArray(3);
}

void Model3D::render(){
	if(isIndexed){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}else
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Model3D::unbindForRender(){
	glDisableVertexAttribArray(0);
	if(hasTexCoords)
		glDisableVertexAttribArray(1);
	if(hasNormals)
		glDisableVertexAttribArray(2);
	if(hasTangents)
		glDisableVertexAttribArray(3);
	glBindVertexArray(0);
}

void Model3D::fullRender(){
	bindForRender();
	render();
	unbindForRender();
}

float* Model3D::calcTangents(const std::vector<float>& vertices, const std::vector<float>& texCoords){
	return nullptr;
}

float* Model3D::calcTangents(const std::vector<float> &vertices, const std::vector<int> &indices, const std::vector<float> &texCoords){
	return nullptr;
}

float* Model3D::calcTangents(float* vertices, int numVertices, float* texCoords){
	float* tangents = new float[numVertices*3];
	for(int i = 0; i < numVertices/3; ++i){
		Vec3 vertex1 = Vec3(vertices[i*9+0], vertices[i*9+1], vertices[i*9+2]);
		Vec3 vertex2 = Vec3(vertices[i*9+3], vertices[i*9+4], vertices[i*9+5]);
		Vec3 vertex3 = Vec3(vertices[i*9+6], vertices[i*9+7], vertices[i*9+8]);

		Vec2 uv1 = Vec2(texCoords[i*6+0], texCoords[i*6+1]);
		Vec2 uv2 = Vec2(texCoords[i*6+2], texCoords[i*6+3]);
		Vec2 uv3 = Vec2(texCoords[i*6+4], texCoords[i*6+5]);

		Vec3 edge1 = vertex3 - vertex1;
		Vec3 edge2 = vertex2 - vertex1;
		Vec2 deltaUV1 = uv3 - uv1;
		Vec2 deltaUV2 = uv2 - uv1;

		Vec3 tangent = (edge1*deltaUV2.y - edge2*deltaUV1.y)/(deltaUV1.x*deltaUV2.y - deltaUV1.y*deltaUV2.x);
		tangents[i*9+0] = tangent.x;
		tangents[i*9+1] = tangent.y;
		tangents[i*9+2] = tangent.z;
		tangents[i*9+3] = tangent.x;
		tangents[i*9+4] = tangent.y;
		tangents[i*9+5] = tangent.z;
		tangents[i*9+6] = tangent.x;
		tangents[i*9+7] = tangent.y;
		tangents[i*9+8] = tangent.z;
	}
	return tangents;
}

float* Model3D::calcTangents(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords){
	float* tangents = new float[numVertices*3];
	for(int i = 0; i < numIndices/3; ++i){
		int index1 = indices[i*3+0]*3;
		Vec3 vertex1 = Vec3(vertices[index1+0], vertices[index1+1], vertices[index1+2]);
		int index2 = indices[i*3+1]*3;
		Vec3 vertex2 = Vec3(vertices[index2+0], vertices[index2+1], vertices[index2+2]);
		int index3 = indices[i*3+2]*3;
		Vec3 vertex3 = Vec3(vertices[index3+0], vertices[index3+1], vertices[index3+2]);
		
		Vec2 uv1 = Vec2(texCoords[index1+0], texCoords[index1+1]);
		Vec2 uv2 = Vec2(texCoords[index2+0], texCoords[index2+1]);
		Vec2 uv3 = Vec2(texCoords[index3+0], texCoords[index3+1]);
		
		Vec3 edge1 = vertex3 - vertex1;
		Vec3 edge2 = vertex2 - vertex1;
		Vec2 deltaUV1 = uv3 - uv1;
		Vec2 deltaUV2 = uv2 - uv1;
		
		Vec3 tangent = (edge1*deltaUV2.y - edge2*deltaUV1.y)/(deltaUV1.x*deltaUV2.y - deltaUV1.y*deltaUV2.x);
		tangents[i*9+0] = tangent.x;
		tangents[i*9+1] = tangent.y;
		tangents[i*9+2] = tangent.z;
		tangents[i*9+3] = tangent.x;
		tangents[i*9+4] = tangent.y;
		tangents[i*9+5] = tangent.z;
		tangents[i*9+6] = tangent.x;
		tangents[i*9+7] = tangent.y;
		tangents[i*9+8] = tangent.z;
	}
	return tangents;
}
