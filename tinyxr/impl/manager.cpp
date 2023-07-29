#include "tinyxr/impl/manager.h"

#include "tinyxr/impl/openxr_utils.h"

#include <algorithm>
#include <iomanip>

TINYXR_NAMESPACE_OPEN

bool ManagerXRImpl::init() {
  if (!createInstance()) {
    return false;
  }

  return true;
}

bool ManagerXRImpl::logInstanceInfo() { return false; }

bool ManagerXRImpl::createInstanceImpl() { return false; }

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

TINYXR_NAMESPACE_CLOSE
