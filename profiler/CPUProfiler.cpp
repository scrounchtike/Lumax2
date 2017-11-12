
#include "CPUProfiler.hpp"

CPUProfiler::CPUProfiler() {

}

CPUProfiler::CPUProfiler(const CPUProfiler& other) { }
CPUProfiler::~CPUProfiler() { }

void CPUProfiler::frame() {
	PDH_FMT_COUNTERVALUE value;

	if (canReadCPU) {
		if ((lastSampleTime + 1000) < GetTickCount()) {
			lastSampleTime = GetTickCount();
			PdhCollectQueryData(queryHandle);
			PdhGetFormattedCounterValue(counterHandle, PDH_FMT_LONG, NULL, &value);
			cpuUsage = value.longValue;
		}
	}
}

void CPUProfiler::initialize() {
	PDH_STATUS status;

	canReadCPU = true;

	status = PdhOpenQuery(NULL, 0, &queryHandle);
	if (status != ERROR_SUCCESS)
		canReadCPU = false;
	status = PdhAddCounter(queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &counterHandle);
	if (status != ERROR_SUCCESS)
		canReadCPU = false;

	lastSampleTime = GetTickCount();
	cpuUsage = 0;
}

void CPUProfiler::cleanUp() {
	if (canReadCPU)
		PdhCloseQuery(queryHandle);
}