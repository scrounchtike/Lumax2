
#pragma once

#include "FPSCounter.hpp"

FPSCounter::FPSCounter() {

}

FPSCounter::FPSCounter(const FPSCounter& other) { }
FPSCounter::~FPSCounter() { }

void FPSCounter::initialize() {
	fps = 0;
	count = 0;
	startTime = timeGetTime();
}

void FPSCounter::frame() {
	++count;
	if (timeGetTime() >= (startTime + 1000)) {
		fps = count;
		count = 0;
		startTime = timeGetTime();
	}
}