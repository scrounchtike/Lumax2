
#pragma once

#ifndef DX11_APP8_HPP
#define DX11_APP8_HPP

#include "DX11App.hpp"

#include "../../rendering/RenderTextureBuffer.hpp"

class DX11App8 : public DX11App {
public:
	DX11App8();
	DX11App8(const DX11App8&);
	~DX11App8();

	void input();
	void update();

	// Rendering reflections using a stencil trick
	void render();

	// Rendering reflections with projective texturing and render to texture buffer
	void render2();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
	Camera* camera;

	Model3DDX11* waterPatch;
	Model3DDX11* cube;
	Model2DDX11* plane;

	RenderTextureBuffer* renderTextureBuffer;

	TextureDX11* texture;

	ShaderDX11* waterShader;
	ShaderDX11* clipShader;
	ShaderDX11* shader2D;
	ShaderDX11* shader3D;
	ShaderDX11* reflectShader;
};

#endif