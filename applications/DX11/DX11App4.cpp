
#pragma once

#include "DX11App4.hpp"

DX11App4::DX11App4() {

}

DX11App4::DX11App4(const DX11App4& other) { }
DX11App4::~DX11App4() { }

void DX11App4::input() {

}

void DX11App4::update() {

}

void DX11App4::render() {
	text->renderDX11(dx11);
}

bool DX11App4::initialize(RenderingContextDX11* dx11) {
	this->dx11 = dx11;

	// Text
	Text::initTextDX11(dx11);
	text = new Text("Hello World!", 50, 50);

	return true;
}

bool DX11App4::cleanUp() {
	return true;
}