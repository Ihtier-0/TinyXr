#include "tinyxr/impl/managerxrfactoryimpl.h"

#include <json/json.h>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>

#include "tinyxr/core/iconfigxrconstants.h"
#include "tinyxr/core/iobject.h"
#include "tinyxr/core/irenderer.h"
#include "tinyxr/impl/contextxr.h"
#include "tinyxr/impl/openxrextensions.h"
#include "tinyxr/impl/openxrutils.h"
#include "tinyxr/impl/stringutils.h"

TINYXR_NAMESPACE_OPEN

static Json::Value readJson(const std::string& path) {
  std::ifstream ifs;
  ifs.open(path);
  if (!ifs.is_open()) {
    return {};
  }

  Json::Value root;
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, ifs, &root, &errs)) {
    std::cout << "errors: " << errs << " while open file: " << path
              << std::endl;
    return {};
  }

  return root;
}

////////////////////////////////////////////////////////////////////////////////
/// ManagerXrImpl
////////////////////////////////////////////////////////////////////////////////

class ManagerXrImpl : public IManagerXr {
public:
  ManagerXrImpl(IConfigXrPtr&& config) : mConfig(std::move(config)) {}
  ~ManagerXrImpl() {
    for (const auto& space : mContext.referenceSpaceMap) {
      xrDestroySpace(space.second);
    }

    for (const auto& actionSet : mContext.actionSetsMap) {
      for (const auto& action : actionSet.second.actions) {
        xrDestroyAction(action.second.action);
      }
      xrDestroyActionSet(actionSet.second.set);
    }

    if (mContext.session != XR_NULL_HANDLE) {
      xrDestroySession(mContext.session);
    }

    if (mContext.instance != XR_NULL_HANDLE) {
      xrDestroyInstance(mContext.instance);
    }
  }

  bool init() {
    auto* objectContext = TXR_NS::getObjectContext();
    if (!objectContext) {
      return false;
    }

    auto* rendererFactory = dynamic_cast<TXR_NS::IRendererFactory*>(
        objectContext->get(TXR_NS::ObjectType::RendererFactory));
    if (!rendererFactory) {
      return false;
    }

    mRenderer = rendererFactory->create(mConfig->getString(currentRenderer));
    if (!mRenderer) {
      return false;
    }

    mExtensionsInfo = std::make_unique<ExtensionsInfo>(
        mConfig->getStrings(TXR_NS::userRequestExtensions));

    if (!createInstance()) {
      return false;
    }

    mExtensionsFunction =
        std::make_unique<ExtensionsFunction>(mContext.instance);

    if (!getSystem()) {
      return false;
    }

    mContext.viewConfigurationType =
        XrViewConfigurationTypeFromString(mConfig->getString(viewConfigType));
    if (mContext.viewConfigurationType == XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM) {
      return false;
    }

    mContext.environmentBlendMode = XrEnvironmentBlendModeFromString(
        mConfig->getString(environmentBlendMode));
    if (mContext.environmentBlendMode == XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM) {
      return false;
    }

    mContext.nearZ = mConfig->getDouble(nearZ);
    mContext.farZ = mConfig->getDouble(farZ);

    if (!createSession()) {
      return false;
    }

    if (!createActions()) {
      return false;
    }

    if (!createReferenceSpace()) {
      return false;
    }

    if (!createSwapchains()) {
      return false;
    }

    return true;
  }

private:
  ////////////////////////////////////////////////////////////////////////////////
  /// Instance
  ////////////////////////////////////////////////////////////////////////////////

