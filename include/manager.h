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
  ~Manager();

  bool init();

private:
  // Instance
  void logLayersAndExtensions();
  bool createInstanceInternal();
  void logInstanceInfo();
  bool createInstance();

  // Debug
  bool initializeDebug();

  // System
  bool initializeSystemInternal();
  void getSystemProperties();
  bool initializeSystem();

  //
  Config mConfig;

  Context mContext;

  // Instance
  std::shared_ptr<ExtensionsInfo> mExtensionsInfo = nullptr;
  std::shared_ptr<ExtensionsFunction> mExtensionsFunction = nullptr;

  // System
  XrFormFactor mFormFactor = XrFormFactor::XR_FORM_FACTOR_MAX_ENUM;
};

#endif // MANAGER_H
