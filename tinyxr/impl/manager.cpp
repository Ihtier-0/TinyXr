#include "tinyxr/impl/manager.h"

#include <algorithm>
#include <iomanip>
#include <string>

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/openxr_utils.h"

// TODO! support all graphics API
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

#include <json/json.h>

TINYXR_NAMESPACE_OPEN

////////////////////////////////////////////////////////////////////////////////
/// ManagerXRImpl
////////////////////////////////////////////////////////////////////////////////

ManagerXRImpl::ManagerXRImpl(const Config &confing)
    : mConfig(confing), mExtensionsInfo(mConfig.getVector<std::string>(
                            "xr.userRequestExtensions")) {}

bool ManagerXRImpl::init() {
  if (!mConfig.isValid()) {
    std::cout << "Invalid config" << std::endl;
    return false;
  }

  mContext.formFactor = XrFormFactorFromString(
      mConfig.getValue<std::string>("xr.system.formFactor"));
  if (mContext.formFactor == XR_FORM_FACTOR_MAX_ENUM) {
    std::cout << "Invalid xr.system.formFactor" << std::endl;
    return false;
  }

  mContext.environmentBlendMode = XrEnvironmentBlendModeFromString(
      mConfig.getValue<std::string>("xr.userRequestEnvironmentBlendMode"));
  if (mContext.environmentBlendMode == XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM) {
    std::cout << "Invalid xr.userRequestEnvironmentBlendMode" << std::endl;
    return false;
  }

  mContext.viewConfigurationType = XrViewConfigurationTypeFromString(
      mConfig.getValue<std::string>("xr.viewConfigType"));
  if (mContext.viewConfigurationType == XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM) {
    std::cout << "Invalid xr.viewConfigType" << std::endl;
    return false;
  }

  if (!createInstance()) {
    std::cout << "Unable to create XrInstance" << std::endl;
    return false;
  }

  mExtensionsFunction = std::make_unique<ExtensionsFunction>(mContext.instance);

  if (!getSystem()) {
    std::cout << "Unable to get XrSystem" << std::endl;
    return false;
  }

  if (!createSession()) {
    std::cout << "Unable to create XrSession" << std::endl;
    return false;
  }

  if (!createActions()) {
    std::cout << "Unable to create XrActions" << std::endl;
    return false;
  }

  if (!createReferenceSpaces()) {
    std::cout << "Unable to create XrSpaces" << std::endl;
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// Instance
////////////////////////////////////////////////////////////////////////////////

bool ManagerXRImpl::logInstanceInfo() {
  if (mContext.instance == XR_NULL_HANDLE) {
    return false;
  }

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

bool ManagerXRImpl::createInstanceImpl() {
  if (mContext.instance != XR_NULL_HANDLE) {
    return false;
  }

  std::vector<const char *> extensions;
  extensions.reserve(mExtensionsInfo.extensions.size());
  for (const auto &extension : mExtensionsInfo.extensions) {
    extensions.push_back(extension.c_str());
  }

  auto createInfo = valid<XrInstanceCreateInfo>();
  createInfo.enabledExtensionCount = (uint32_t)extensions.size();
  createInfo.enabledExtensionNames = extensions.data();

  // engine
  createInfo.applicationInfo.engineVersion =
      static_cast<uint32_t>(XR_MAKE_VERSION(
          TINYXR_VERSION_MAJOR, TINYXR_VERSION_MINOR, TINYXR_VERSION_PATCH));
  std::memcpy(createInfo.applicationInfo.engineName, TINYXR_STRING,
              std::min<std::size_t>(std::strlen(TINYXR_STRING),
                                    XR_MAX_ENGINE_NAME_SIZE));
  createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

  // application
  const auto applicationName =
      mConfig.getValue<std::string>("xr.applicationInfo.applicationName");
  const auto applicationVersion = XrVersionFromString(
      mConfig.getValue<std::string>("xr.applicationInfo.applicationVersion"));

  std::memcpy(createInfo.applicationInfo.applicationName,
              applicationName.c_str(),
              std::min<std::size_t>(applicationName.size(),
                                    XR_MAX_APPLICATION_NAME_SIZE));

  createInfo.applicationInfo.applicationVersion =
      static_cast<uint32_t>(applicationVersion);

  return XR_SUCCEEDED(xrCreateInstance(&createInfo, &mContext.instance));
}

bool ManagerXRImpl::logLayersAndExtensions() {
  const auto logExtensions = [](const char *layerName, int indent) {
    std::vector<XrExtensionProperties> extensions;
    if (!TWO_CALL(std::bind(xrEnumerateInstanceExtensionProperties, layerName,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3),
                  extensions)) {
      return false;
    }

    if (extensions.empty()) {
      return true;
    }

    const auto max_element = std::max_element(
        extensions.begin(), extensions.end(),
        [](const XrExtensionProperties &l, const XrExtensionProperties &r) {
          return std::strlen(l.extensionName) < std::strlen(r.extensionName);
        });
    const auto max_len = std::strlen(max_element->extensionName);

    const std::string indentStr(indent, ' ');
    std::cout << indentStr << "Available Extensions: ("
              << std::to_string(extensions.size()) << ")" << std::endl;
    for (const XrExtensionProperties &extension : extensions) {
      std::cout << indentStr << "  Name " << std::setw(max_len)
                << extension.extensionName << " SpecVersion "
                << XrVersionToString(extension.extensionVersion) << std::endl;
    }

    return true;
  };

  if (!logExtensions(nullptr, 0)) {
    return false;
  }

  std::vector<XrApiLayerProperties> layers;
  if (!TWO_CALL(xrEnumerateApiLayerProperties, layers)) {
    return false;
  }

  std::cout << "Available Layers: (" << layers.size() << ')' << std::endl;
  for (const XrApiLayerProperties &layer : layers) {
    std::cout << "  Name=" << layer.layerName
              << " SpecVersion=" << XrVersionToString(layer.specVersion)
              << " LayerVersion=" << XrVersionToString(layer.layerVersion)
              << " Description=" << layer.description;
    if (!logExtensions(layer.layerName, 4)) {
      return false;
    }
  }

  return true;
}

bool ManagerXRImpl::createInstance() {
  if (mContext.instance != XR_NULL_HANDLE) {
    return false;
  }

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

bool ManagerXRImpl::getSystem() {
  if (mContext.instance == XR_NULL_HANDLE ||
      mContext.systemId != XR_NULL_SYSTEM_ID) {
    return false;
  }

  auto systemInfo = valid<XrSystemGetInfo>();
  systemInfo.formFactor = mContext.formFactor;

  if (XR_FAILED(
          xrGetSystem(mContext.instance, &systemInfo, &mContext.systemId))) {
    return false;
  }

  std::cout << "Using system " << mContext.systemId << " for form factor "
            << mContext.formFactor << std::endl;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// Session
////////////////////////////////////////////////////////////////////////////////

bool ManagerXRImpl::createSessionImpl() {
  auto binding = valid<XrGraphicsBinding>();
  binding.hDC = GetDC(glfwGetWin32Window(mGraphicsContext));
  binding.hGLRC = glfwGetWGLContext(mGraphicsContext);

  auto createInfo = valid<XrSessionCreateInfo>();
  createInfo.next = &binding;
  createInfo.systemId = mContext.systemId;
  return XR_SUCCEEDED(
      xrCreateSession(mContext.instance, &createInfo, &mContext.session));
}

bool ManagerXRImpl::initGraphicsApi(
    const XrGraphicsRequirements &requirements) {
  // TODO! support all graphics API
  const auto major = XR_VERSION_MAJOR(requirements.minApiVersionSupported);
  const auto minor = XR_VERSION_MINOR(requirements.minApiVersionSupported);

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Offscreen contexts
  mGraphicsContext = glfwCreateWindow(10, 10, "TinyXrGlContext", NULL, NULL);
  glfwMakeContextCurrent(mGraphicsContext);

  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return false;
  }

  return true;
}

bool ManagerXRImpl::initializeGraphicsDevice() {
  if (!mExtensionsInfo.graphicExtension ||
      mExtensionsFunction->xrGetGraphicsRequirementsKHR == nullptr) {
    return false;
  }

  auto requirements = valid<XrGraphicsRequirements>();

  if (XR_FAILED(mExtensionsFunction->xrGetGraphicsRequirementsKHR(
          mContext.instance, mContext.systemId, &requirements))) {
    return false;
  }

  if (!initGraphicsApi(requirements)) {
    return false;
  }

  // TODO! support all graphics API
  GLint major = 0;
  GLint minor = 0;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  const auto currentVersion = XR_MAKE_VERSION(major, minor, 0);
  if (requirements.minApiVersionSupported > currentVersion) {
    std::cout << "The current version of OpenGL is lower than what the OpenXR "
                 "runtime requires."
              << std::endl;
    return false;
  }

  return true;
}

bool ManagerXRImpl::logEnvironmentBlendMode(
    const XrViewConfigurationType type) {
  if (mContext.instance == XR_NULL_HANDLE ||
      mContext.systemId == XR_NULL_SYSTEM_ID) {
    return false;
  }

  std::vector<XrEnvironmentBlendMode> modes;
  if (!TWO_CALL(std::bind(xrEnumerateEnvironmentBlendModes, mContext.instance,
                          mContext.systemId, type, std::placeholders::_1,
                          std::placeholders::_2, std::placeholders::_3),
                modes)) {
    return false;
  }

  for (const auto mode : modes) {
    std::cout << "Environment Blend Mode: "
              << XrEnvironmentBlendModeToString(mode)
              << (mode == mContext.environmentBlendMode ? " (Selected)" : "")
              << std::endl;
  }

  return true;
}

bool ManagerXRImpl::logViewConfigurations() {
  if (mContext.instance == XR_NULL_HANDLE ||
      mContext.systemId == XR_NULL_SYSTEM_ID) {
    return false;
  }

  std::vector<XrViewConfigurationType> types;
  if (!TWO_CALL(std::bind(xrEnumerateViewConfigurations, mContext.instance,
                          mContext.systemId, std::placeholders::_1,
                          std::placeholders::_2, std::placeholders::_3),
                types)) {
    return false;
  }

  std::cout << "Available View Configuration Types: (" << types.size() << ")"
            << std::endl;
  for (const auto &type : types) {
    std::cout << "  View Configuration Type: "
              << XrViewConfigurationTypeToString(type)
              << (type == mContext.viewConfigurationType ? " (Selected)" : "")
              << std::endl;

    auto properties = valid<XrViewConfigurationProperties>();
    if (XR_FAILED(xrGetViewConfigurationProperties(
            mContext.instance, mContext.systemId, type, &properties))) {
      continue;
    }

    std::cout << "  View configuration FovMutable="
              << (properties.fovMutable == XR_TRUE ? "True" : "False")
              << std::endl;

    std::vector<XrViewConfigurationView> views;
    if (!TWO_CALL(std::bind(xrEnumerateViewConfigurationViews,
                            mContext.instance, mContext.systemId, type,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3),
                  views)) {
      continue;
    }

    for (uint32_t i = 0; i < views.size(); i++) {
      const XrViewConfigurationView &view = views[i];

      std::cout << "    View [" << i
                << "]: Recommended Width=" << view.recommendedImageRectWidth
                << " Height=" << view.recommendedImageRectHeight
                << " SampleCount=" << view.recommendedSwapchainSampleCount
                << std::endl;
      std::cout << "    View [" << i
                << "]: Maximum     Width=" << view.maxImageRectWidth
                << " Height=" << view.maxImageRectHeight
                << " SampleCount=" << view.maxSwapchainSampleCount << std::endl;
    }

    if (!logEnvironmentBlendMode(type)) {
      continue;
    }
  }

  return true;
}

bool ManagerXRImpl::createSession() {
  if (mContext.instance == XR_NULL_HANDLE ||
      mContext.systemId == XR_NULL_SYSTEM_ID) {
    return false;
  }

  if (!logViewConfigurations()) {
    return false;
  }

  if (!initializeGraphicsDevice()) {
    return false;
  }

  if (!createSessionImpl()) {
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// Actions
////////////////////////////////////////////////////////////////////////////////

bool ManagerXRImpl::createActions() {
  const auto actionSetsConfig =
      mConfig.getValue<std::string>("xr.actionSetsConfig");

  std::ifstream actionSetsFile(actionSetsConfig);
  if (actionSetsFile.bad()) {
    return false;
  }

  Json::Value actionSetsRoot;

  Json::CharReaderBuilder builder;
  Json::String errs;

  if (!parseFromStream(builder, actionSetsFile, &actionSetsRoot, &errs)) {
    std::cout << "xr.actionSetsConfig parse error: " << errs << std::endl;
    return false;
  }

  if (!actionSetsRoot.isArray() || actionSetsRoot.empty()) {
    return false;
  }

  for (const auto actionSet : actionSetsRoot) {
    auto setInfo = valid<XrActionSetCreateInfo>();

    if (!actionSet.isObject()) {
      continue;
    }

    const auto setName = actionSet["name"];
    const auto setNameStr = setName.asString();
    if (setNameStr.empty()) {
      continue;
    }

    const auto localisedName = actionSet["localisedName"];
    const auto localisedNameStr = localisedName.asString();
    if (localisedNameStr.empty()) {
      continue;
    }

    std::memcpy(setInfo.actionSetName, setNameStr.c_str(),
                std::min<std::size_t>(setNameStr.size() + 1,
                                      XR_MAX_ACTION_SET_NAME_SIZE));
    std::memcpy(setInfo.localizedActionSetName, localisedNameStr.c_str(),
                std::min<std::size_t>(localisedNameStr.size() + 1,
                                      XR_MAX_ACTION_SET_NAME_SIZE));

    XrActionSet set = XR_NULL_HANDLE;
    if (XR_FAILED(xrCreateActionSet(mContext.instance, &setInfo, &set))) {
      continue;
    }

    mContext.actionSets[setNameStr].set = set;

    const auto actions = actionSet["actions"];
    if (!actions.isArray() || actions.empty()) {
      continue;
    }

    for (const auto action : actions) {
      auto actionInfo = valid<XrActionCreateInfo>();

      const auto type = action["type"];
      const auto typeStr = type.asString();
      actionInfo.actionType = XrActionTypeFromString(typeStr);
      if (actionInfo.actionType == XrActionType::XR_ACTION_TYPE_MAX_ENUM) {
        continue;
      }

      const auto name = action["name"];
      const auto nameStr = name.asString();
      if (nameStr.empty()) {
        continue;
      }

      const auto localisedName = action["localisedName"];
      const auto localisedNameStr = localisedName.asString();
      if (localisedNameStr.empty()) {
        continue;
      }

      std::memcpy(
          actionInfo.actionName, nameStr.c_str(),
          std::min<std::size_t>(nameStr.size(), XR_MAX_ACTION_NAME_SIZE));
      std::memcpy(actionInfo.localizedActionName, localisedNameStr.c_str(),
                  std::min<std::size_t>(localisedNameStr.size(),
                                        XR_MAX_LOCALIZED_ACTION_NAME_SIZE));

      const auto paths = action["paths"];
      if (!paths.isArray() || paths.empty()) {
        continue;
      }

      Action action;
      std::vector<XrPath> subactionPaths;

      for (const auto path : paths) {
        const auto pathStr = path.asString();

        if (mContext.subactionPaths.find(pathStr) ==
            mContext.subactionPaths.end()) {
          XrPath xrPath;
          if (XR_FAILED(xrStringToPath(mContext.instance, pathStr.c_str(),
                                       &xrPath))) {
            continue;
          }

          mContext.subactionPaths[pathStr].path = xrPath;
          mContext.subactionPaths[pathStr].side = SideFromPath(pathStr);
        }

        action.subactionPaths[pathStr] = mContext.subactionPaths[pathStr];
        subactionPaths.push_back(mContext.subactionPaths[pathStr].path);
      }

      actionInfo.countSubactionPaths = subactionPaths.size();
      actionInfo.subactionPaths = subactionPaths.data();

      XrAction xrAction;
      if (XR_FAILED(xrCreateAction(set, &actionInfo, &xrAction))) {
        continue;
      }

      action.action = xrAction;
      mContext.actionSets[setNameStr].actions[nameStr] = action;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// Spaces
////////////////////////////////////////////////////////////////////////////////

bool ManagerXRImpl::createReferenceSpacesImpl() {
  const auto createSpace = [this](const XrReferenceSpaceType type) {
    XrSpace space;

    auto createInfo = valid<XrReferenceSpaceCreateInfo>();
    createInfo.referenceSpaceType = static_cast<XrReferenceSpaceType>(type);

    const auto error =
        xrCreateReferenceSpace(mContext.session, &createInfo, &space);
    if (XR_FAILED(error)) {
      std::cout << "Failed to create reference space "
                << XrReferenceSpaceTypeToString(createInfo.referenceSpaceType)
                << " with error " << XrResultToString(error) << std::endl;
    }

    mContext.referenceSpaces[createInfo.referenceSpaceType] = space;
  };

  const auto begin = static_cast<int>(XR_REFERENCE_SPACE_TYPE_VIEW);
  const auto end = static_cast<int>(XR_REFERENCE_SPACE_TYPE_STAGE);

  for (auto space = begin; space <= end; ++space) {
    createSpace(static_cast<XrReferenceSpaceType>(space));
  }

  return !mContext.referenceSpaces.empty();
}

bool ManagerXRImpl::logReferenceSpaces() {
  if (mContext.session == XR_NULL_HANDLE) {
    return false;
  }

  std::vector<XrReferenceSpaceType> spaces;
  if (!TWO_CALL(std::bind(xrEnumerateReferenceSpaces, mContext.session,
                          std::placeholders::_1, std::placeholders::_2,
                          std::placeholders::_3),
                spaces)) {
    return false;
  }

  std::cout << "Available reference spaces (" << spaces.size() << ')'
            << std::endl;
  for (const auto &space : spaces) {
    std::cout << "  Name: " << XrReferenceSpaceTypeToString(space) << std::endl;
  }

  return true;
}

bool ManagerXRImpl::createReferenceSpaces() {
  if (!logReferenceSpaces()) {
    return false;
  }

  if (!createReferenceSpacesImpl()) {
    return false;
  }

  return true;
}

TINYXR_NAMESPACE_CLOSE
