#ifndef TINYXR_IMPL_CONTEXT_H
#define TINYXR_IMPL_CONTEXT_H

#include "tinyxr/impl/openxr.h"

struct Context {
  XrInstance instance = XR_NULL_HANDLE;
  XrSystemId systemId = XR_NULL_SYSTEM_ID;
};

#endif // TINYXR_IMPL_CONTEXT_H
