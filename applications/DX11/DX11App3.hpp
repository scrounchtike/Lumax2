
#pragma once

#ifndef DX11_APP3_HPP
#define DX11_APP3_HPP

#include "DX11App.hpp"

class DX11App3 : public DX11App {
public:
	DX11App3();
	DX11App3(const DX11App3&);
	~DX11App3();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* context);
	bool cleanUp();
private:
	ShaderDX11* shader;
	ShaderDX11* shader2D;
	TextureDX11* texture;
	Model3DDX11* model;
	Model2DDX11* sprite;

	Camera* camera;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};

#endif