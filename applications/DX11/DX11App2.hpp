
#pragma once

#ifndef DX11_APP_2
#define DX11_APP_2

#include "DX11App.hpp"

class DX11App2 : public DX11App {
public:
	DX11App2();
	DX11App2(const DX11App2&);
	~DX11App2();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
	ShaderDX11* shader;
	TextureDX11* texture;

	ID3D11Buffer* vertexBuffer,* indexBuffer;
};

#endif