
#pragma once

#ifndef DX11_APP_7
#define DX11_APP_7

#include "DX11App.hpp"

#include "../../OBJ_Loader.hpp"

// Render to texture
#include "../../rendering/RenderTextureBuffer.hpp"

class DX11App7 : public DX11App {
public:
	DX11App7();
	DX11App7(const DX11App7&);
	~DX11App7();
	
	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* context);
	bool cleanUp();
private:
	ShaderDX11* shader;
	Model3DDX11* model;
	Model3DDX11* cube;
	TextureDX11* texture;

	RenderTextureBuffer* renderTextureBuffer;

	Camera* camera;
};

#endif