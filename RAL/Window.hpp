
#ifndef WINDOW_HPP
#define WINDOW_HPP

#if defined _WIN32 || defined _WIN64
#define _WINDOWS
#endif
#ifdef __unix
#define _UNIX
#endif

#include <string>
#include <cassert>

// Rendering Context
#include "RenderingContext.hpp"

// Inputs
#include "../DirectInput.hpp"

//#define _DX11_DEBUG

// Window system
#define _USE_WINAPI
//#define _USE_GLFW

//#define _USE_OPENGL
#define _USE_DIRECTX11

// Abstract Window Interface

// Store inputs
#define NUM_KEYS 512
#define NUM_BUTTONS 8

class Window {
public:
	virtual bool shouldClose() = 0;
	virtual void clear() = 0;
	virtual void input() = 0;
	virtual void update() = 0;

	virtual void setTitle(const std::string& title) = 0;
	
	virtual void showCursor(bool) = 0;
	virtual void setCursorPosition(int, int) = 0;
	virtual unsigned char getLastCharacter() = 0;

	// Input asbtraction layer
	//TODO: Using an OS universal key addressing system
	virtual bool wasKeyPressed(int key) = 0;
	virtual bool wasKeyJustPressed(int key) = 0;
	virtual bool wasButtonPressed(int button) = 0;
	virtual bool wasButtonJustPressed(int button) = 0;

	virtual void getMousePosition(int& posX, int& posY) = 0;

	// Should this class remain abstract?
	// Are there any advantages to an abstract class?
	// I think there are, so ...
	// TODO: Move this to the derived Window classes
	unsigned int getHeight() { return height; }
	unsigned int getWidth() { return width; }
	std::string getTitle() { return title;  }

	virtual bool initialize(int, const std::string&, unsigned int, unsigned int) = 0;
	virtual bool shutdown() = 0;

	virtual bool initOpenGL() = 0;
	virtual bool shutdownOpenGL() = 0;

	virtual bool initDirectX11() = 0;
	virtual bool shutdownDirectX11() = 0;

	virtual bool initGLEW() = 0;
	virtual bool shutdownGLEW() = 0;

	RenderingContext* getRenderingContext() { return renderContext; }
protected:
	unsigned int width, height;
	std::string title;

	RenderingContext* renderContext;
};

#endif