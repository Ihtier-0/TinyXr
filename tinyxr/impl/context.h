#ifndef TINYXR_IMPL_CONTEXT_H
#define TINYXR_IMPL_CONTEXT_H

#include "tinyxr/core/tinyxr.h"

#include "tinyxr/impl/action_set.h"
#include "tinyxr/impl/openxr.h"
#include "tinyxr/impl/openxr_extensions.h"

TINYXR_NAMESPACE_OPEN

using SpaceMap = std::unordered_map<XrReferenceSpaceType, XrSpace>;

struct Context {
  XrFormFactor formFactor = XR_FORM_FACTOR_MAX_ENUM;
  XrEnvironmentBlendMode environmentBlendMode =
      XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM;
  XrViewConfigurationType viewConfigurationType =
      XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM;

  XrInstance instance = XR_NULL_HANDLE;
  XrSystemId systemId = XR_NULL_SYSTEM_ID;
  XrSession session = XR_NULL_HANDLE;

  ActionSetsMap actionSets;

  SpaceMap referenceSpaces;

  SubactionPathMap subactionPaths;

  int64_t colorSwapchainFormat;

  std::vector<XrViewConfigurationView> configurationViews;
  std::vector<XrSwapchain> swapchains;
  std::vector<XrSwapchainImage> imageBuffer;
  std::map<XrSwapchain, XrSwapchainImageBaseHeader *> swapchainImages;

  bool exitRenderLoop = false;
  bool requestRestart = false;
  bool requestExit = false; // TODO! mb atomic

  XrEventDataBuffer eventDataBuffer;

  XrSessionState sessionState = XR_SESSION_STATE_UNKNOWN;

  bool sessionRunning = false;
};

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_IMPL_CONTEXT_H
