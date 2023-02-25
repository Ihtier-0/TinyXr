#ifndef OS_H
#define OS_H

// clang-format off

// based on: https://github.com/StereoKit/StereoKit/blob/master/StereoKitC/systems/platform/platform_utils.h
#if defined(__EMSCRIPTEN__)
  #define OS_WEB
  #define XR_WEBXR
  #define WEB_EXPORT EMSCRIPTEN_KEEPALIVE extern "C"
#elif defined(__ANDROID__)
  #define OS_ANDROID
  #define XR_OPENXR
#elif defined(__linux__)
  #define OS_LINUX
  #define XR_OPENXR
#elif defined(WINDOWS_UWP)
  #define OS_WINDOWS_UWP
  #define XR_OPENXR
#elif defined(_WIN32)
  #define OS_WINDOWS
  #define XR_OPENXR
#endif

#if !defined(NDEBUG)
  #define DEBUG
#endif

// clang-format on

#endif // OS_H
