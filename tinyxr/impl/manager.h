#ifndef TINYXR_IMPL_MANAGER_H
#define TINYXR_IMPL_MANAGER_H

#include "tinyxr/core/config.h"
#include "tinyxr/core/irenderer.h"
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
  ManagerXRImpl(const Config &config, IRendererPtr renderer);
  ~ManagerXRImpl() = default;

  bool init();

  // RenderLoop
  // RenderLoop::Events
  bool pollEvents();

  bool exitRenderLoopRequested() const;
  bool restartRequested() const;
  bool exitRequested() const;
  bool sessionRunning() const;

  void requestExit();

  // RenderLoop::Actions
  bool pollActions();

  // RenderLoop::Frames
  bool beforeFrames();
  bool renderFrames();
  bool afterFrames();

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
                           &requirements);  // TODO! support all graphics API
  bool initializeGraphicsDevice();
  bool logEnvironmentBlendMode(const XrViewConfigurationType type);
  bool logViewConfigurations();
  bool createSession();

  // Actions
  bool suggestInteractionProfiles();
  bool createActions();

  // Spaces
  bool createReferenceSpacesImpl();
  bool logReferenceSpaces();
  bool createReferenceSpaces();

  // Swapchains
  XrSwapchainImageBaseHeader *allocateSwapchainImages(uint32_t capacity);
  int64_t selectColorSwapchainFormat(
      const std::vector<int64_t> &swapchainFormats);
  bool createSwapchainsImpl();
  bool logSystemProperties();
  bool createSwapchains();

  // RenderLoop

  // Events
  void handleSessionStateChangedEvent(
      const XrEventDataSessionStateChanged &stateChangedEvent);
  const XrEventDataBaseHeader *tryReadNextEvent();

  // locates
  bool locateViews();
  bool locateSpaces();

  Config mConfig;

  ExtensionsInfo mExtensionsInfo;
  std::unique_ptr<ExtensionsFunction> mExtensionsFunction;

  Context mContext;
  IRendererPtr mRenderer;

  // TODO! support all graphics API
  GLFWwindow *mGraphicsContext;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_MANAGER_H
