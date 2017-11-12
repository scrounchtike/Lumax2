
#pragma once

#ifndef DX11_APP5_HPP
#define DX11_APP5_HPP

#include "DX11App.hpp"

#include "../../rendering/Text.hpp"

class DX11App5 : public DX11App {
public:
	DX11App5();
	DX11App5(const DX11App5&);
	~DX11App5();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
	ShaderDX11* shader;
	Model2DDX11* plane;
	TextureDX11* tex1,* tex2;
};

#endif