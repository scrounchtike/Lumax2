
#pragma once

#ifndef DX11_APP_10_HPP
#define DX11_APP_10_HPP

#include "DX11App.hpp"

#include "../../terrain/TerrainDX11.hpp"

class DX11App10 : public DX11App {
public:
	DX11App10();
	DX11App10(const DX11App10&);
	~DX11App10();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
	Camera* camera;

	TerrainDX11* terrain;

	ShaderDX11* shader;

	TextureDX11* dirt;
	TextureDX11* test;
	TextureDX11* normalMap;
};

#endif