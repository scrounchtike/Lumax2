
#pragma once

#ifndef DX11_APP_HPP
#define DX11_APP_HPP

#include "../../main.hpp"

#include "../../rendering/Camera.hpp"
#include "../../RL/Model3DDX11.hpp"
#include "../../RL/Model2DDX11.hpp"
#include "../../rendering/TextureDX11.hpp"
#include "../../rendering/ShaderDX11.hpp"

#include "../../RAL/RenderingContext.hpp"

class DX11App {
public:
	//DX11App();
	//DX11App(const DX11App&);
	//~DX11App();

	virtual void input() = 0;
	virtual  void update() = 0;
	virtual void render() = 0;

	virtual bool initialize(RenderingContextDX11* dx11) = 0;
	virtual bool cleanUp() = 0;
protected:
	RenderingContextDX11* dx11;
};

#endif