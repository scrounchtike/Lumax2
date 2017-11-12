
#pragma once

#ifndef MODEL_3D_DX11_HPP
#define MODEL_3D_DX11_HPP

#include <vector>

#include "../RAL/RenderingContextDX11.hpp"
#include "../math.hpp"

class Model3DDX11 {
public:
	Model3DDX11(RenderingContextDX11* dx11);

	Model3DDX11(const Model3DDX11&);
	~Model3DDX11();

	void update();
	void render() const;

	// Initialization with OBJ file
	void initialize(const std::string& filename);

	// These functions only use one attribute for all data 
	void initialize(float* vertices, int numVertices, int stride);
	void initialize(float* vertices, int numVertices);
	void initialize(float* vertices, int numVertices, float* texCoords);
	void initialize(float* vertices, int numVertices, float* texCoords, float* normals);
	void initialize(float* vertices, int numVertices, float* texCoords, float* normals, float* tangents, float* bitangents);

	void initialize(float* vertices, int numVertices, int* indices, int numIndices, int stride);
	void initialize(float* vertices, int numVertices, int* indices, int numIndices);
	void initialize(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords);
	void initialize(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals);
	void initialize(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals, float* tangents, float* bitangents);
private:
	RenderingContextDX11* dx11;

	int strideSize;

	bool isIndexed = false;
	int numVertices, numIndices;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	void createInterleavedBuffer(float* vertices, int numVertices);
	void createIndexBuffer(int* indices, int numIndices);

	static void interleaveData(std::vector<float>& result, float* vertices, int numVertices, float* texCoords);
	static void interleaveData(std::vector<float>& result, float* vertices, int numVertices, float* texCoords, float* normals);
	static void interleaveData(std::vector<float>& result, float* vertices, int numVertices, float* texCoords, float* normals, float* tangents, float* bitangents);
};

#endif