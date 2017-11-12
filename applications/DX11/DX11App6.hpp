
#pragma once

#ifndef DX11_APP6_HPP
#define DX11_APP6_HPP

#include "DX11App.hpp"

#include "../../rendering/Text.hpp"

class DX11App6 : public DX11App {
public:
	DX11App6();
	DX11App6(const DX11App6&);
	~DX11App6();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
	ShaderDX11* shader;
	TextureDX11* texture,* normalMap;
	Model3DDX11* model;

	Camera* camera;
};

#endif