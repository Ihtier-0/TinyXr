#ifndef PLATFORM_H
#define PLATFORM_H

#include "os.h"

// clang-format off

// based on: https://github.com/StereoKit/StereoKit/blob/master/StereoKitC/systems/platform/openxr.h
#if defined(XR_OPENXR)

#if defined(OS_ANDROID)
  #define XR_USE_PLATFORM_ANDROID
  #define XR_USE_TIMESPEC
  #define XR_TIME_EXTENSION XR_KHR_CONVERT_TIMESPEC_TIME_EXTENSION_NAME
  #define XR_USE_GRAPHICS_API_OPENGL_ES

#elif defined(OS_LINUX)
  #if defined(LINUX_EGL)
    #define XR_USE_PLATFORM_EGL
    #define XR_USE_GRAPHICS_API_OPENGL_ES
  #else
    #define XR_USE_PLATFORM_XLIB
    #define XR_USE_GRAPHICS_API_OPENGL
  #endif

  #define XR_USE_TIMESPEC
  #define XR_TIME_EXTENSION XR_KHR_CONVERT_TIMESPEC_TIME_EXTENSION_NAME

#elif defined(OS_WEB)
  #define XR_USE_TIMESPEC
  #define XR_TIME_EXTENSION XR_KHR_CONVERT_TIMESPEC_TIME_EXTENSION_NAME
  #define XR_USE_GRAPHICS_API_OPENGL

#elif defined(OS_WINDOWS) || defined(OS_WINDOWS_UWP)
  #define XR_USE_GRAPHICS_API_OPENGL // now support OpenGL only
  #define XR_USE_PLATFORM_WIN32
  #define XR_TIME_EXTENSION                                                    \
    XR_KHR_WIN32_CONVERT_PERFORMANCE_COUNTER_TIME_EXTENSION_NAME

  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif  // !WIN32_LEAN_AND_MEAN

  #ifndef NOMINMAX
    #define NOMINMAX
  #endif  // !NOMINMAX

  #include <windows.h> // for HDC in openxr/openxr_platform.h
  #include <unknwn.h> // for IUnknown in openxr/openxr_platform.h

  #include <QOffscreenSurface>
  #include <QOpenGLContext>
  // #include <QtPlatformHeaders/QWGLNativeContext>
  #include <wingdi.h>
#endif

#endif // XR_OPENXR

// clang-format on

#endif // PLATFORM_H
