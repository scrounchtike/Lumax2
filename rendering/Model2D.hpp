#ifndef MODEL_2D_HPP
#define MODEL_2D_HPP

#include <vector>
#include <iostream>

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

class Model2D{
public:
	Model2D(const std::vector<float>& vertices);
	Model2D(const std::vector<float>& vertices, const std::vector<float>& texCoords);
	Model2D(const std::vector<float>& vertices, const std::vector<int>& indices);
	Model2D(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& texCoords);

	Model2D(float* vertices, int numVertices);
	Model2D(float* vertices, int numVertices, float* texCoords);
	Model2D(float* vertices, int numVertices, int* indices, int numIndices);
	Model2D(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords);

	void bindForRender();
	void render();
	void unbindForRender();

	void fullRender();
private:
	//State variables
	bool hasTexCoords;
	bool isIndexed;

	GLuint vaoID;
	GLuint vboID;
	GLuint iboID;
	GLuint vbotID;

	int numVertices;
	int numIndices;

	void createVertexArray();

	void createVBO(const std::vector<float>& vertices, int index, int size);
	void createVBOT(const std::vector<float>& texCoords, int index, int size);
	void createIBO(const std::vector<int>& indices);

	void createVBO(float* vertices, int numVertices, int index, int size);
	void createVBOT(float* texCoords, int numTexCoords, int index, int size);
	void createIBO(int* indices, int numIndices);

	void unbindBuffers();
};

#endif
