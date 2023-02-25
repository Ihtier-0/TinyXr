#ifndef MANAGER_H
#define MANAGER_H

#include <memory>

#include "context.h"

struct ExtensionsInfo;
struct ExtensionsFunction;

// based on:
// https://github.com/KhronosGroup/OpenXR-SDK-Source/blob/main/src/tests/hello_xr/openxr_program.h
class Manager {
public:
  bool init();

private:
  // Instance
  void logLayersAndExtensions();

  bool createInstanceInternal();

  void logInstanceInfo();

  bool createInstance();

  // System
  bool initializeSystem();

  //

  Context mContext;

  // Instance
  std::shared_ptr<ExtensionsInfo> mExtensionsInfo = nullptr;
  std::shared_ptr<ExtensionsFunction> mExtensionsFunction = nullptr;

  // System
  XrFormFactor mFormFactor = XrFormFactor::XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
};

#endif // MANAGER_H
