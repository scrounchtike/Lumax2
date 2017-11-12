
#pragma once

#include "DX11App5.hpp"

#include "../../math.hpp"

DX11App5::DX11App5() {

}

DX11App5::DX11App5(const DX11App5& other) { }
DX11App5::~DX11App5() { }

void DX11App5::input() {

}

void DX11App5::update() {

}

void DX11App5::render() {
	shader->bind(dx11);
	// Shader uniforms
	Mat4* transform = new Mat4();
	transform->initTranslation(0, 0, 0);
	float* uniforms = (float*)transform;
	float matrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	shader->bindUniforms(dx11, matrix);
	delete transform;
	// Multitexturing
	ID3D11ShaderResourceView* textures[2] = { tex1->getTexture(), tex2->getTexture() };
	//ID3D11ShaderResourceView* textures[1] = { tex2->getTexture() };
	shader->bindTexture(dx11, textures, 2);

	plane->render();
}

bool DX11App5::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	// Shader
	shader = new ShaderDX11();
	shader->addAttribute("POSITION", 0, 2, false);
	shader->addAttribute("TEXCOORD", 0, 2, true);
	shader->initialize(dx11, "applications/DX11/app5/multitexture", 16);

	// Model 2D
	float vertices[8] = { -0.5,-0.5,-0.5,0.5,0.5,0.5,0.5,-0.5 };
	float texCoords[8] = { 0,0,0,1,1,1,1,0 };
	int indices[6] = { 0,1,2,2,3,0 };
	plane = new Model2DDX11(dx11);
	plane->initialize(vertices, 4, indices, 6, texCoords);

	// Textures
	tex1 = new TextureDX11();
	tex1->initialize(dx11, "res/textures_dds/grass.dds");
	tex2 = new TextureDX11();
	tex2->initialize(dx11, "res/textures_dds/bricks.dds");

	return true;
}

bool DX11App5::cleanUp() {
	return true;
}