#ifndef CONTEXT_H
#define CONTEXT_H

#include "openxr.h"

struct SystemProperties {
  bool handTracking = false;
  bool depthLayer = false;
  bool eyeTracking = false;
};

struct Context {
  XrInstance instance = XR_NULL_HANDLE;
  XrDebugUtilsMessengerEXT debug = XR_NULL_HANDLE;
  XrSystemId system = XR_NULL_SYSTEM_ID;
  SystemProperties systemProperties;
};

#endif // CONTEXT_H
