
#include "RenderAPI.hpp"

RenderAPI::RenderAPI() {

}

RenderAPI::RenderAPI(const RenderAPI& other) { }
RenderAPI::~RenderAPI() { }

bool RenderAPI::initOpenGL() {
	return true;
}

bool RenderAPI::shutdownOpenGL() {
	return true;
}

bool RenderAPI::initDirectX11() {
	return true;
}

bool RenderAPI::shutdownDirectX11() {
	return true;
}