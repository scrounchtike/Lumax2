
#pragma once

#ifndef RENDER_TEXTURE_BUFFER_HPP
#define RENDER_TEXTURE_BUFFER_HPP

#include "../RAL/RenderingContextDX11.hpp"

class RenderTextureBuffer {
public:
	RenderTextureBuffer();
	RenderTextureBuffer(const RenderTextureBuffer&);
	~RenderTextureBuffer();

	void setRenderTarget(RenderingContextDX11* dx11, ID3D11DepthStencilView* depthStencilView);
	void clearRenderTarget(RenderingContextDX11* dx11, ID3D11DepthStencilView* depthStencilView, float r, float g, float b, float a);
	
	ID3D11ShaderResourceView* getShaderResourceView() { return shaderResourceView; }

	bool initialize(RenderingContextDX11* dx11, int texWidth, int texHeight);
	bool cleanUp();
private:
	unsigned int texWidth, texHeight;

	ID3D11Texture2D* renderTargetTexture;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;
};

#endif