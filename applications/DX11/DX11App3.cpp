
#include "DX11App3.hpp"

#include "../../main.hpp"

#include <iostream>
#include <fstream>
#include <string>

DX11App3::DX11App3() {
	shader = 0;
	texture = 0;
}

DX11App3::DX11App3(const DX11App3&) { }
DX11App3::~DX11App3() { }

void DX11App3::input() {
	camera->input();
}

void DX11App3::update() {
	camera->update();
}

void DX11App3::render() {
	Mat4 transform = Mat4().initTranslation(0,0,3);
	Mat4 view = camera->getView();
	Mat4 projection = Mat4().initProjectionDX11(70.0f, (600.0f/800.0f), 0.1f, 1000.0f);

	shader->bind(dx11);
	shader->bindMatrixUniforms(dx11, transform, view, projection);
	ID3D11ShaderResourceView* textures[1] = { texture->getTexture() };
	shader->bindTexture(dx11, textures, 1);

	model->render();

	shader2D->bind(dx11);

	sprite->render();

	/*
	unsigned int stride = sizeof(float) * 8;
	unsigned int offset = 0;
	dx11->getDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//dx11->getDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dx11->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	dx11->getDeviceContext()->Draw(32, 0);
	//dx11->getDeviceContext()->DrawIndexed(6, 0, 0);
	*/
}

bool DX11App3::initialize(RenderingContextDX11* dx11) {
	camera = new Camera(getStaticWindow(), Vec3(0, 0, 0));

	this->dx11 = dx11;
	HRESULT result;

	// Create 3D shader
	shader = new ShaderDX11();
	shader->addAttribute("POSITION", 0, 3, false);
	shader->addAttribute("TEXCOORD", 0, 2, true);
	shader->addAttribute("NORMAL", 0, 3, true);
	shader->initialize(dx11, "applications/DX11/app3/shader3D", 0);

	// Create 2D shader
	shader2D = new ShaderDX11();
	shader2D->addAttribute("POSITION", 0, 2, false);
	shader2D->initialize(dx11, "applications/DX11/app3/shader2D", 0);

	// Texture creation
	texture = new TextureDX11();
	texture->initialize(dx11, "res/textures_dds/test.dds");

	// 3D Cube loading from cube.txt file
	std::ifstream cubeInput;
	cubeInput.open("applications/DX11/app3/cube.txt");

	if (cubeInput.fail()) {
		OutputDebugString("Error: Could not load cube.txt required to launch app3\n");
		assert(false);
		return false;
	}

	// OBJ_Loader

	//OBJ_Loader::loadModel("res/models/cube.obj");

	// TODO: Make the cube with indexed data
	const int numVertices = 36;
	struct Vertex {
		float x, y, z;
		float u, v;
		float nx, ny, nz;
	};
	Vertex vertices[numVertices];

	int vertexCount = 36 * 3;
	float verticesData[36 * 3];
	float texCoordsData[36 * 2];
	float normalsData[36 * 3];

	char input;
	for (int i = 0; i < numVertices; ++i) {
		//cubeInput >> vertices[i].x >> vertices[i].y >> vertices[i].z;
		//cubeInput >> vertices[i].u >> vertices[i].v;
		//cubeInput >> vertices[i].nx >> vertices[i].ny >> vertices[i].nz;
		cubeInput >> verticesData[i * 3] >> verticesData[i * 3 + 1] >> verticesData[i * 3 + 2];
		cubeInput >> texCoordsData[i * 2] >> texCoordsData[i * 2 + 1];
		cubeInput >> normalsData[i * 3] >> normalsData[i * 3 + 1] >> normalsData[i * 3 + 2];
	}
	cubeInput.close();

	for (int i = 0; i < numVertices; ++i) {
		// Really have to figure out a better way to access the console resources..
		std::string str = std::to_string(vertices[i].x) + " " + std::to_string(vertices[i].y) + " " + std::to_string(vertices[i].z) + "\n";
		OutputDebugString(str.c_str());
		//std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
	}

	model = new Model3DDX11(dx11);
	//model->initialize(verticesData, 36, texCoordsData, normalsData);
	model->initialize("res/models/cube.obj");

	sprite = new Model2DDX11(dx11);
	float verticesSprite[6 * 2] = { -0.5,-0.5,-0.5,0.5,0.5,0.5,0.5,-0.5 };
	int indicesSprite[6] = { 0,1,2,2,3,0 };
	sprite->initialize(verticesSprite, 4, indicesSprite, 6);


	// Buffer creation
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * numVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	float verticesPlane[32] = { -0.5f, -0.5f, 1, 0, 0, 0, 0, -1,
							-0.5f, 0.5f, 1, 0, 1, 0, 0, -1,
							 0.5f, 0.5f, 1, 1, 1, 0, 0, -1,
							 0.5f, -0.5f, 1, 1, 0, 0, 0, -1 };
	int indicesPlane[6] = { 0,1,2,2,3,0 };

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	indexData.pSysMem = indicesPlane;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = dx11->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result)) {
		OutputDebugString("Error: Failed to create vertex buffer for 3D cube in app3\n");
		assert(false);
		return false;
	}
	result = dx11->getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result)) {
		OutputDebugString("Error: Failed to create index buffer for 3D cube in app3\n");
		assert(false);
		return false;
	}

	return true;
}

bool DX11App3::cleanUp() {
	if (shader) {
		shader->cleanUp();
		shader = 0;
	}
	if (texture) {
		texture->cleanUp();
		texture = 0;
	}

	return true;
}