
#pragma once

#include "DX11App7.hpp"

DX11App7::DX11App7() {

}

DX11App7::DX11App7(const DX11App7& other) { }
DX11App7::~DX11App7() { }

void DX11App7::input() {
	camera->input();
}

void DX11App7::update() {
	camera->update();
}

void DX11App7::render() {
	struct UniformBuffer {
		Mat4 projection;
		Mat4 view;
		Mat4 transform;
		Vec4 clipPlane;
		float alpha;
		Vec3 padding;
	};
	UniformBuffer buffer;
	buffer.alpha = 1.0f;
	//buffer.projection = Mat4().initProjectionDX11(70.0, 800.0 / 800.0, 0.1f, 1000.0f);

	// Bind the shader for both renders
	shader->bind(dx11);

	// First render the cube to the offscreen buffer
	//renderTextureBuffer->setRenderTarget(dx11, dx11->getDepthStencilView());
	// Clear the render target texture
	//renderTextureBuffer->clearRenderTarget(dx11, dx11->getDepthStencilView(), 0.0f, 0.0f, 0.5f, 1.0f);

	// Change the camera postion to be facing the cube
	Vec3 position = Vec3(7, 0, 6);

	Mat4 view = Mat4().initCamera(Vec3(-1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));

	Mat4 camera_movement = Mat4().initTranslation(-position.x, -position.y, -position.z);
	buffer.view = view.mul(camera_movement);

	// Bind uniforms and texture
	buffer.projection = Mat4().initProjectionDX11(70.0, 1, 0.1f, 1000.0f);
	buffer.transform = Mat4().initTranslation(0, 0, 6);
	buffer.clipPlane = Vec4(0, 1, 0, 0);

	shader->bindUniforms(dx11, (float*)(&buffer));
	ID3D11ShaderResourceView* textures[1] = { texture->getTexture() };
	shader->bindTexture(dx11, textures, 1);

	// Actual render call
	cube->render();

	// Change the render target to the window
	//dx11->setRenderTarget();
	// Reset the viewports to the window size
	//dx11->setViewport();
	// Clear the depth buffer
	float color[4] = { 0.5f,0.5f,0.5f,1.0f };
	//dx11->clearBuffer(color);

	// Set uniforms for real render
	buffer.view = camera->getView();
	buffer.transform = Mat4().initTranslation(0, 0, 2);
	buffer.projection = camera->getProjection();
	buffer.clipPlane = Vec4(0, 0, 0, 0);
	shader->bindUniforms(dx11, (float*)(&buffer));

	// Get the texture from the offscreen buffer
	ID3D11ShaderResourceView* textureOffscreen[1] = { renderTextureBuffer->getShaderResourceView() };
	//ID3D11ShaderResourceView* textureOffscreen[1] = { texture->getTexture() };
	shader->bindTexture(dx11, textureOffscreen, 1);

	// Render the plane for displaying offscreen rendering texture
	model->render();

	// Set the uniforms for the semi transparent cube
	buffer.transform = Mat4().initTranslation(0, 1, -2);
	buffer.alpha = 0.5f;
	shader->bindUniforms(dx11, (float*)(&buffer));
	shader->bindTexture(dx11, textures, 1);

	// Render the transparent cube
	cube->render();
}

bool DX11App7::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	camera = new Camera(getStaticWindow(), Vec3(0, 0, 0));

	// Render to texture
	renderTextureBuffer = new RenderTextureBuffer();
	renderTextureBuffer->initialize(dx11, 512, 512);

	// Shader initialization
	shader = new ShaderDX11();
	shader->addAttribute("POSITION", 0, 3, false);
	shader->addAttribute("TEXCOORD", 0, 2, true);
	shader->addClipPlane();
	shader->initialize(dx11, "applications/DX11/app7/renderToTextureShader", 56);

	// Texture initialization
	texture = new TextureDX11();
	texture->initialize(dx11, "res/textures_dds/test.dds");

	// Model initialization
	float vertices[12] = { -1,-1,0,-1,1,0,1,1,0,1,-1,0 };
	float texCoords[8] = { 0,0,0,1,1,1,1,0 };
	int indices[6] = { 0,1,2,2,3,0 };
	model = new Model3DDX11(dx11);
	model->initialize(vertices, 4, indices, 6, texCoords);

	//OBJ_Loader::getModel(OBJ_Loader::loadModel("res/models/cube.obj"));
	cube = new Model3DDX11(dx11);
	cube->initialize("res/models/cube.obj");

	return true;
}

bool DX11App7::cleanUp() {
	return true;
}