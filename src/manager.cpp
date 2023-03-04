#include "manager.h"

#include <vector>

#include "extensions.h"
#include "openxr.h"
#include "tinyxr.h"
#include "utils.h"

////////////////////////////////////////////////////////////////////////////////
/// Manager
////////////////////////////////////////////////////////////////////////////////

Manager::Manager(const Config &config) : mConfig(config) {}

bool Manager::init() {
  if (!createInstance()) {
    return false;
  }

  if (!initializeSystem()) {
    return false;
  }

  mBlendMode = getPreferredBlendMode();

  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// Instance
////////////////////////////////////////////////////////////////////////////////

void Manager::logLayersAndExtensions() {
  // Write out extension properties for a given layer.
  const auto logExtensions = [](const char *layerName, int indent = 0) {
    std::vector<XrExtensionProperties> extensions;

    if (!TWO_CALL(std::bind(xrEnumerateInstanceExtensionProperties, layerName,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3),
                  extensions)) {
      return;
    }

    const std::string indentStr(static_cast<std::string::size_type>(indent),
                                ' ');
    std::cout << indentStr << "Available Extensions: (" << extensions.size()
              << ")" << std::endl;
    for (const XrExtensionProperties &extension : extensions) {
      std::cout << indentStr << "  Name=" << extension.extensionName
                << " SpecVersion=" << extension.extensionVersion << std::endl;
    }
  };

  // Log non-layer extensions (layerName==nullptr).
  logExtensions(nullptr);

  // Log layers and any of their extensions.
  {
    std::vector<XrApiLayerProperties> layers;

    if (!TWO_CALL(xrEnumerateApiLayerProperties, layers)) {
      return;
    }

    std::cout << "Available Layers: " << layers.size() << std::endl;
    for (const XrApiLayerProperties &layer : layers) {
      std::cout << "  Name=" << layer.layerName
                << " SpecVersion=" << XrVersionToString(layer.specVersion)
                << " LayerVersion=" << XrVersionToString(layer.layerVersion)
                << " Description=" << layer.description << std::endl;
      logExtensions(layer.layerName, 4);
    }
  }
}

bool Manager::createInstanceInternal() {
  if (!mExtensionsInfo) {
    return false;
  }

  XrInstanceCreateInfo createInfo = valid<XrInstanceCreateInfo>();
  // applicationInfo
  createInfo.applicationInfo.engineVersion =
      static_cast<uint32_t>(XR_MAKE_VERSION(
          TINYXR_VERSION_MAJOR, TINYXR_VERSION_MINOR, TINYXR_VERSION_PATCH));
  std::memcpy(createInfo.applicationInfo.engineName, TINYXR_STRING,
              std::strlen(TINYXR_STRING));
  createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

  const auto applicationName =
      mConfig.getValue<std::string>("ApplicationInfo.applicationName");
  const auto applicationVersion = XrVersionFromString(
      mConfig.getValue<std::string>("ApplicationInfo.applicationVersion"));

  if (applicationName.size() <= XR_MAX_APPLICATION_NAME_SIZE) {
    std::memcpy(createInfo.applicationInfo.applicationName,
                applicationName.c_str(), applicationName.size() * sizeof(char));
  }
  createInfo.applicationInfo.applicationVersion =
      static_cast<uint32_t>(applicationVersion);

  // It may seem stupid, but it gets the job done. ¯\_(ツ)_/¯
  std::vector<const char *> extensionsCstr;
  extensionsCstr.reserve(mExtensionsInfo->extensions.size());

  for (const auto &extension : mExtensionsInfo->extensions) {
    extensionsCstr.push_back(extension.c_str());
  }

  createInfo.enabledExtensionNames = extensionsCstr.data();
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(mExtensionsInfo->extensions.size());

  xrCreateInstance(&createInfo, &mContext.instance);

  return true;
}

void Manager::logInstanceInfo() {
  auto instanceProperties = valid<XrInstanceProperties>();
  xrGetInstanceProperties(mContext.instance, &instanceProperties);

  std::cout << "Instance RuntimeName=" << instanceProperties.runtimeName
            << " RuntimeVersion="
            << XrVersionToString(instanceProperties.runtimeVersion)
            << std::endl;
}

bool Manager::createInstance() {
  if (mContext.instance != XR_NULL_HANDLE) {
    return false;
  }

  logLayersAndExtensions();

  const std::vector<std::string> userRequestExtensions =
      mConfig.getArray<std::string>("userRequestExtensions");
  mExtensionsInfo = std::make_shared<ExtensionsInfo>(userRequestExtensions);

  if (!createInstanceInternal()) {
    return false;
  }

  mExtensionsFunction = std::make_shared<ExtensionsFunction>(mContext);

  logInstanceInfo();

  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// System
////////////////////////////////////////////////////////////////////////////////

bool Manager::initializeSystem() {
  if (mContext.instance == XR_NULL_HANDLE ||
      mContext.system != XR_NULL_SYSTEM_ID) {
    return false;
  }

  mFormFactor = XrFormFactorFromString(
      mConfig.getValue<std::string>("System.formFactor"));

  if (mFormFactor == XR_MAX_ENUM) {
    return false;
  }

  auto systemInfo = valid<XrSystemGetInfo>();
  systemInfo.formFactor = mFormFactor;
  xrGetSystem(mContext.instance, &systemInfo, &mContext.system);

  std::cout << "Using system " << mContext.system << " for form factor "
            << XrFormFactorToString(mFormFactor) << std::endl;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// BlendMode
////////////////////////////////////////////////////////////////////////////////

XrEnvironmentBlendMode Manager::getPreferredBlendMode() const {
  std::vector<XrEnvironmentBlendMode> blendModes;

  TWO_CALL(std::bind(xrEnumerateEnvironmentBlendModes, mContext.instance,
                     mContext.system, mViewType, std::placeholders::_1,
                     std::placeholders::_2, std::placeholders::_3),
           blendModes);

  for (const auto &blendMode : blendModes) {
    if (mAcceptableBlendModes.count(blendMode)) {
      return blendMode;
    }
  }

  return XrEnvironmentBlendMode::XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM;
}
