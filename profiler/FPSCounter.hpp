
#pragma once

#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

#pragma comment(lib, "winmm.lib")

#include "windows.h"
#include "mmsystem.h"

class FPSCounter {
public:
	FPSCounter();
	FPSCounter(const FPSCounter&);
	~FPSCounter();

	void initialize();
	void frame();

	int getFPS() { return fps; }
private:
	int fps, count;
	unsigned long startTime;
};

#endif