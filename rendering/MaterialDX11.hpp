
#pragma once

#ifndef MATERIAL_DX11_HPP
#define MATERIAL_DX11_HPP

#include "../RAL/RenderingContextDX11.hpp"

#include "../math.hpp"
#include "TextureDX11.hpp"
#include "ShaderDX11.hpp"

#define FULL_COLOR 0
#define FULL_TEXTURE 1;

class MaterialDX11 {
public:
	MaterialDX11();
	MaterialDX11(const MaterialDX11&);
	~MaterialDX11();

	void bind(RenderingContextDX11* dx11, ShaderDX11* shader);

	void addColor(Vec4 color, float blend);
	void addTexture(TextureDX11* texture, float blend);

	void initialize(TextureDX11* texture);
	void initialize(Vec4 color);
	void initialize(TextureDX11* texture, Vec4 color, float blendingFactor);

	void cleanUp();
private:
	float blendingFactor;
	Vec4 color;
	TextureDX11* texture;

	bool hasTexture;
	bool hasColor;
};

#endif