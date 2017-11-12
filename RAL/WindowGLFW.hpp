
#pragma once

#ifndef WINDOW_GLFW_HPP
#define WINDOW_GLFW_HPP

#include <string>

#include "Window.hpp"

#include "RenderingContextGL.hpp"

class WindowGLFW : public Window {
public:
	WindowGLFW();
	WindowGLFW(const WindowGLFW&);
	~WindowGLFW();

	bool shouldClose();
	void clear();
	void input();
	void update();

	// Input asbtraction layer
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

	bool initGLEW();
	bool shutdownGLEW();

	bool initDirectX11();
	bool shutdownDirectX11();
private:
	GLFWwindow* window;

	class Input {
	public:
		static double cursorX, cursorY;
		static char keys[NUM_KEYS];
		static char keys_once[NUM_KEYS];
		static char buttons[NUM_BUTTONS];
		static char buttons_once[NUM_BUTTONS];
		static unsigned char lastCharacter;
	};

};

#endif