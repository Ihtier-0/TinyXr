#include "tinyxr/impl/managerxr_impl.h"

#include <iomanip>

#include "tinyxr/impl/contextxr.h"
#include "tinyxr/impl/openxr_extensions.h"
#include "tinyxr/impl/openxr_utils.h"

TINYXR_NAMESPACE_OPEN

class ManagerXrImpl : public IManagerXr {
public:
  ManagerXrImpl(ConfigXrPtr &&confing) : mConfig(std::move(confing)) {}
  ~ManagerXrImpl() {}

  bool init() {
    if (!createInstance()) {
      return false;
    }

    return true;
  }

private:
  ////////////////////////////////////////////////////////////////////////////////
  /// Instance
  ////////////////////////////////////////////////////////////////////////////////

  bool logLayersAndExtensions() {
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
        std::cout << indentStr << "  Name=" << std::setw(max_len)
                  << extension.extensionName << " SpecVersion="
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

  bool createInstance() {
    if (!logLayersAndExtensions()) {
      return false;
    }

    if (mContext.instance != XR_NULL_HANDLE) {
      return false;
    }

    mExtensionsInfo = std::make_unique<ExtensionsInfo>(
        mConfig->getStringVector("xr.userRequestExtensions"));

    return true;
  }

private:
  ConfigXrPtr mConfig;

  ContextXr mContext;
  ExtensionsInfoPtr mExtensionsInfo;
};

////////////////////////////////////////////////////////////////////////////////
/// ManagerXrFactoryImpl
////////////////////////////////////////////////////////////////////////////////

ManagerXrPtr ManagerXrFactoryImpl::create(
    ConfigXrPtr &&confing) /* override */ {
  auto manager = std::make_unique<ManagerXrImpl>(std::move(confing));
  return manager->init() ? std::move(manager) : nullptr;
}

TINYXR_NAMESPACE_CLOSE