  bool logLayersAndExtensions() {
    const auto logExtensions = [](const char* layerName, int indent = 0) {
      std::vector<XrExtensionProperties> extensions;
      if (!TWO_CALL(std::bind(xrEnumerateInstanceExtensionProperties, layerName,
                              std::placeholders::_1, std::placeholders::_2,
                              std::placeholders::_3),
                    extensions)) {
        return false;
      }

      const auto maxElement = std::max_element(
          extensions.begin(), extensions.end(),
          [](const XrExtensionProperties& l, const XrExtensionProperties& r) {
            return std::strlen(l.extensionName) < std::strlen(r.extensionName);
          });
      const auto maxLen = maxElement != extensions.end()
                              ? std::strlen(maxElement->extensionName)
                              : 0;

      const std::string indentStr(indent, ' ');
      std::cout << indentStr << "Available Extensions: (" << extensions.size()
                << ")" << std::endl;
      for (const auto& extension : extensions) {
        std::cout << indentStr << "  Name=" << std::left << std::setw(maxLen)
                  << extension.extensionName
                  << " SpecVersion=" << extension.extensionVersion << std::endl;
      }

      return true;
    };

    if (!logExtensions(nullptr)) {
      return false;
    }

    std::vector<XrApiLayerProperties> layers;
    if (!TWO_CALL(xrEnumerateApiLayerProperties, layers)) {
      return false;
    }

    std::cout << "Available Layers: (" << layers.size() << ")" << std::endl;
    for (const XrApiLayerProperties& layer : layers) {
      std::cout << "  Name=" << layer.layerName
                << " SpecVersion=" << XrVersionToString(layer.specVersion)
                << " LayerVersion=" << layer.layerVersion
                << " Description=" << layer.description << std::endl;
      if (!logExtensions(layer.layerName, 4)) {
        return false;
      }
    }

    return true;
  }

  bool createInstanceImpl() {
    assert(mContext.instance == XR_NULL_HANDLE);

    std::vector<const char*> extensions;
    const auto pushBack = [&extensions](
                              const std::vector<std::string>& values) {
      std::transform(values.begin(), values.end(),
                     std::back_inserter(extensions),
                     [](const std::string& value) { return value.c_str(); });
    };

    pushBack(mExtensionsInfo->extensions);

    auto createInfo = valid<XrInstanceCreateInfo>();
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.enabledExtensionNames = extensions.data();

    auto& applicationInfo = createInfo.applicationInfo;
    const auto applicationName = mConfig->getString(TXR_NS::applicationName);
    copyStringToCharArray(applicationName, applicationInfo.applicationName);

    applicationInfo.apiVersion = XR_API_VERSION_1_0;

    copyStringToCharArray(TINYXR_STRING, applicationInfo.engineName);

    return XR_SUCCEEDED(xrCreateInstance(&createInfo, &mContext.instance));
  }

  bool logInstanceInfo() {
    assert(mContext.instance != XR_NULL_HANDLE);

    auto instanceProperties = valid<XrInstanceProperties>();
    if (XR_FAILED(
            xrGetInstanceProperties(mContext.instance, &instanceProperties))) {
      return false;
    }

    std::cout << "Instance RuntimeName=" << instanceProperties.runtimeName
              << " RuntimeVersion="
              << XrVersionToString(instanceProperties.runtimeVersion)
              << std::endl;
    return true;
  }

