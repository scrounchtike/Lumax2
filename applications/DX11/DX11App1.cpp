
#include "DX11App1.hpp"

DX11App1::DX11App1() {

}

DX11App1::DX11App1(const DX11App1& other) { }
DX11App1::~DX11App1() { }

void DX11App1::input() {

}

void DX11App1::update() {

}

void DX11App1::render() {
	basicShader->bind(dx11);

	unsigned int stride = sizeof(Vec3);
	unsigned int offset = 0;
	dx11->getDeviceContext()->IASetVertexBuffers(0, 1, &vertexBufferPoints, &stride, &offset);
	dx11->getDeviceContext()->IASetIndexBuffer(indexBufferPoints, DXGI_FORMAT_R32_UINT, 0);
	dx11->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Actual draw call
	dx11->getDeviceContext()->DrawIndexed(6, 0, 0);
}

bool DX11App1::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;
	// Init basic shader
	basicShader = new ShaderDX11();
	basicShader->addAttribute("POSITION", 0, 3, false);
	basicShader->initialize(dx11, "applications/DX11/app1/color", 0);

	// Init buffers
	//Vertex verticesPoints[4] = { 0.25f, 0.25f, 0.5f, 0.25f, 0.5f, 0.5f, 0.25f, 0.5f };
	const int numVertices = 4;
	const int numIndices = 6;
	
	Vec3 verticesPoints[numVertices] = { Vec3(0.25, 0.25, 0), Vec3(0.25, 0.5, 0),
										 Vec3(0.5, 0.5, 0), Vec3(0.5, 0.25, 0) };
	int indicesPoints[numIndices] = { 0,1,2,2,3,0 };

	D3D11_BUFFER_DESC vertexBufferPointsDesc;
	ZeroMemory(&vertexBufferPointsDesc, sizeof(vertexBufferPointsDesc));
	vertexBufferPointsDesc.ByteWidth = numVertices * sizeof(Vertex);
	vertexBufferPointsDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferPointsDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferPointsDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexSubResource;
	ZeroMemory(&vertexSubResource, sizeof(vertexSubResource));
	vertexSubResource.pSysMem = verticesPoints;

	HRESULT result;
	result = dx11->getDevice()->CreateBuffer(&vertexBufferPointsDesc, &vertexSubResource, &vertexBufferPoints);
	if (FAILED(result)) {
		assert(false);
		return false;
	}
	assert(vertexBufferPoints);

	D3D11_BUFFER_DESC indexBufferPointsDesc;
	ZeroMemory(&indexBufferPointsDesc, sizeof(indexBufferPointsDesc));
	indexBufferPointsDesc.ByteWidth = numIndices * sizeof(int);
	indexBufferPointsDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferPointsDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferPointsDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexSubResource;
	ZeroMemory(&indexSubResource, sizeof(indexSubResource));
	indexSubResource.pSysMem = indicesPoints;

	result = dx11->getDevice()->CreateBuffer(&indexBufferPointsDesc, &indexSubResource, &indexBufferPoints);
	if (FAILED(result)) {
		assert(false);
		return false;
	}
	assert(indexBufferPoints);

	return true;
}

bool DX11App1::cleanUp() {
	if (indexBufferPoints) {
		indexBufferPoints->Release();
		indexBufferPoints = 0;
	}
	if (vertexBufferPoints) {
		vertexBufferPoints->Release();
		vertexBufferPoints = 0;
	}
	return true;
}