#include "tinyxr/impl/manager.h"

#include <algorithm>
#include <iomanip>
#include <string>

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/openxr_utils.h"

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

  if (!createInstance()) {
    std::cout << "Unable to create XrInstance" << std::endl;
    return false;
  }

  if (!getSystem()) {
    std::cout << "Unable to get XrSystem" << std::endl;
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
  if (mContext.instance == XR_NULL_HANDLE) {
    return false;
  }

  const auto formFactor = mConfig.getValue<std::string>("xr.system.FormFactor");

  auto systemInfo = valid<XrSystemGetInfo>();
  systemInfo.formFactor = XrFormFactorFromString(formFactor);

  if (XR_FAILED(
          xrGetSystem(mContext.instance, &systemInfo, &mContext.systemId))) {
    return false;
  }

  std::cout << "Using system " << mContext.systemId << " for form factor "
            << formFactor << std::endl;

  return true;
}

TINYXR_NAMESPACE_CLOSE
