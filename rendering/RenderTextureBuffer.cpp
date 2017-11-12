
#pragma once

#include "RenderTextureBuffer.hpp"

RenderTextureBuffer::RenderTextureBuffer() {

}

RenderTextureBuffer::RenderTextureBuffer(const RenderTextureBuffer& other) { }
RenderTextureBuffer::~RenderTextureBuffer() { }

void RenderTextureBuffer::setRenderTarget(RenderingContextDX11* dx11, ID3D11DepthStencilView* depthStencilView) {
	// Binding the render target view with the depth buffer to the pipeline
	dx11->getDeviceContext()->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Set the viewports to the texture size
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)texWidth;
	viewport.Height = (float)texHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Setting the viewport.
	dx11->getDeviceContext()->RSSetViewports(1, &viewport);
}

void RenderTextureBuffer::clearRenderTarget(RenderingContextDX11* dx11, ID3D11DepthStencilView* depthStencilView, float r, float g, float b, float a) {
	float color[4] = { r,g,b,a };

	// Clear the color buffer
	dx11->getDeviceContext()->ClearRenderTargetView(renderTargetView, color);

	// Clear the depth buffer
	dx11->getDeviceContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

bool RenderTextureBuffer::initialize(RenderingContextDX11* dx11, int texWidth, int texHeight) {
	HRESULT result;

	this->texWidth = texWidth;
	this->texHeight = texHeight;

	// Setup the texture2D we will be writing in
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = texWidth;
	textureDesc.Height = texHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	result = dx11->getDevice()->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture);
	if (FAILED(result)) {
		OutputDebugString("Error: Failed to create render target texture during Render Texture Buffer initialization\n");
		assert(false);
		return false;
	}

	// Setup the render target view
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = dx11->getDevice()->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView);
	if (FAILED(result)) {
		OutputDebugString("Error: Failed to create Render Target View during Render Texture initialization\n");
		assert(false);
		return false;
	}

	// Setup the shader resource for shader utilization
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture1D.MipLevels = 1;

	result = dx11->getDevice()->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);
	if (FAILED(result)) {
		OutputDebugString("Error: Failed to create Shader Resource object during Render Texture initialization\n");
		assert(false);
		return false;
	}

	return true;
}

bool RenderTextureBuffer::cleanUp() {
	if (shaderResourceView) {
		shaderResourceView->Release();
		shaderResourceView = 0;
	}
	if (renderTargetView) {
		renderTargetView->Release();
		renderTargetView = 0;
	}
	if (renderTargetTexture) {
		renderTargetTexture->Release();
		renderTargetTexture = 0;
	}

	return true;
}