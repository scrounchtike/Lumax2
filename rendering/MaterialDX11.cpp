
#pragma once

#include "MaterialDX11.hpp"

MaterialDX11::MaterialDX11() {
	hasTexture = false;
	hasColor = false;
	blendingFactor = 0;
}

MaterialDX11::MaterialDX11(const MaterialDX11& other) { }
MaterialDX11::~MaterialDX11() { }

void MaterialDX11::bind(RenderingContextDX11* dx11, ShaderDX11* shader) {
	// Bind material specific uniforms

	// Bind textures
	if (hasTexture) {
		const int numTextures = 1;
		ID3D11ShaderResourceView* textures[numTextures] = { texture->getTexture() };
		shader->bindTexture(dx11, textures, numTextures);
	}

	if (hasColor) {
		// Add a color uniform after the matrix uniforms
		int matrixUniformBufferSize = 48;
	}
}

void MaterialDX11::addColor(Vec4 color, float blend) {
	this->color = color;
	hasColor = true;
	blendingFactor = blend;
}

void MaterialDX11::addTexture(TextureDX11* texture, float blend) {
	this->texture = texture;
	hasTexture = true;
	blendingFactor = blend;
}

void MaterialDX11::initialize(Vec4 color) {
	this->color = color;
	blendingFactor = FULL_COLOR;
	hasColor = true;
}

void MaterialDX11::initialize(TextureDX11* texture) {
	this->texture = texture;
	blendingFactor = FULL_TEXTURE;
	hasTexture = true;
}

void MaterialDX11::initialize(TextureDX11* texture, Vec4 color, float blendingFactor) {
	this->texture = texture;
	this->color = color;
	blendingFactor = blendingFactor;
	hasTexture = hasColor = true;
}

void MaterialDX11::cleanUp() {

}