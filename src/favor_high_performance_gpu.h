#ifndef FAVOR_HIGH_PERFORMANCE_GPU_H
#define FAVOR_HIGH_PERFORMANCE_GPU_H

#include "os.h"

// based on:
// https://github.com/KhronosGroup/OpenXR-SDK-Source/blob/main/src/tests/hello_xr/main.cpp
#if defined(OS_WINDOWS)

#include <minwindef.h> // for DWORD

// Favor the high performance NVIDIA or AMD GPUs
extern "C" {
// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
// https://gpuopen.com/learn/amdpowerxpressrequesthighperformance/
__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}

#endif // defined(_WIN32)

#endif // FAVOR_HIGH_PERFORMANCE_GPU_H
