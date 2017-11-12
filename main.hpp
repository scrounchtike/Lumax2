
#ifndef MAIN_HPP
#define MAIN_HPP

#include "RAL/Window.hpp"

#ifdef _USE_DIRECTX11
#include "RAL/RenderingContextDX11.hpp"
#elif defined _USE_OPENGL
#include "RAL/RenderingContextGL.hpp"
#endif

extern Window* window;
extern Window* getStaticWindow();
#ifdef _USE_DIRECTX11
extern RenderingContextDX11* getDX11RenderingContext();
#elif defined _USE_OPENGL
extern RenderingContextGL* getGLRenderingContext();
#endif

extern bool wasKeyPressed(int key);
extern bool wasKeyJustPressed(int key);
extern bool wasButtonPressed(int button);
extern bool wasButtonJustPressed(int button);

extern void getMousePosition(int&, int&);

#endif