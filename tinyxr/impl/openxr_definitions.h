#ifndef TINYXR_IMPL_OPENXR_DEFINITIONS_H
#define TINYXR_IMPL_OPENXR_DEFINITIONS_H

#include "tinyxr/core/os.h"

// based on:
// https://github.com/StereoKit/StereoKit/blob/master/StereoKitC/systems/platform/openxr.h
#if defined(TINYXR_XR_OPENXR)

#if defined(TINYXR_OS_ANDROID)
#define XR_USE_PLATFORM_ANDROID
#define XR_USE_TIMESPEC
#define XR_TIME_EXTENSION XR_KHR_CONVERT_TIMESPEC_TIME_EXTENSION_NAME
#define XR_USE_GRAPHICS_API_OPENGL_ES

#elif defined(TINYXR_OS_LINUX)
#if defined(TINYXR_LINUX_EGL)
#define XR_USE_PLATFORM_EGL
#define XR_USE_GRAPHICS_API_OPENGL_ES
#else
#define XR_USE_PLATFORM_XLIB
#define XR_USE_GRAPHICS_API_OPENGL
#endif

#define XR_USE_TIMESPEC
#define XR_TIME_EXTENSION XR_KHR_CONVERT_TIMESPEC_TIME_EXTENSION_NAME

#elif defined(TINYXR_OS_WEB)
#define XR_USE_TIMESPEC
#define XR_TIME_EXTENSION XR_KHR_CONVERT_TIMESPEC_TIME_EXTENSION_NAME
#define XR_USE_GRAPHICS_API_OPENGL

#elif defined(TINYXR_OS_WINDOWS) || defined(TINYXR_OS_WINDOWS_UWP)
#define XR_USE_GRAPHICS_API_OPENGL
#define XR_USE_PLATFORM_WIN32
#define XR_TIME_EXTENSION \
  XR_KHR_WIN32_CONVERT_PERFORMANCE_COUNTER_TIME_EXTENSION_NAME

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif  // !NOMINMAX

#include <unknwn.h>   // for IUnknown in openxr/openxr_platform.h
#include <windows.h>  // for HDC in openxr/openxr_platform.h

#endif

#endif  // TINYXR_XR_OPENXR

#endif  // TINYXR_IMPL_OPENXR_PREDEFINITIONS_H
