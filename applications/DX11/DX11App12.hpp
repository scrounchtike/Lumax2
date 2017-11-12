
#pragma once

#ifndef DX11_APP_12_HPP
#define DX11_APP_12_HPP

#include "DX11App.hpp"

class DX11App12 : public DX11App {
public:
	DX11App12();
	~DX11App12();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* dx11);
	bool cleanUp();
private:
};

#endif