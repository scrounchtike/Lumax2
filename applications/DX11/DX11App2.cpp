
#include "DX11App2.hpp"

#include "../../math.hpp"

DX11App2::DX11App2() {

}

DX11App2::DX11App2(const DX11App2& other) { }
DX11App2::~DX11App2() { }

void DX11App2::input() {

}

void DX11App2::update() {

}

void DX11App2::render() {
	unsigned int stride = sizeof(float) * 5;
	unsigned int offset = 0;

	dx11->getDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dx11->getDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dx11->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Mat4 transform = Mat4().initIdentity();
	Mat4 view = Mat4().initIdentity();
	Mat4 projection = Mat4().initIdentity();

	shader->bindMatrixUniforms(dx11, transform, view, projection);
	ID3D11ShaderResourceView* textures[1] = { texture->getTexture() };
	shader->bindTexture(dx11, textures, 1);
	shader->bind(dx11);

	// Actual draw call
	dx11->getDeviceContext()->DrawIndexed(6, 0, 0);
}

bool DX11App2::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	// Creating the shader
	shader = new ShaderDX11();
	shader->addAttribute("POSITION", 0, 3, false);
	shader->addAttribute("TEXCOORD0", 0, 2, true);
	shader->initialize(dx11, "applications/DX11/app2/shaderTexture", 0);

	// Creating the texture
	texture = new TextureDX11();
	bool success = texture->initialize(dx11, "res/textures_dds/test.dds");
	assert(success);

	const int numVertices = 4;
	const int numIndices = 6;
	float shaderData[numVertices * 5] = { -0.5f,-0.5f,0,0,0,
										  -0.5f, 0.5f,0,0,1,
										   0.5f,0.5f,0,1,1,
										   0.5f,-0.5f,0,1,0 };
	int indices[6] = { 0,1,2,2,3,0 };

	// Creating the buffers
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(float) * numVertices * 5;

	vertexData.pSysMem = shaderData;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * numIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	HRESULT result;
	result = dx11->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result)) {
		OutputDebugString("Error: Could not create vertex data buffer for application 2\n");
		assert(false);
		return false;
	}

	result = dx11->getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result)) {
		OutputDebugString("Error: Could not create index data buffer for application 2\n");
		assert(false);
		return false;
	}

	return true;
}

bool DX11App2::cleanUp() {
	if (texture) {
		texture->cleanUp();
		texture = 0;
	}

	return true;
}