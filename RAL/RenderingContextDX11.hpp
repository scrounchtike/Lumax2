
#pragma once

#ifndef RENDERING_CONTEXT_DX11_HPP
#define RENDERING_CONTEXT_DX11_HPP

#include "RenderingContext.hpp"

#include "UsingDX11.hpp"

#include <vector>
#include <string>
#include <map>

struct DepthStencilState {
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
};

class RenderingContextDX11 : public RenderingContext {
public:
	RenderingContextDX11(StateDescription state, HWND hwnd);
	~RenderingContextDX11();

	virtual void clearColorBuffer(float color[4]);
	virtual void clearDepthBuffer(float depthValue = 1.0);
	virtual void clearBuffers(float color[4], float depthValue = 1.0);

	virtual void swapBuffers();

	// Allow state modification
	void registerDepthStencilView(DepthStencilDescription depthStencilDescription, const std::string& stateIdentifier);

	// Static state modification for transparency
	//void enableTransparency();
	//void disableTransparency();

	// No disable functions included because we do not want an undefined behaviour
	// Always have a depth/stencil state selected basically
	void enableState(const std::string& stateIdentifier);

	ID3D11Device* getDevice() { return device; }
	ID3D11DeviceContext* getDeviceContext() { return deviceContext; }
private:
	StateDescription state;

	// Window ref
	HWND hwnd;

	// DX11 State declarations
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

	ID3D11RasterizerState* rasterState;
	ID3D11RenderTargetView* renderTargetView;

	int depthStencilIndex = 0;
	std::vector<DepthStencilState> depthStencilDescriptions;
	std::map<const std::string, int> map;

	ID3D11BlendState* blendState;
	ID3D11BlendState* transparencyState;

	D3D_FEATURE_LEVEL featureLevel;

	bool initialize();
	bool cleanUp();

	bool createRenderBuffer();
	bool createDepthStencilBuffer();
	bool createRasterMode();
	bool createBlendMode();
	bool createViewport();
};

#endif