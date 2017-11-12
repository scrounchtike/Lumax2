
#pragma once

#include "DX11App6.hpp"

DX11App6::DX11App6() {

}

DX11App6::DX11App6(const DX11App6& other) { }
DX11App6::~DX11App6() { }

void DX11App6::input() {
	camera->input();
}

void DX11App6::update() {
	camera->update();
}

void DX11App6::render() {
	shader->bind(dx11);

	//Mat4 projection = Mat4().initProjectionDX11(70.0f, 600.0 / 800.0, 0.1f, 1000.0f);
	//Mat4 view = Mat4().initCamera(Vec3(0, 0, -1), Vec3(0, 1, 0), Vec3(1, 0, 0));
	Mat4 transform = Mat4().initTranslation(0, 0, 0);

	struct Uniforms {
		Mat4 projection;
		Mat4 view;
		Mat4 transform;
		Vec3 cameraPos;
		float padding;
	};
	Uniforms uniforms;
	uniforms.projection = camera->getProjection();
	uniforms.view = camera->getView();
	uniforms.transform = transform;
	uniforms.cameraPos = camera->getPosition();
	uniforms.padding = 0;

	shader->bindUniforms(dx11, (float*)(&uniforms));
	ID3D11ShaderResourceView* textures[2] = { texture->getTexture(), normalMap->getTexture() };
	shader->bindTexture(dx11, textures, 2);
	
	model->render();
}

bool DX11App6::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	// Camera
	camera = new Camera(getStaticWindow(), Vec3(0, 0, 0));

	// Shader
	shader = new ShaderDX11();
	shader->addAttribute("POSITION", 0, 3, false);
	shader->addAttribute("TEXCOORD", 0, 2, true);
	shader->addAttribute("NORMAL", 0, 3, true);
	shader->addAttribute("TANGENT", 0, 3, true);
	shader->addAttribute("BITANGENT", 0, 3, true);
	shader->initialize(dx11, "applications/DX11/app6/normalShader", 49);

	// Textures
	texture = new TextureDX11();
	texture->initialize(dx11, "res/textures_dds/bricks3.dds");
	normalMap = new TextureDX11();
	normalMap->initialize(dx11, "res/textures_dds/bricks3_normal.dds");

	// Model
	float vertices[12] = { -1,0,-1,-1,0,1,1,0,1,1,0,-1 };
	float texCoords[8] = { 0,0,0,1,1,1,1,0 };
	float normals[12] = { 0,1,0,0,1,0,0,1,0,0,1,0 };
	float tangents[12] = { 1,0,0,1,0,0,1,0,0,1,0,0 };
	float bitangents[12] = { 0,0,-1,0,0,-1,0,0,-1,0,0,-1 };
	int indices[6] = { 0,1,2,2,3,0 };
	model = new Model3DDX11(dx11);
	model->initialize(vertices, 4, indices, 6, texCoords, normals, tangents, bitangents);
	
	return true;
}

bool DX11App6::cleanUp() {
	return true;
}