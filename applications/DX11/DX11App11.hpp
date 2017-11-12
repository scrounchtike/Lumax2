
#pragma once

#ifndef DX11_APP_11_HPP
#define DX11_APP_11_HPP

#include "DX11App.hpp"
#include "../../main.hpp"

#include "../../engine/RenderingEngineDX11.hpp"

#include "../../RL/Model3D.hpp"

class DX11App11 : public DX11App {
public:
	DX11App11();
	DX11App11(const DX11App11&);
	~DX11App11();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
	RenderingEngineDX11 engine;

	ShaderDX11* shader;
	TextureDX11* test;
	MaterialDX11* material;
	Model3D* model;
};

#endif