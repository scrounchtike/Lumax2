
#include <iostream>

#include "main.hpp"
#include "RAL/Window.hpp"
#include "RAL/WindowGLFW.hpp"
#include "RAL/WindowWinAPI.hpp"

#include "rendering/BasicShader.hpp"

//Apps
#include "applications/App1.hpp"
#include "applications/App2.hpp"
#include "applications/App3.hpp"
#include "applications/App4.hpp"
#include "applications/App5.hpp"
#include "applications/App6.hpp"
#include "applications/App7.hpp"
#include "applications/App8.hpp"
#include "applications/App9.hpp"
// DX11 Apps
#include "applications/DX11/DX11App1.hpp"
#include "applications/DX11/DX11App2.hpp"
#include "applications/DX11/DX11App3.hpp"
#include "applications/DX11/DX11App4.hpp"
#include "applications/DX11/DX11App5.hpp"
#include "applications/DX11/DX11App6.hpp"
#include "applications/DX11/DX11App7.hpp"
#include "applications/DX11/DX11App8.hpp"
#include "applications/DX11/DX11App9.hpp"
#include "applications/DX11/DX11App10.hpp"
#include "applications/DX11/DX11App11.hpp"
#include "applications/DX11/DX11App12.hpp"

// Profiling
#include "profiler/FPSCounter.hpp"
#include "profiler/CPUProfiler.hpp"

// Radiosity
#include "radiosity/radiosity.hpp"

// Debug Rendering
#include "rendering/DebugRenderer.hpp"

Window* window;

Window* getStaticWindow() {
	return window;
}

bool wasKeyPressed(int key) {
	return window->wasKeyPressed(key);
}

bool wasKeyJustPressed(int key) {
	return window->wasKeyJustPressed(key);
}

bool wasButtonPressed(int button) {
	return window->wasButtonPressed(button);
}

bool wasButtonJustPressed(int button) {
	return window->wasButtonJustPressed(button);
}

void getMousePosition(int& posX, int& posY) {
	window->getMousePosition(posX, posY);
}

void run();

void input();
void update();
void render();

// DX11
DX11App* app;
// OpenGL
App8* app8;
App9* app9;

// Profiler
FPSCounter fpsCounter;
CPUProfiler cpuProfiler;

const std::string title = "3D Game Engine";
const unsigned int width = 800;
const unsigned int height = 600;

// Deciding if we make our own window using GLFW or if we use the WinAPI
#ifdef _UNIX
int main(int argc, const char* argv[]){
#elif defined _WINDOWS
#ifdef _USE_WINAPI
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){
#else
int main(int argc, const char* argv[]){
#endif
#endif

#ifdef UNICODE
	assert(false);
#endif

	// Creating the window
#ifdef _USE_GLFW
	window = new WindowGLFW();
#elif defined _USE_WINAPI
	window = new WindowWinAPI();

#endif
	// WinAPI style = nCmdShow
#ifdef _USE_GLFW
	bool windowInit = window->initialize(0, title, width, height);
#elif defined _USE_WINAPI
	bool windowInit = window->initialize(nCmdShow, title, width, height);
#endif
	assert(windowInit);

	// Initializing rendering API
	bool initAPI = false;
#ifdef _USE_OPENGL
	initAPI = window->initOpenGL();
#elif defined _USE_DIRECTX11
	initAPI = window->initDirectX11();
	//DX11System::initialize(((WindowWinAPI*)window)->getDX11Context());
#endif
	assert(initAPI);

	// GLEW could have been initialized during the OpenGL initialization phase
#if defined _USE_OPENGL && defined _WINDOWS
	bool glewInit = window->initGLEW();
	assert(glewInit);
#endif

	// Check that we also want transparency
#ifdef _USE_OPENGL
	//Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_BLEND);

	//printf("%s\n", glGetString(GL_VERSION));
	OutputDebugString((LPCSTR)glGetString(GL_VERSION));
	OutputDebugString("\n");
#endif

	fpsCounter = FPSCounter();
	fpsCounter.initialize();

	cpuProfiler = CPUProfiler();
	cpuProfiler.initialize();

	// NOT GOOD: Find a better solution to pass the DX11 context to applications that require it.
	// Perhaps through the Window Interface; informing the user if the DX11 context is created or not.
	app = new DX11App11();
	// This is not safe, check if the cast succeeded first
	app->initialize((RenderingContextDX11*)window->getRenderingContext());

	//initApp1();
	//initApp2();
	//App7::init();
	//app8 = new App8();
	//Radiosity::initRadiosity();
	
	run();

	app->cleanUp();
	//cleanUpApp1();
	//eanUpApp2();
	//App7::cleanUp();
	//app8->cleanUp();
	//Radiosity::cleanUpRadiosity();

	window->shutdown();
	// TODO: Clean up rendering API
	
	return 0;
}

void run(){
	//main loop
	while(!window->shouldClose()){
		input();
		update();
		fpsCounter.frame();
		cpuProfiler.frame();
		//TODO: Make the CPU Profiler have access to CPU data.
		//OutputDebugString(std::to_string(cpuProfiler.ableToReadCPU()).c_str());
		render();
		int cpuUsage = cpuProfiler.getCpuUsage();
		window->setTitle(title + "  |  FPS: " + std::to_string(fpsCounter.getFPS()) + "  |  CPU: " + std::to_string(cpuUsage));
	}
}

void input(){
	window->input();

	app->input();

	//inputApp1();
	//App7::input();
	//app8->input();
	//Radiosity::input();
}

void update(){
	app->update();

	//updateApp1();
	//App7::update();
	//app8->update();
	//Radiosity::update();

	//Debug Rendering
	//DebugRenderer::update();
}

void render(){
	window->clear();

	//render call

	app->render();

	//renderApp1();
	//App7::render();
	//app8->render();
	//Radiosity::render();

	//Debug Rendering
	//Taking care of by the application since the renderer needs camera information
	//DebugRenderer::render();

	window->update();
}