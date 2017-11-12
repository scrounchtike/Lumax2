
#pragma once

#include "Model3D_DX11.hpp"
#include "../OBJ_Loader.hpp"

Model3DDX11::Model3DDX11(RenderingContextDX11* dx11) : dx11(dx11){

}

Model3DDX11::Model3DDX11(const Model3DDX11& other) { }
Model3DDX11::~Model3DDX11() { }

void Model3DDX11::update() {

}

void Model3DDX11::render() const {
	unsigned int stride = strideSize;
	unsigned int offset = 0;

	dx11->getDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	if (isIndexed)
		dx11->getDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dx11->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (isIndexed)
		dx11->getDeviceContext()->DrawIndexed(numIndices, 0, 0);
	else
		dx11->getDeviceContext()->Draw(numVertices, 0);
}

void Model3DDX11::initialize(const std::string& filename) {
	RawModel raw = OBJ_Loader::loadModel(filename);
	initialize(raw.vertices, raw.numVertices, raw.texCoords, raw.normals);
}

void Model3DDX11::initialize(float* vertices, int numVertices, int stride) {
	isIndexed = false;
	this->numVertices = numVertices;
	this->strideSize = sizeof(float) * stride;

	createInterleavedBuffer(vertices, numVertices);
}

void Model3DDX11::initialize(float* vertices, int numVertices) {
	initialize(vertices, numVertices, 3);
}

void Model3DDX11::initialize(float* vertices, int numVertices, float* texCoords) {
	std::vector<float> vertexData;
	interleaveData(vertexData, vertices, numVertices, texCoords);

	initialize(&vertexData[0], numVertices, 5);
}

void Model3DDX11::initialize(float* vertices, int numVertices, float* texCoords, float* normals) {
	std::vector<float> vertexData;
	interleaveData(vertexData, vertices, numVertices, texCoords, normals);

	initialize(&vertexData[0], numVertices, 8);
}

void Model3DDX11::initialize(float* vertices, int numVertices, float* texCoords, float* normals, float* tangents, float* bitangents) {
	std::vector<float> vertexData;
	interleaveData(vertexData, vertices, numVertices, texCoords, normals, tangents, bitangents);

	initialize(&vertexData[0], numVertices, 14);
}

void Model3DDX11::initialize(float* vertices, int numVertices, int* indices, int numIndices, int stride) {
	isIndexed = true;
	this->numIndices = numIndices;
	this->strideSize = stride * sizeof(float);

	createInterleavedBuffer(vertices, numVertices);
	createIndexBuffer(indices, numIndices);
}

void Model3DDX11::initialize(float* vertices, int numVertices, int* indices, int numIndices) {
	initialize(vertices, numVertices, indices, numIndices, 3);
}

void Model3DDX11::initialize(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords) {
	std::vector<float> vertexData;
	interleaveData(vertexData, vertices, numVertices, texCoords);

	initialize(&vertexData[0], numVertices, indices, numIndices, 5);
}

void Model3DDX11::initialize(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals) {
	std::vector<float> vertexData;
	interleaveData(vertexData, vertices, numVertices, texCoords, normals);

	initialize(&vertexData[0], numVertices, indices, numIndices, 8);
}

void Model3DDX11::initialize(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords, float* normals, float* tangents, float* bitangents) {
	std::vector<float> vertexData;
	interleaveData(vertexData, vertices, numVertices, texCoords, normals, tangents, bitangents);

	initialize(&vertexData[0], numVertices, indices, numIndices, 14);
}

void Model3DDX11::createInterleavedBuffer(float* vertices, int numVertices) {
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = strideSize * numVertices;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT result = dx11->getDevice()->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(result)) {
		OutputDebugString("Error: Failed to initialize DX11 3D model with vertex info");
		assert(false);
		return;
	}
}

void Model3DDX11::createIndexBuffer(int* indices, int numIndices) {
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(int) * numIndices;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	HRESULT result = dx11->getDevice()->CreateBuffer(&indexDesc, &indexData, &indexBuffer);
	if (FAILED(result)) {
		OutputDebugString("Error: Failed to initialize index buffer during DX11 3D model creation");
		assert(false);
		return;
	}
}

void Model3DDX11::interleaveData(std::vector<float>& vertexData, float* vertices, int numVertices, float* texCoords) {
	for (int i = 0; i < numVertices; ++i) {
		vertexData.push_back(vertices[i * 3]);
		vertexData.push_back(vertices[i * 3 + 1]);
		vertexData.push_back(vertices[i * 3 + 2]);
		vertexData.push_back(texCoords[i * 2]);
		vertexData.push_back(texCoords[i * 2 + 1]);
	}
}

void Model3DDX11::interleaveData(std::vector<float>& vertexData, float* vertices, int numVertices, float* texCoords, float* normals) {
	for (int i = 0; i < numVertices; ++i) {
		vertexData.push_back(vertices[i * 3]);
		vertexData.push_back(vertices[i * 3 + 1]);
		vertexData.push_back(vertices[i * 3 + 2]);
		vertexData.push_back(texCoords[i * 2]);
		vertexData.push_back(texCoords[i * 2 + 1]);
		vertexData.push_back(normals[i * 3]);
		vertexData.push_back(normals[i * 3 + 1]);
		vertexData.push_back(normals[i * 3 + 2]);
	}
}

void Model3DDX11::interleaveData(std::vector<float>& vertexData, float* vertices, int numVertices, float* texCoords, float* normals, float* tangents, float* bitangents) {
	for (int i = 0; i < numVertices; ++i) {
		vertexData.push_back(vertices[i * 3]);
		vertexData.push_back(vertices[i * 3 + 1]);
		vertexData.push_back(vertices[i * 3 + 2]);
		vertexData.push_back(texCoords[i * 2]);
		vertexData.push_back(texCoords[i * 2 + 1]);
		vertexData.push_back(normals[i * 3]);
		vertexData.push_back(normals[i * 3 + 1]);
		vertexData.push_back(normals[i * 3 + 2]);
		vertexData.push_back(tangents[i * 3]);
		vertexData.push_back(tangents[i * 3 + 1]);
		vertexData.push_back(tangents[i * 3 + 2]);
		vertexData.push_back(bitangents[i * 3]);
		vertexData.push_back(bitangents[i * 3 + 1]);
		vertexData.push_back(bitangents[i * 3 + 2]);
	}
}