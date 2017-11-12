
#include <cassert>

#include "WindowWinAPI.hpp"

bool WindowWinAPI::exitRequested = false;

WindowWinAPI::WindowWinAPI() {
	width = height = 0;
	hwnd = 0;
	hInstance = 0;
	hdc = 0;
	openGLContext = 0;
	exitRequested = false;
}

WindowWinAPI::WindowWinAPI(const WindowWinAPI&) { }
WindowWinAPI::~WindowWinAPI() { }

bool WindowWinAPI::shouldClose() {
	return exitRequested;
}

void WindowWinAPI::clear() {
	float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	renderContext->clearBuffers(color);
}

void WindowWinAPI::input() {
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Direct Input
	directInput->frame();
}

void WindowWinAPI::update() {
	renderContext->swapBuffers();
}

bool WindowWinAPI::wasKeyPressed(int key) {
	return directInput->wasKeyPressed(key);
}

bool WindowWinAPI::wasKeyJustPressed(int key) {
	return directInput->wasKeyJustPressed(key);
}

bool WindowWinAPI::wasButtonPressed(int button) {
	return directInput->wasButtonPressed(button);
}

bool WindowWinAPI::wasButtonJustPressed(int button) {
	return directInput->wasButtonJustPressed(button);
}

void WindowWinAPI::getMousePosition(int& posX, int& posY) {
	//directInput->getMouseLocation(posX, posY);
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	posX = pt.x;
	posY = pt.y;
}

void WindowWinAPI::setTitle(const std::string& title) {
	SetWindowText(hwnd, title.c_str());
}

void WindowWinAPI::showCursor(bool show) {
	// TODO: Implement cursor mechanics
	ShowCursor(show);
	ShowCursor(show);
}

void WindowWinAPI::setCursorPosition(int xpos, int ypos) {
	POINT pt;
	pt.x = xpos;
	pt.y = ypos;
	ClientToScreen(hwnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

unsigned char WindowWinAPI::getLastCharacter() {
	// TODO: Implement text recognition from sequence of characters
	return 0;
}

bool WindowWinAPI::initialize(int nCmdShow, const std::string& stringName, unsigned int width, unsigned int height) {
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));

	this->width = width;
	this->height = height;
	this->title = stringName;
	this->hInstance = GetModuleHandle(NULL);

	wc.lpfnWndProc = windowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = (LPCSTR)title.c_str();

	RegisterClass(&wc);

	int posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	// TODO: Style of CS_OWNDC is only for setting our openGL with WGL in the window
	// TODO: Check if we use OpenGL or DirectX for the style
	hwnd = CreateWindowEx(0, (LPCSTR)title.c_str(), (LPCSTR)title.c_str(), WS_OVERLAPPEDWINDOW,
		posX, posY, width, height, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		OutputDebugString("Error: Could not create Win API window\n");
		assert(false);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(true);

	// Get window position
	WINDOWPLACEMENT windowPlacement;
	windowPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd, &windowPlacement);
	windowPosX = windowPlacement.ptMinPosition.x;
	windowPosY = windowPlacement.ptMaxPosition.y;

	windowPosX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2.0;
	windowPosY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2.0;

	// Direct Input initialization
	directInput = new DirectInput();
	directInput->initialize(hInstance, hwnd, width, height);

	return true;
}

bool WindowWinAPI::shutdown() {
	// Only for OpenGL stuff
	/*
	HDC hdc;
	wglMakeCurrent(hdc, NULL);

	// Deleting the context
	wglDeleteContext(openGLContext);
	*/

	ShowCursor(true);

	DestroyWindow(hwnd);
	hwnd = 0;

	UnregisterClass((LPCSTR)title.c_str(), hInstance);
	hInstance = NULL;

	return true;
}

bool WindowWinAPI::initOpenGL() {
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	hdc = GetDC(hwnd);

	// Getting and setting the pixel format
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	// Creating the openGL context
	openGLContext = wglCreateContext(hdc);
	// Making the GL context current for rendering purposes
	wglMakeCurrent(hdc, openGLContext);

	// Doing the GLEW init here is perhaps not very clear for the user
	if (glewInit()) {
		OutputDebugString("Error: Failed to initialize GLEW\n");
		assert(false);
		return false;
	}

	glClearColor(0, 0, 0.7f, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

bool WindowWinAPI::initGLEW() {
	return true;
}

bool WindowWinAPI::shutdownOpenGL() {
	return true;
}

bool WindowWinAPI::shutdownGLEW() {
	return true;
}

bool WindowWinAPI::initDirectX11() {
	//dx11 = new DX11Context();
	//return dx11->initialize(hwnd, width, height);

	StateDescription stateDesc;
	renderContext = new RenderingContextDX11(stateDesc, hwnd);

	return true;
}

bool WindowWinAPI::shutdownDirectX11() {
	delete renderContext;
	return true;
}

void WindowWinAPI::processKey(UINT umsg, WPARAM wparam, LPARAM lparam) {
	// This method of input wont work anymore..
	/*
	if (umsg == WM_KEYDOWN)
		Input::keys[wparam] = true;
	else
		Input::keys[wparam] = false;
	*/
}

LRESULT CALLBACK WindowWinAPI::windowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
		// Not needed since we now use DirectInput
		/*
	case WM_KEYDOWN:
		processKey(umsg, wparam, lparam);
		break;
	case WM_KEYUP:
		processKey(umsg, wparam, lparam);
		*/
	case WM_CLOSE:
		PostQuitMessage(0);
		WindowWinAPI::exitRequested = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		WindowWinAPI::exitRequested = true;
		break;
	case WM_QUIT:
		WindowWinAPI::exitRequested = true;
	default:
		DefWindowProc(hwnd, umsg, wparam, lparam);
		break;
	}
	return 1;
}