#ifndef TINYXR_CORE_FAVOR_HIGH_PERFORMANCE_GPU_H
#define TINYXR_CORE_FAVOR_HIGH_PERFORMANCE_GPU_H

#include "tinyxr/core/os.h"

// based on:
// https://github.com/KhronosGroup/OpenXR-SDK-Source/blob/main/src/tests/hello_xr/main.cpp
#if defined(TINYXR_OS_WINDOWS)
#include <windows.h>
// Favor the high performance NVIDIA or AMD GPUs
extern "C" {
// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
// https://gpuopen.com/learn/amdpowerxpressrequesthighperformance/
__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif  // TINYXR_OS_WINDOWS

#endif  // TINYXR_CORE_FAVOR_HIGH_PERFORMANCE_GPU_H
