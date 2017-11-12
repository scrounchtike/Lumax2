#ifndef MODEL3D_HPP
#define MODEL3D_HPP

#include <vector>
#include <iostream>

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

#include "../math.hpp"

class Model3D{
public:
	Model3D(const std::vector<float>& vertices);
	Model3D(const std::vector<float>& vertices, const std::vector<float>& texCoords);
	Model3D(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals);
	Model3D(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normal, const std::vector<float>& tangents);
	Model3D(const std::vector<float>& vertices, const std::vector<int>& indices);
	Model3D(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& texCoords);
	Model3D(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& texCoords, const std::vector<float>& normals);
	Model3D(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<float>& tangents);

	Model3D(float* vertices, int numVertices);
	Model3D(float* vertices, int numVertices, float* texCoords);
	Model3D(float* vertices, int numVertices, float* texCoords, float* normals);
	Model3D(float* vertices, int numVertices, float* texCoords, float* normals, float* tangents);
	Model3D(float* vertices, int numVertices, int* indices, int numIndices);
	Model3D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords);
	Model3D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals, bool withTangents);
	Model3D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals);
	Model3D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals, float* tangents);

	static float* calcTangents(const std::vector<float>& vertices, const std::vector<float>& texCoords);
	static float* calcTangents(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& texCoords);
	static float* calcTangents(float* vertices, int numVertices, float* texCoords);
	static float* calcTangents(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords);

	void bindForRender();
	void render();
	void unbindForRender();

	void fullRender();
protected:
	//State variables
	bool hasTexCoords;
	bool hasNormals;
	bool hasTangents;
	bool isIndexed;

	GLuint vaoID;
	GLuint vboID;
	GLuint vbotID;
	GLuint vbonID;
	GLuint vbotgID;
	GLuint iboID;

	int numVertices;
	int numIndices;

	void createVertexArray();
	
	void createVBO(const std::vector<float>& vertices, int index, int size);
	void createVBOT(const std::vector<float>& texCoords, int index, int size);
	void createVBON(const std::vector<float>& normals, int index, int size);
	void createVBOTG(const std::vector<float>& tangents, int index, int size);
	void createIBO(const std::vector<int>& indices);

	void createVBO(float* vertices, int numVertices, int index, int size);
	void createVBOT(float* texCoords, int numTexCoords, int index, int size);
	void createVBON(float* normals, int numNormals, int index, int size);
	void createVBOTG(float* tangents, int numTangents, int index, int size);
	void createIBO(int* indices, int numIndices);
	
	void unbindBuffers();
};

#endif
