
#ifndef MAIN_HPP
#define MAIN_HPP

#include "RAL/Window.hpp"

extern Window* window;
extern Window* getStaticWindow();

extern bool wasKeyPressed(int key);
extern bool wasKeyJustPressed(int key);
extern bool wasButtonPressed(int button);
extern bool wasButtonJustPressed(int button);

extern void getMousePosition(int&, int&);

#endif