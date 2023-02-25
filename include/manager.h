#ifndef MANAGER_H
#define MANAGER_H

#include <memory>
#include <set>

#include "config.h"
#include "context.h"

struct ExtensionsInfo;
struct ExtensionsFunction;

// based on:
// https://github.com/KhronosGroup/OpenXR-SDK-Source/blob/main/src/tests/hello_xr/openxr_program.h
// and
// https://github.com/StereoKit/StereoKit/blob/master/StereoKitC/systems/platform/openxr.h
class Manager {
public:
  Manager(const Config &config);

  bool init();

private:
  // Instance
  void logLayersAndExtensions();

  bool createInstanceInternal();

  void logInstanceInfo();

  bool createInstance();

  // System
  bool initializeSystem();

  // BlendMode
  XrEnvironmentBlendMode getPreferredBlendMode() const;

  //
  Config mConfig;

  Context mContext;

  // Instance
  std::shared_ptr<ExtensionsInfo> mExtensionsInfo = nullptr;
  std::shared_ptr<ExtensionsFunction> mExtensionsFunction = nullptr;

  // System
  XrFormFactor mFormFactor = XrFormFactor::XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

  // ViewConfigurationType
  XrViewConfigurationType mViewType =
      XrViewConfigurationType::XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

  // BlendMode
  const std::set<XrEnvironmentBlendMode> mAcceptableBlendModes = {
      XrEnvironmentBlendMode::XR_ENVIRONMENT_BLEND_MODE_OPAQUE,
      XrEnvironmentBlendMode::XR_ENVIRONMENT_BLEND_MODE_ADDITIVE,
      XrEnvironmentBlendMode::XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND};
  XrEnvironmentBlendMode mBlendMode =
      XrEnvironmentBlendMode::XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
};

#endif // MANAGER_H
