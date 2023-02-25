#include "manager.h"

#include <vector>

#include "extensions.h"
#include "openxr.h"
#include "utils.h"

bool Manager::init() {
  if (!createInstance()) {
    return false;
  }

  if (!initializeSystem()) {
    return false;
  }

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
                << " SpecVersion=" << toString(layer.specVersion)
                << " LayerVersion=" << toString(layer.layerVersion)
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
  createInfo.next = nullptr;
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(mExtensionsInfo->extensions.size());
  createInfo.enabledExtensionNames = mExtensionsInfo->extensions.data();

  strcpy(createInfo.applicationInfo.applicationName, "TinyXr");
  createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

  xrCreateInstance(&createInfo, &mContext.instance);

  return true;
}

void Manager::logInstanceInfo() {
  auto instanceProperties = valid<XrInstanceProperties>();
  xrGetInstanceProperties(mContext.instance, &instanceProperties);

  std::cout << "Instance RuntimeName=" << instanceProperties.runtimeName
            << " RuntimeVersion=" << toString(instanceProperties.runtimeVersion)
            << std::endl;
}

bool Manager::createInstance() {
  if (mContext.instance != XR_NULL_HANDLE) {
    return false;
  }

  logLayersAndExtensions();

  mExtensionsInfo =
      std::make_shared<ExtensionsInfo>(std::vector<const char *>());

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

  auto systemInfo = valid<XrSystemGetInfo>();
  systemInfo.formFactor = mFormFactor;
  xrGetSystem(mContext.instance, &systemInfo, &mContext.system);

  std::cout << "Using system " << mContext.system << " for form factor "
            << toString(mFormFactor) << std::endl;

  return true;
}
