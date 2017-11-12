
#pragma once

#ifndef CPU_PROFILER_HPP
#define CPU_PRFILER_HPP

#include <pdh.h>

#pragma comment(lib, "pdh.lib")

class CPUProfiler {
public:
	CPUProfiler();
	CPUProfiler(const CPUProfiler&);
	~CPUProfiler();

	void frame();
	int getCpuUsage() { return cpuUsage; }
	bool ableToReadCPU() { return canReadCPU; }

	void initialize();
	void cleanUp();
private:
	bool canReadCPU;
	HQUERY queryHandle;
	HCOUNTER counterHandle;
	unsigned long lastSampleTime;
	long cpuUsage;
};

#endif