
#include "WindowGLFW.hpp"

double WindowGLFW::Input::cursorX, WindowGLFW::Input::cursorY;
char WindowGLFW::Input::keys[NUM_KEYS];
char WindowGLFW::Input::keys_once[NUM_KEYS];
char WindowGLFW::Input::buttons[NUM_BUTTONS];
char WindowGLFW::Input::buttons_once[NUM_BUTTONS];
unsigned char WindowGLFW::Input::lastCharacter;

void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow*, double, double);
void button_callback(GLFWwindow*, int, int, int);

WindowGLFW::WindowGLFW() {
	window = 0;
	width = height = 0;
}

WindowGLFW::WindowGLFW(const WindowGLFW& other) { }
WindowGLFW::~WindowGLFW() { }

bool WindowGLFW::shouldClose() {
	return glfwWindowShouldClose(window);
}

void WindowGLFW::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowGLFW::input() {
	int previous_keys[NUM_KEYS];
	for (int i = 0; i < NUM_KEYS; ++i)
		previous_keys[i] = Input::keys[i];
	int previous_buttons[NUM_BUTTONS];
	for (int i = 0; i < NUM_BUTTONS; ++i)
		previous_buttons[i] = Input::buttons[i];

	glfwPollEvents();

	for (int i = 0; i < NUM_KEYS; ++i)
		Input::keys_once[i] = !previous_keys[i] && Input::keys[i];
	for (int i = 0; i < NUM_BUTTONS; ++i)
		Input::buttons_once[i] = !previous_buttons[i] && Input::buttons[i];
}

void WindowGLFW::update() {
	glfwSwapBuffers(window);
}

bool WindowGLFW::wasKeyPressed(int key) {
	return Input::keys[key];
}

bool WindowGLFW::wasKeyJustPressed(int key) {
	return Input::keys_once[key];
}

bool WindowGLFW::wasButtonPressed(int button) {
	return Input::buttons[button];
}

bool WindowGLFW::wasButtonJustPressed(int button) {
	return Input::buttons_once[button];
}

void WindowGLFW::getMousePosition(int& posX, int& posY) {
	double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	posX = (int)cursorX;
	posY = (int)cursorY;
}

void WindowGLFW::setTitle(const std::string& title) {
	glfwSetWindowTitle(window, title.c_str());
}

void WindowGLFW::showCursor(bool show) {
	if (show)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowGLFW::setCursorPosition(int xpos, int ypos) {
	glfwSetCursorPos(window, xpos, ypos);
	Input::cursorX = xpos;
	Input::cursorY = ypos;
}

unsigned char WindowGLFW::getLastCharacter() {
	unsigned char lastChar = Input::lastCharacter;
	Input::lastCharacter = 0;
	return lastChar;
}

bool WindowGLFW::initialize(int style, const std::string& title, unsigned int width, unsigned int height) {
	// This kinda feels weird. Perhaps move the title, width, height initialization in the constructor and then call this init function? 
	// After all, the constructor should always be necessary...(?)
	this->title = title;
	this->width = width;
	this->height = height;

	if (!glfwInit()) {
		// Add log statement
		assert(false);
		return false;
	}

	// GLFW hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window) {
		// Add log statement
		glfwTerminate();
		assert(false);
		return false;
	}

	// Window user pointer
	glfwSetWindowUserPointer(window, this);

	// Setting the callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, button_callback);

	// Vsync
	glfwSwapInterval(1);
	// Making the context current
	glfwMakeContextCurrent(window);

	// Have been bugs where if this is not included weird stuff happens.
	glfwPollEvents();

	return true;
}

bool WindowGLFW::shutdown() {
	glfwTerminate();

	return true;
}

bool WindowGLFW::initOpenGL() {
	glClearColor(0, 0, 0.7f, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	return true;
}

bool WindowGLFW::initGLEW() {
	if (glewInit()) {
		// Add log statement
		//OutputDebugString("Error: Failed to initialize GLEW\n");
		assert(false);
		return false;
	}
	return true;
}

bool WindowGLFW::initDirectX11() {
	return false;
}

bool WindowGLFW::shutdownDirectX11() {
	return false;
}

bool WindowGLFW::shutdownOpenGL() {
	return true;
}

bool WindowGLFW::shutdownGLEW() {
	return true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

}

void button_callback(GLFWwindow* window, int button, int action, int mods) {

}