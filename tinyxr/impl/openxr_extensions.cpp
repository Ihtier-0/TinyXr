#include "tinyxr/impl/openxr_extensions.h"

#include "tinyxr/impl/openxr_utils.h"

TINYXR_NAMESPACE_OPEN

ExtensionsInfo::ExtensionsInfo(
    const std::vector<std::string> &userRequestExtensions) {
#define CHECK_EXT(name, available)                                             \
  else if (available && strcmp("XR_" #name, extension.extensionName) == 0) {   \
    name = true;                                                               \
    extensions.push_back("XR_" #name);                                         \
  }

  std::vector<XrExtensionProperties> extensionProperties;
  TWO_CALL(std::bind(xrEnumerateInstanceExtensionProperties, nullptr,
                     std::placeholders::_1, std::placeholders::_2,
                     std::placeholders::_3),
           extensionProperties);

  for (const auto &extension : extensionProperties) {
    if (strcmp(XR_GFX_EXTENSION, extension.extensionName) == 0) {
      graphicExtension = true;
      extensions.push_back(XR_GFX_EXTENSION);
    } else if (strcmp(XR_TIME_EXTENSION, extension.extensionName) == 0) {
      timeExtension = true;
      extensions.push_back(XR_TIME_EXTENSION);
    }
    FOR_EACH_EXT_ALL(CHECK_EXT)
    FOR_EACH_EXT_UWP(CHECK_EXT)
    FOR_EACH_EXT_ANDROID(CHECK_EXT)
    FOR_EACH_EXT_LINUX(CHECK_EXT)
    FOR_EACH_EXT_DEBUG(CHECK_EXT)
    else {
      for (const auto &userExtension : userRequestExtensions) {
        if (userExtension == extension.extensionName) {
          extensions.push_back(userExtension);
          break;
        }
      }
    }
  }

#undef CHECK_EXT
}

TINYXR_NAMESPACE_CLOSE
