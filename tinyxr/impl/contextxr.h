#ifndef TINYXR_IMPL_CONTEXTXR_H
#define TINYXR_IMPL_CONTEXTXR_H

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/actionset.h"
#include "tinyxr/impl/openxr.h"
#include "tinyxr/impl/openxrutils.h"
#include "tinyxr/impl/swapchain.h"

TINYXR_NAMESPACE_OPEN

using ReferenceSpaceMap = std::unordered_map<XrReferenceSpaceType, XrSpace>;

struct ContextXr {
  XrInstance instance = XR_NULL_HANDLE;
  XrSystemId systemId = XR_NULL_SYSTEM_ID;
  XrSession session = XR_NULL_HANDLE;
  XrViewConfigurationType viewConfigurationType =
      XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM;
  XrEnvironmentBlendMode environmentBlendMode =
      XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM;
  ActionSetsMap actionSetsMap;
  ReferenceSpaceMap referenceSpaceMap;
  XrSpace appSpace = XR_NULL_HANDLE;
  int64_t swapchainFormat = 0;
  float nearZ = 0.0f;
  float farZ = 0.0f;
  std::vector<XrView> views;
  Swapchains swapchains;
  XrEventDataBuffer eventDataBuffer = valid<XrEventDataBuffer>();
  XrSessionState sessionState = XR_SESSION_STATE_UNKNOWN;
  bool sessionRunning = false;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_CONTEXTXR_H
