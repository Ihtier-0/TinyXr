#ifndef TINYXR_IMPL_CONTEXT_H
#define TINYXR_IMPL_CONTEXT_H

#include "tinyxr/impl/openxr.h"

struct Context {
  XrFormFactor formFactor = XR_FORM_FACTOR_MAX_ENUM;
  XrEnvironmentBlendMode environmentBlendMode =
      XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM;
  XrViewConfigurationType viewConfigurationType =
      XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM;

  XrInstance instance = XR_NULL_HANDLE;
  XrSystemId systemId = XR_NULL_SYSTEM_ID;
  XrSession session = XR_NULL_HANDLE;
};

#endif // TINYXR_IMPL_CONTEXT_H
