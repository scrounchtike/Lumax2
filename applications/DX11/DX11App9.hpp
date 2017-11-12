
#pragma once

#ifndef DX11_APP_9_HPP
#define DX22_APP_9_HPP	

#include "DX11App.hpp"

#include "../../rendering/Camera.hpp"
#include "../../RL/Model3DDX11.hpp"
#include "../../rendering/TextureDX11.hpp"
#include "../../rendering/ShaderDX11.hpp"

class DX11App9 : public DX11App {
public:
	DX11App9();
	DX11App9(const DX11App9&);
	~DX11App9();

	void input();
	void update();
	void render();

	void renderMipmap();
	void renderAnisotropic();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
	ShaderDX11* shader3D;

	Model3DDX11* planeModel;

	TextureDX11* texture;

	Camera* camera;
};

#endif