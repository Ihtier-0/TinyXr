#ifndef TINYXR_IMPL_CONTEXTXR_H
#define TINYXR_IMPL_CONTEXTXR_H

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/openxr.h"

TINYXR_NAMESPACE_OPEN

struct ContextXr {
  XrInstance instance = XR_NULL_HANDLE;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_CONTEXTXR_H
