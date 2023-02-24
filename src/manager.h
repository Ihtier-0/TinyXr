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

  Context mContext;

  std::shared_ptr<ExtensionsInfo> mExtensionsInfo = nullptr;
  std::shared_ptr<ExtensionsFunction> mExtensionsFunction = nullptr;
};

#endif // MANAGER_H
