#ifndef TINYXR_IMPL_MANAGER_H
#define TINYXR_IMPL_MANAGER_H

#include "tinyxr/core/config.h"
#include "tinyxr/core/tinyxr.h"

#include "tinyxr/impl/context.h"
#include "tinyxr/impl/openxr_extensions.h"

struct GLFWwindow;

TINYXR_NAMESPACE_OPEN

class ManagerXRImpl {
  ManagerXRImpl(const ManagerXRImpl &) = delete;
  ManagerXRImpl(const ManagerXRImpl &&) = delete;
  ManagerXRImpl &operator=(const ManagerXRImpl &) = delete;
  ManagerXRImpl &operator=(const ManagerXRImpl &&) = delete;

public:
  ManagerXRImpl(const Config &config);
  ~ManagerXRImpl() = default;

  bool init();

private:
  // Instance
  bool logInstanceInfo();
  bool createInstanceImpl();
  bool logLayersAndExtensions();
  bool createInstance();

  // System
  bool getSystem();

  // Session
  bool createSessionImpl();
  bool initGraphicsApi(const XrGraphicsRequirements
                           &requirements); // TODO! support all graphics API
  bool initializeGraphicsDevice();
  bool logEnvironmentBlendMode(const XrViewConfigurationType type);
  bool logViewConfigurations();
  bool createSession();

  // Actions
  bool createActions();

  // Spaces
  bool createReferenceSpacesImpl();
  bool logReferenceSpaces();
  bool createReferenceSpaces();

  Config mConfig;

  ExtensionsInfo mExtensionsInfo;
  std::unique_ptr<ExtensionsFunction> mExtensionsFunction;

  Context mContext;

  // TODO! support all graphics API
  GLFWwindow *mGraphicsContext;
};

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_IMPL_MANAGER_H
