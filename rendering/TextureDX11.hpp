
#pragma once

#ifndef TEXTURE_DX11_HPP
#define TEXTURE_DX11_HPP

#include "../RAL/RenderingContextDX11.hpp"

class TextureDX11 {
public:
	TextureDX11();
	TextureDX11(const TextureDX11&);
	~TextureDX11();

	void bind() const;
	ID3D11ShaderResourceView* getTexture() const { return texture; }

	bool initialize(RenderingContextDX11*, LPCSTR);
	bool cleanUp();
private:
	ID3D11ShaderResourceView* texture;
};

#endif