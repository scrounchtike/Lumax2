
#pragma once

#include "Model2D_DX11.hpp"

Model2DDX11::Model2DDX11(RenderingContextDX11* dx11) : dx11(dx11) {

}

Model2DDX11::Model2DDX11(const Model2DDX11& other) { }
Model2DDX11::~Model2DDX11() { }

void Model2DDX11::update() {

}

void Model2DDX11::render() const {
	unsigned int stride = sizeof(float) * strideSize;
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

void Model2DDX11::initialize(float* vertices, int numVertices) {
	this->isIndexed = false;
	this->isTextured = false;
	this->numVertices = numVertices;
	this->strideSize = 2;

	createInterleavedBuffer(vertices, numVertices, strideSize);
}

void Model2DDX11::initialize(float* vertices, int numVertices, float* texCoords) {
	this->isIndexed = false;
	this->isTextured = true;
	this->numVertices = numVertices;
	this->strideSize = 4;

	std::vector<float> vertexData;
	interleaveData(vertexData, vertices, numVertices, texCoords);

	createInterleavedBuffer(&vertexData[0], numVertices, strideSize);
}

void Model2DDX11::initialize(float* vertices, int numVertices, int* indices, int numIndices) {
	this->isIndexed = true;
	this->isTextured = false;
	this->numIndices = numIndices;
	this->strideSize = 2;

	createInterleavedBuffer(vertices, numVertices, strideSize);
	createIndexBuffer(indices, numIndices);
}

void Model2DDX11::initialize(float* vertices, int numVertices, int* indices, int numIndices, float* texCoords) {
	this->isIndexed = true;
	this->isTextured = true;
	this->numIndices = numIndices;
	this->strideSize = 4;

	std::vector<float> vertexData;
	interleaveData(vertexData, vertices, numVertices, texCoords);

	createInterleavedBuffer(&vertexData[0], numVertices, strideSize);
	createIndexBuffer(indices, numIndices);
}

void Model2DDX11::interleaveData(std::vector<float>& result, float* vertices, int numVertices, float* texCoords) {
	for (int i = 0; i < numVertices; ++i) {
		result.push_back(vertices[i * 2 + 0]);
		result.push_back(vertices[i * 2 + 1]);
		result.push_back(texCoords[i * 2 + 0]);
		result.push_back(texCoords[i * 2 + 1]);
	}
}

void Model2DDX11::createInterleavedBuffer(float* vertexData, int numVertices, int strideSize) {
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = sizeof(float) * strideSize * numVertices;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT result = dx11->getDevice()->CreateBuffer(&vertexDesc, &data, &vertexBuffer);
	if (FAILED(result)) {
		OutputDebugString("Error: Failed to create 2D interleaved vertex buffer");
		assert(false);
		return;
	}
}

void Model2DDX11::createIndexBuffer(int* indices, int numIndices) {
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
		OutputDebugString("Error: Failed to create 2D index buffer");
		assert(false);
		return;
	}
}