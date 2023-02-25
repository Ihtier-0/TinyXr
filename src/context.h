#ifndef CONTEXT_H
#define CONTEXT_H

#include "openxr.h"

struct Context {
  XrInstance instance = XR_NULL_HANDLE;
  XrSystemId system = XR_NULL_SYSTEM_ID;
};

#endif // CONTEXT_H
