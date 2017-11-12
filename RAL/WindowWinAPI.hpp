
#pragma once

#ifndef WINDOW_WIN_API_HPP
#define WINDOW_WIN_API_HPP

#include <string>

#include "RenderingContextDX11.hpp"
#include "Window.hpp"

class WindowWinAPI : public Window {
public:
	WindowWinAPI();
	WindowWinAPI(const WindowWinAPI&);
	~WindowWinAPI();

	bool shouldClose();
	void clear();
	void input();
	void update();

	// Input abstraction layer
	bool wasKeyPressed(int key);
	bool wasKeyJustPressed(int key);
	bool wasButtonPressed(int button);
	bool wasButtonJustPressed(int button);
	void getMousePosition(int&, int&);

	void setTitle(const std::string& title);

	void showCursor(bool);
	void setCursorPosition(int, int);
	unsigned char getLastCharacter();

	bool initialize(int, const std::string&, unsigned int, unsigned int);
	bool shutdown();

	bool initOpenGL();
	bool shutdownOpenGL();

	// TODO: GLEW initialization is the same independent of the window
	bool initGLEW();
	bool shutdownGLEW();

	bool initDirectX11();
	bool shutdownDirectX11();

	//DX11Context* getDX11Context() { return dx11; }
private:
	static bool exitRequested;
	unsigned int windowPosX, windowPosY;

	// OpenGL with Win32 window using WGL
	// TODO: Remove that
	HGLRC openGLContext;
	HDC hdc;

	// Window handles
	HWND hwnd;
	HINSTANCE hInstance;

	// Direct Input interface
	DirectInput* directInput;

	static void processKey(UINT umsg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
};

#endif