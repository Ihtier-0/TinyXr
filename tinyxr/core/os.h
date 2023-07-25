#ifndef TINYXR_CORE_OS_H
#define TINYXR_CORE_OS_H

// based on:
// https://github.com/StereoKit/StereoKit/blob/master/StereoKitC/systems/platform/platform_utils.h
#if defined(__EMSCRIPTEN__)
#define TINYXR_OS_WEB
#define TINYXR_XR_WEBXR
#elif defined(__ANDROID__)
#define TINYXR_OS_ANDROID
#define TINYXR_XR_OPENXR
#elif defined(__linux__)
#define TINYXR_OS_LINUX
#define TINYXR_XR_OPENXR
#elif defined(WINDOWS_UWP)
#define TINYXR_OS_WINDOWS_UWP
#define TINYXR_XR_OPENXR
#elif defined(_WIN32)
#define TINYXR_OS_WINDOWS
#define TINYXR_XR_OPENXR
#endif

#if !defined(NDEBUG)
#define TINYXR_DEBUG
#endif

#endif  // TINYXR_CORE_OS_H
