
#pragma once

#ifndef DX11_APP1_HPP
#define DX11_APP1_HPP

#include "DX11App.hpp"

// Very temporary
struct Vertex {
	Vec3 position;
};

class DX11App1 : public DX11App {
public:
	DX11App1();
	DX11App1(const DX11App1&);
	~DX11App1();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
	ShaderDX11* basicShader;

	ID3D11Buffer* vertexBufferPoints;
	ID3D11Buffer* indexBufferPoints;
};

#endif