  bool createInstance() {
    if (!logLayersAndExtensions()) {
      return false;
    }

    if (!createInstanceImpl()) {
      return false;
    }

    if (!logInstanceInfo()) {
      return false;
    }

    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// System
  ////////////////////////////////////////////////////////////////////////////////

  bool getSystem() {
    assert(mContext.instance != XR_NULL_HANDLE);
    assert(mContext.systemId == XR_NULL_SYSTEM_ID);

    auto systemInfo = valid<XrSystemGetInfo>();
    const auto formFactorStr = mConfig->getString(formFactor);
    systemInfo.formFactor = XrFormFactorFromString(formFactorStr);

    if (XR_FAILED(
            xrGetSystem(mContext.instance, &systemInfo, &mContext.systemId))) {
      return false;
    }

    std::cout << "Using system " << mContext.systemId << " for form factor "
              << formFactorStr << std::endl;
    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Session
  ////////////////////////////////////////////////////////////////////////////////

  bool verifyRendererCompatibility() {
    auto graphicsRequirements = valid<XrGraphicsRequirements>();
    if (XR_FAILED(mExtensionsFunction->xrGetGraphicsRequirementsKHR(
            mContext.instance, mContext.systemId, &graphicsRequirements))) {
      return false;
    }

    const auto version = mRenderer->getVersion();
    const auto xrVersion =
        XR_MAKE_VERSION(version.major, version.minor, version.patch);

    if (graphicsRequirements.minApiVersionSupported > xrVersion ||
        graphicsRequirements.maxApiVersionSupported < xrVersion) {
      return false;
    }

    return true;
  }

  bool createSessionImpl() {
    assert(mContext.instance != XR_NULL_HANDLE);
    assert(mContext.systemId != XR_NULL_SYSTEM_ID);
    assert(mContext.session == XR_NULL_HANDLE);

    auto graphicsBinding = valid<XrGraphicsBinding>();
    // TODO! support other XrGraphicsBinding
    graphicsBinding.hDC = HDC(mRenderer->getDeviceContext());
    graphicsBinding.hGLRC = HGLRC(mRenderer->getHandleRenderContext());

    auto sessionCreateInfo = valid<XrSessionCreateInfo>();
    sessionCreateInfo.next = &graphicsBinding;
    sessionCreateInfo.systemId = mContext.systemId;
    if (XR_FAILED(xrCreateSession(mContext.instance, &sessionCreateInfo,
                                  &mContext.session))) {
      return false;
    }

    return true;
  }

  bool logSystemProperties() {
    auto systemProperties = valid<XrSystemProperties>();
    if (XR_FAILED(xrGetSystemProperties(mContext.instance, mContext.systemId,
                                        &systemProperties))) {
      return false;
    }

    std::cout << "System Properties: Name=" << systemProperties.systemName
              << " VendorId=" << systemProperties.vendorId << std::endl;
    std::cout << "System Graphics Properties: MaxWidth="
              << systemProperties.graphicsProperties.maxSwapchainImageWidth
              << " MaxHeight="
              << systemProperties.graphicsProperties.maxSwapchainImageHeight
              << " MaxLayers="
              << systemProperties.graphicsProperties.maxLayerCount << std::endl;
    std::cout << "System Tracking Properties: OrientationTracking= "
              << (systemProperties.trackingProperties.orientationTracking ==
                          XR_TRUE
                      ? "True"
                      : "False")
              << "PositionTracking="
              << (systemProperties.trackingProperties.positionTracking ==
                          XR_TRUE
                      ? "True"
                      : "False")
              << std::endl;
    return true;
  }

  bool createSession() {
    if (!verifyRendererCompatibility()) {
      return false;
    }

    if (!createSessionImpl()) {
      return false;
    }

    if (!logSystemProperties()) {
      return false;
    }

    return true;
  }

  bool beginSession() {
    assert(mContext.session != XR_NULL_HANDLE);
    auto sessionBeginInfo = valid<XrSessionBeginInfo>();
    sessionBeginInfo.primaryViewConfigurationType =
        mContext.viewConfigurationType;
    if (XR_FAILED(xrBeginSession(mContext.session, &sessionBeginInfo))) {
      return false;
    }

    mContext.sessionRunning = true;

    return true;
  }

  bool endSession() {
    if (XR_FAILED(xrEndSession(mContext.session))) {
      return false;
    }

    mContext.sessionRunning = false;

    return true;
  }

  bool handleSessionStateChangedEvent(
      const XrEventDataSessionStateChanged& eventDataBuffer) {
    std::cout << "XrEventDataSessionStateChanged: state "
              << XrSessionStateToString(mContext.sessionState) << "->"
              << XrSessionStateToString(eventDataBuffer.state)
              << " session=" << eventDataBuffer.session
              << " time=" << eventDataBuffer.time << std::endl;
    mContext.sessionState = eventDataBuffer.state;

    if (eventDataBuffer.session != mContext.session) {
      std::cout << "XrEventDataSessionStateChanged for unknown session"
                << std::endl;
      return false;
    }

    switch (mContext.sessionState) {
    case XR_SESSION_STATE_READY: {
      return beginSession();
    }
    case XR_SESSION_STATE_STOPPING: {
      return endSession();
    }
    }

    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Actions
  ////////////////////////////////////////////////////////////////////////////////

  bool createActions() {
    assert(mContext.instance != XR_NULL_HANDLE);

    Json::Value actionSetsConfigRoot =
        readJson(mConfig->getString(TXR_NS::actionSetsConfig));
    if (!actionSetsConfigRoot) {
      return false;
    }

    std::vector<XrActionSet> actionSets;
    for (const auto& actionSet : actionSetsConfigRoot) {
      const auto name = actionSet["name"].asString();
      const auto localisedName = actionSet["localisedName"].asString();
      const auto priority = actionSet["priority"].asInt();

      XrActionSet xrActionSet = XR_NULL_HANDLE;
      auto actionSetCreateInfo = valid<XrActionSetCreateInfo>();
      copyStringToCharArray(name, actionSetCreateInfo.actionSetName);
      copyStringToCharArray(localisedName,
                            actionSetCreateInfo.localizedActionSetName);
      actionSetCreateInfo.priority = priority;
      if (XR_FAILED(xrCreateActionSet(mContext.instance, &actionSetCreateInfo,
                                      &xrActionSet))) {
        continue;
      }

      ActionsMap actionsMap;
      const auto actions = actionSet["actions"];
      for (const auto& action : actions) {
        const auto paths = action["paths"];

        std::vector<XrPath> subactionPaths;
        SubactionPathMap subactionPathMap;
        for (const auto& path : paths) {
          const auto pathString = path.asString();
          XrPath xrPath;
          if (XR_FAILED(xrStringToPath(mContext.instance, pathString.c_str(),
                                       &xrPath))) {
            continue;
          }
          subactionPathMap[pathString] = xrPath;
          subactionPaths.emplace_back(xrPath);
        }

        const auto type = action["type"].asString();
        const auto name = action["name"].asString();
        const auto localisedName = action["localisedName"].asString();

        auto actionCreateInfo = valid<XrActionCreateInfo>();
        actionCreateInfo.actionType = XrActionTypeFromString(type);
        copyStringToCharArray(name, actionCreateInfo.actionName);
        copyStringToCharArray(localisedName,
                              actionCreateInfo.localizedActionName);
        actionCreateInfo.countSubactionPaths = subactionPaths.size();
        actionCreateInfo.subactionPaths = subactionPaths.data();
        XrAction xrAction = XR_NULL_HANDLE;
        if (XR_FAILED(
                xrCreateAction(xrActionSet, &actionCreateInfo, &xrAction))) {
          continue;
        }
        auto& action = actionsMap[name];
        action.type = actionCreateInfo.actionType;
        action.action = xrAction;
        action.subactionPaths = subactionPathMap;
      }
      auto& actionSet = mContext.actionSetsMap[name];
      actionSet.set = xrActionSet;
      actionSet.actions = actionsMap;

      actionSets.emplace_back(xrActionSet);
    }

    Json::Value interactionProfilesConfigRoot =
        readJson(mConfig->getString(TXR_NS::interactionProfilesConfig));
    if (!interactionProfilesConfigRoot) {
      return false;
    }

    for (const auto& interactionProfile : interactionProfilesConfigRoot) {
      const auto path = interactionProfile["path"].asString();
      const auto bindings = interactionProfile["bindings"];

      XrPath xrPath;
      if (XR_FAILED(xrStringToPath(mContext.instance, path.c_str(), &xrPath))) {
        continue;
      }

      std::vector<XrActionSuggestedBinding> xrBindings;
      for (const auto& binding : bindings) {
        const auto set = binding["set"].asString();
        const auto action = binding["action"].asString();
        const auto paths = binding["paths"];

        const auto findSet = mContext.actionSetsMap.find(set);
        if (findSet == mContext.actionSetsMap.end()) {
          continue;
        }
        const auto& actionSet = findSet->second;

        const auto findAction = actionSet.actions.find(action);
        if (findAction == actionSet.actions.end()) {
          continue;
        }
        const auto xrAction = findAction->second.action;

        XrActionSuggestedBinding binding;
        binding.action = xrAction;
        for (const auto& path : paths) {
          const auto pathString = path.asString();
          XrPath xrPath;
          if (XR_FAILED(xrStringToPath(mContext.instance, pathString.c_str(),
                                       &xrPath))) {
            continue;
          }
          binding.binding = xrPath;
          xrBindings.push_back(binding);
        }
      }

      auto suggestedBindings = valid<XrInteractionProfileSuggestedBinding>();
      suggestedBindings.interactionProfile = xrPath;
      suggestedBindings.countSuggestedBindings = xrBindings.size();
      suggestedBindings.suggestedBindings = xrBindings.data();
      if (XR_FAILED(xrSuggestInteractionProfileBindings(mContext.instance,
                                                        &suggestedBindings))) {
        continue;
      }
    }

    auto attachInfo = valid<XrSessionActionSetsAttachInfo>();
    attachInfo.countActionSets = actionSets.size();
    attachInfo.actionSets = actionSets.data();
    if (XR_FAILED(xrAttachSessionActionSets(mContext.session, &attachInfo))) {
      return false;
    }

    return true;
  }

  bool pollActions() {
    const auto actionSetsCount = mContext.actionSetsMap.size();
    std::vector<XrActiveActionSet> activeActionSets(actionSetsCount,
                                                    valid<XrActiveActionSet>());
    auto activeActionSetsIt = activeActionSets.begin();
    auto actionSetsIt = mContext.actionSetsMap.begin();
    for (; activeActionSetsIt != activeActionSets.end();
         ++activeActionSetsIt, ++actionSetsIt) {
      activeActionSetsIt->actionSet = actionSetsIt->second.set;
    }

    auto actionsSyncInfo = valid<XrActionsSyncInfo>();
    actionsSyncInfo.countActiveActionSets = actionSetsCount;
    actionsSyncInfo.activeActionSets = activeActionSets.data();
    if (XR_FAILED(xrSyncActions(mContext.session, &actionsSyncInfo))) {
      return false;
    }

    for (const auto& actionSet : mContext.actionSetsMap) {
      for (const auto& action : actionSet.second.actions) {
        auto actionStateGetInfo = valid<XrActionStateGetInfo>();
        actionStateGetInfo.action = action.second.action;

        for (const auto& subactionPath : action.second.subactionPaths) {
          actionStateGetInfo.subactionPath = subactionPath.second;

          switch (action.second.type) {
          case XR_ACTION_TYPE_BOOLEAN_INPUT: {
            auto actionStateBoolean = valid<XrActionStateBoolean>();
            if (XR_FAILED(xrGetActionStateBoolean(mContext.session,
                                                  &actionStateGetInfo,
                                                  &actionStateBoolean))) {
              break;
            }
            break;
          }
          case XR_ACTION_TYPE_FLOAT_INPUT: {
            auto actionStateFloat = valid<XrActionStateFloat>();
            if (XR_FAILED(xrGetActionStateFloat(mContext.session,
                                                &actionStateGetInfo,
                                                &actionStateFloat))) {
              break;
            }
            break;
          }
          case XR_ACTION_TYPE_VECTOR2F_INPUT: {
            auto actionStateVector2f = valid<XrActionStateVector2f>();
            if (XR_FAILED(xrGetActionStateVector2f(mContext.session,
                                                   &actionStateGetInfo,
                                                   &actionStateVector2f))) {
              break;
            }
            break;
          }
          case XR_ACTION_TYPE_POSE_INPUT: {
            auto actionStatePose = valid<XrActionStatePose>();
            if (XR_FAILED(xrGetActionStatePose(
                    mContext.session, &actionStateGetInfo, &actionStatePose))) {
              break;
            }
            break;
          }
          default:
            break;
          }
        }
      }
    }

    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Spaces
  ////////////////////////////////////////////////////////////////////////////////

  bool logReferenceSpaces() {
    assert(mContext.session != XR_NULL_HANDLE);

    std::vector<XrReferenceSpaceType> referenceSpaceTypes;
    if (!TWO_CALL(std::bind(xrEnumerateReferenceSpaces, mContext.session,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3),
                  referenceSpaceTypes)) {
      return false;
    }

    std::cout << "Available reference spaces: " << referenceSpaceTypes.size()
              << std::endl;
    for (const auto referenceSpaceType : referenceSpaceTypes) {
      std::cout << "  Name: "
                << XrReferenceSpaceTypeToString(referenceSpaceType)
                << std::endl;
    }

    return true;
  }

  bool createReferenceSpaceImpl() {
    std::vector<XrReferenceSpaceType> referenceSpaceTypes;
    if (!TWO_CALL(std::bind(xrEnumerateReferenceSpaces, mContext.session,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3),
                  referenceSpaceTypes)) {
      return false;
    }

    for (const auto referenceSpaceType : referenceSpaceTypes) {
      auto referenceSpaceCreateInfo = valid<XrReferenceSpaceCreateInfo>();
      referenceSpaceCreateInfo.referenceSpaceType = referenceSpaceType;
      XrSpace space = XR_NULL_HANDLE;
      if (XR_FAILED(xrCreateReferenceSpace(
              mContext.session, &referenceSpaceCreateInfo, &space))) {
        continue;
      }
      mContext.referenceSpaceMap[referenceSpaceType] = space;
    }

    const auto appSpaceType =
        XrReferenceSpaceTypeFromString(mConfig->getString(TXR_NS::appSpace));
    const auto find = mContext.referenceSpaceMap.find(appSpaceType);
    if (find == mContext.referenceSpaceMap.end()) {
      return false;
    }
    mContext.appSpace = find->second;

    return true;
  }

  bool createReferenceSpace() {
    if (!logReferenceSpaces()) {
      return false;
    }

    if (!createReferenceSpaceImpl()) {
      return false;
    }

    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Swapchain
  ////////////////////////////////////////////////////////////////////////////////

  bool logEnvironmentBlendMode(XrViewConfigurationType viewConfigurationType) {
    assert(mContext.instance != XR_NULL_HANDLE);
    assert(mContext.systemId != XR_NULL_SYSTEM_ID);

    std::vector<XrEnvironmentBlendMode> environmentBlendModes;
    if (!TWO_CALL(std::bind(xrEnumerateEnvironmentBlendModes, mContext.instance,
                            mContext.systemId, viewConfigurationType,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3),
                  environmentBlendModes)) {
      return false;
    }

    for (const auto environmentBlendMode : environmentBlendModes) {
      std::cout << "  Environment Blend Mode: "
                << XrEnvironmentBlendModeToString(environmentBlendMode)
                << (environmentBlendMode == mContext.environmentBlendMode
                        ? " (Selected)"
                        : "")
                << std::endl;
    }

    return true;
  }

  bool logViewConfigurations() {
    assert(mContext.instance != XR_NULL_HANDLE);
    assert(mContext.systemId != XR_NULL_SYSTEM_ID);

    std::vector<XrViewConfigurationType> viewConfigurationTypes;
    if (!TWO_CALL(std::bind(xrEnumerateViewConfigurations, mContext.instance,
                            mContext.systemId, std::placeholders::_1,
                            std::placeholders::_2, std::placeholders::_3),
                  viewConfigurationTypes)) {
      return false;
    }

    std::cout << "Available View Configuration Types: ("
              << viewConfigurationTypes.size() << ")" << std::endl;
    for (const auto viewConfigurationType : viewConfigurationTypes) {
      std::cout << "  View Configuration Type: "
                << XrViewConfigurationTypeToString(viewConfigurationType)
                << (viewConfigurationType == mContext.viewConfigurationType
                        ? " (Selected)"
                        : "")
                << std::endl;

      auto viewConfigProperties = valid<XrViewConfigurationProperties>();
      if (XR_FAILED(xrGetViewConfigurationProperties(
              mContext.instance, mContext.systemId, viewConfigurationType,
              &viewConfigProperties))) {
        return false;
      }

      std::cout << "  View configuration FovMutable="
                << (viewConfigProperties.fovMutable == XR_TRUE ? "True"
                                                               : "False")
                << std::endl;

      std::vector<XrViewConfigurationView> viewConfigurationViews;
      if (!TWO_CALL(std::bind(xrEnumerateViewConfigurationViews,
                              mContext.instance, mContext.systemId,
                              viewConfigurationType, std::placeholders::_1,
                              std::placeholders::_2, std::placeholders::_3),
                    viewConfigurationViews)) {
        return false;
      }

      for (uint32_t i = 0; i < viewConfigurationViews.size(); ++i) {
        const auto& viewConfigurationView = viewConfigurationViews[i];
        std::cout << "    View [" << i << "]: Recommended Width="
                  << viewConfigurationView.recommendedImageRectWidth
                  << " Height="
                  << viewConfigurationView.recommendedImageRectHeight
                  << " SampleCount="
                  << viewConfigurationView.recommendedSwapchainSampleCount
                  << std::endl;
        std::cout << "    View [" << i << "]: Maximum     Width="
                  << viewConfigurationView.maxImageRectWidth
                  << " Height=" << viewConfigurationView.maxImageRectHeight
                  << " SampleCount="
                  << viewConfigurationView.maxSwapchainSampleCount << std::endl;
      }

      if (!logEnvironmentBlendMode(viewConfigurationType)) {
        return false;
      }
    }

    return true;
  }

  bool createSwapchainsImpl() {
    std::vector<XrViewConfigurationView> viewConfigurationViews;
    if (!TWO_CALL(
            std::bind(xrEnumerateViewConfigurationViews, mContext.instance,
                      mContext.systemId, mContext.viewConfigurationType,
                      std::placeholders::_1, std::placeholders::_2,
                      std::placeholders::_3),
            viewConfigurationViews)) {
      return false;
    }
    mContext.views.resize(viewConfigurationViews.size(), valid<XrView>());

    std::vector<int64_t> swapchainFormats;
    if (!TWO_CALL(std::bind(xrEnumerateSwapchainFormats, mContext.session,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3),
                  swapchainFormats)) {
      return false;
    }
    mContext.swapchainFormat =
        mRenderer->selectSwapchainFormat(swapchainFormats);

    for (int i = 0; i < viewConfigurationViews.size(); ++i) {
      const auto& viewConfigurationView = viewConfigurationViews[i];

      auto swapchainCreateInfo = valid<XrSwapchainCreateInfo>();
      swapchainCreateInfo.arraySize = 1;
      swapchainCreateInfo.format = mContext.swapchainFormat;
      swapchainCreateInfo.width =
          viewConfigurationView.recommendedImageRectWidth;
      swapchainCreateInfo.height =
          viewConfigurationView.recommendedImageRectHeight;
      swapchainCreateInfo.mipCount = 1;
      swapchainCreateInfo.faceCount = 1;
      swapchainCreateInfo.sampleCount =
          viewConfigurationView.recommendedSwapchainSampleCount;
      swapchainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT |
                                       XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;

      XrSwapchain xrSwapchain = XR_NULL_HANDLE;
      if (XR_FAILED(xrCreateSwapchain(mContext.session, &swapchainCreateInfo,
                                      &xrSwapchain))) {
        continue;
      }

      uint32_t imageCount;
      if (XR_FAILED(xrEnumerateSwapchainImages(xrSwapchain, 0, &imageCount,
                                               nullptr))) {
        continue;
      }
      std::vector<XrSwapchainImage> swapchainImages(imageCount,
                                                    valid<XrSwapchainImage>());
      if (XR_FAILED(xrEnumerateSwapchainImages(
              xrSwapchain, imageCount, &imageCount,
              reinterpret_cast<XrSwapchainImageBaseHeader*>(
                  swapchainImages.data())))) {
        continue;
      }

      Swapchain swapchain;
      swapchain.handle = xrSwapchain;
      swapchain.width = swapchainCreateInfo.width;
      swapchain.height = swapchainCreateInfo.height;
      swapchain.images = std::move(swapchainImages);
      mContext.swapchains.emplace_back(swapchain);
    }

    return true;
  }

  bool createSwapchains() {
    if (!logViewConfigurations()) {
      return false;
    }

    if (!createSwapchainsImpl()) {
      return false;
    }

    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Event
  ////////////////////////////////////////////////////////////////////////////////

  const XrEventDataBuffer* pollEvent() {
    mContext.eventDataBuffer = valid<XrEventDataBuffer>();
    return xrPollEvent(mContext.instance, &mContext.eventDataBuffer) ==
                   XR_SUCCESS
               ? &mContext.eventDataBuffer
               : nullptr;
  }

  bool pollEvents() {
    while (const auto* event = pollEvent()) {
      switch (event->type) {
      case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
        auto sessionStateChangedEvent =
            *reinterpret_cast<const XrEventDataSessionStateChanged*>(event);
        return handleSessionStateChangedEvent(sessionStateChangedEvent);
      }
      }
    }

    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Render
  ////////////////////////////////////////////////////////////////////////////////

  bool renderLayer(
      XrTime predictedDisplayTime, XrCompositionLayerProjection& layer,
      std::vector<XrCompositionLayerProjectionView>& layerProjectionViews) {
    auto viewLocateInfo = valid<XrViewLocateInfo>();
    viewLocateInfo.viewConfigurationType = mContext.viewConfigurationType;
    viewLocateInfo.displayTime = predictedDisplayTime;
    viewLocateInfo.space = mContext.appSpace;

    auto viewState = valid<XrViewState>();

    uint32_t viewCapacityInput = mContext.views.size();
    uint32_t viewCountOutput;

    if (XR_FAILED(xrLocateViews(mContext.session, &viewLocateInfo, &viewState,
                                viewCapacityInput, &viewCountOutput,
                                mContext.views.data()))) {
      return false;
    }

    if (viewCapacityInput != viewCountOutput ||
        mContext.swapchains.size() != viewCountOutput) {
      return false;
    }

    layerProjectionViews.resize(viewCountOutput,
                                valid<XrCompositionLayerProjectionView>());

    for (int i = 0; i < viewCountOutput; ++i) {
      const auto& swapchain = mContext.swapchains[i];

      auto acquireInfo = valid<XrSwapchainImageAcquireInfo>();

      uint32_t swapchainImageIndex;
      if (XR_FAILED(xrAcquireSwapchainImage(swapchain.handle, &acquireInfo,
                                            &swapchainImageIndex))) {
        break;
      }

      auto waitInfo = valid<XrSwapchainImageWaitInfo>();
      waitInfo.timeout = XR_INFINITE_DURATION;
      if (XR_FAILED(xrWaitSwapchainImage(swapchain.handle, &waitInfo))) {
        break;
      }

      auto& layerProjectionView = layerProjectionViews[i];
      layerProjectionView.pose = mContext.views[i].pose;
      layerProjectionView.fov = mContext.views[i].fov;
      layerProjectionView.subImage.swapchain = swapchain.handle;
      layerProjectionView.subImage.imageRect.offset = {0, 0};
      layerProjectionView.subImage.imageRect.extent = {swapchain.width,
                                                       swapchain.height};

      RenderContext renderContext;
      renderContext.viewport = {0, 0, swapchain.width, swapchain.height};
      renderContext.projection = createProjection(
          layerProjectionView.fov, mContext.nearZ, mContext.farZ);
      renderContext.view = createView(layerProjectionView.pose);

      const auto image = swapchain.images[swapchainImageIndex];
      mRenderer->render(renderContext, image.image);

      const auto releaseInfo = valid<XrSwapchainImageReleaseInfo>();
      if (XR_FAILED(xrReleaseSwapchainImage(swapchain.handle, &releaseInfo))) {
        break;
      }
    }

    layer.space = mContext.appSpace;
    layer.layerFlags =
        mContext.environmentBlendMode == XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND
            ? XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT |
                  XR_COMPOSITION_LAYER_UNPREMULTIPLIED_ALPHA_BIT
            : 0;
    layer.viewCount = layerProjectionViews.size();
    layer.views = layerProjectionViews.data();

    return true;
  }

  bool renderFrame() {
    auto frameWaitInfo = valid<XrFrameWaitInfo>();
    auto frameState = valid<XrFrameState>();
    if (XR_FAILED(xrWaitFrame(mContext.session, &frameWaitInfo, &frameState))) {
      return false;
    }

    auto frameBeginInfo = valid<XrFrameBeginInfo>();
    if (XR_FAILED(xrBeginFrame(mContext.session, &frameBeginInfo))) {
      return false;
    }

    auto layer = valid<XrCompositionLayerProjection>();
    std::vector<XrCompositionLayerProjectionView> layerProjectionViews;
    if (frameState.shouldRender == XR_TRUE) {
      if (!renderLayer(frameState.predictedDisplayTime, layer,
                       layerProjectionViews)) {
        return false;
      }
    }

    std::vector<XrCompositionLayerBaseHeader*> layers = {
        reinterpret_cast<XrCompositionLayerBaseHeader*>(&layer)};

    auto frameEndInfo = valid<XrFrameEndInfo>();
    frameEndInfo.displayTime = frameState.predictedDisplayTime;
    frameEndInfo.environmentBlendMode = mContext.environmentBlendMode;
    frameEndInfo.layerCount = layers.size();
    frameEndInfo.layers = layers.data();
    if (XR_FAILED(xrEndFrame(mContext.session, &frameEndInfo))) {
      return false;
    }

    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// exec
  ////////////////////////////////////////////////////////////////////////////////

  int exec() {
    while (true) {
      if (!pollEvents()) {
        return EXIT_FAILURE;
      }

      if (mContext.sessionRunning) {
        if (!pollActions()) {
          return EXIT_FAILURE;
        }

        if (!renderFrame()) {
          return EXIT_FAILURE;
        }
      }
    }

    return EXIT_SUCCESS;
  }

private:
  IConfigXrPtr mConfig;
  IRendererPtr mRenderer;
  ExtensionsInfoPtr mExtensionsInfo;
  ContextXr mContext;
  ExtensionsFunctionPtr mExtensionsFunction;
};

////////////////////////////////////////////////////////////////////////////////
/// ManagerXrFactoryImpl
////////////////////////////////////////////////////////////////////////////////

IManagerXrPtr ManagerXrFactoryImpl::create(
    IConfigXrPtr&& confing) /* override */ {
  auto manager = std::make_unique<ManagerXrImpl>(std::move(confing));
  return manager->init() ? std::move(manager) : nullptr;
}

TINYXR_NAMESPACE_CLOSE
