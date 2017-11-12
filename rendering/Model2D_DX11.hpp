
#pragma once

#ifndef MODEL_2D_DX11_HPP
#define MODEL_2D_DX11_HPP

#include <vector>

#include "../RAL/RenderingContextDX11.hpp"

class Model2DDX11 {
public:
	Model2DDX11(RenderingContextDX11* dx11);
	Model2DDX11(const Model2DDX11&);
	~Model2DDX11();

	void update();
	void render() const;

	void initialize(float* vertices, int numVertices);
	void initialize(float* vertices, int numVertices, float* texCoords);
	void initialize(float* vertices, int numVertices, int* indices, int numIndices);
	void initialize(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords);
private:
	RenderingContextDX11* dx11;

	int strideSize;
	bool isIndexed = false;
	bool isTextured = false;
	unsigned int numVertices, numIndices;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* texCoordBuffer;
	ID3D11Buffer* indexBuffer;

	void interleaveData(std::vector<float>& result, float* vertices, int numVertices, float* texCoords);

	void createInterleavedBuffer(float* vertexData, int numVertices, int strideSize);
	void createIndexBuffer(int* indices, int numIndices);
};

#endif