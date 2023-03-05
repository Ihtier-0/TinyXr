#include "manager.h"

#include <vector>

#include "extensions.h"
#include "logger.h"
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

  if (!initializeDebug()) {
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
    LOG_INFO(indentStr + "Available Extensions: (" +
             std::to_string(extensions.size()) + ")");
    for (const XrExtensionProperties &extension : extensions) {
      LOG_INFO(indentStr + "  Name=" + std::string(extension.extensionName) +
               " SpecVersion=" + XrVersionToString(extension.extensionVersion));
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

    LOG_INFO("Available Layers: " + std::to_string(layers.size()));
    for (const XrApiLayerProperties &layer : layers) {
      LOG_INFO("  Name=" + std::string(layer.layerName) +
               " SpecVersion=" + XrVersionToString(layer.specVersion) +
               " LayerVersion=" + XrVersionToString(layer.layerVersion) +
               " Description=" + std::string(layer.description));
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
              std::min<std::size_t>(std::strlen(TINYXR_STRING),
                                    XR_MAX_ENGINE_NAME_SIZE));
  createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

  const auto applicationName =
      mConfig.getValue<std::string>("xr.ApplicationInfo.applicationName");
  const auto applicationVersion = XrVersionFromString(
      mConfig.getValue<std::string>("xr.ApplicationInfo.applicationVersion"));

  std::memcpy(createInfo.applicationInfo.applicationName,
              applicationName.c_str(),
              std::min<std::size_t>(applicationName.size(),
                                    XR_MAX_APPLICATION_NAME_SIZE));

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

  return CHECK_XR(xrCreateInstance(&createInfo, &mContext.instance));
}

void Manager::logInstanceInfo() {
  auto instanceProperties = valid<XrInstanceProperties>();

  if (CHECK_XR(
          xrGetInstanceProperties(mContext.instance, &instanceProperties))) {
    return;
  }

  LOG_INFO("Instance RuntimeName=" +
           std::string(instanceProperties.runtimeName) + " RuntimeVersion=" +
           XrVersionToString(instanceProperties.runtimeVersion));
}

bool Manager::createInstance() {
  if (mContext.instance != XR_NULL_HANDLE) {
    return false;
  }

  logLayersAndExtensions();

  const std::vector<std::string> userRequestExtensions =
      mConfig.getArray<std::string>("xr.userRequestExtensions");
  mExtensionsInfo = std::make_shared<ExtensionsInfo>(userRequestExtensions);

  if (!createInstanceInternal()) {
    return false;
  }

  mExtensionsFunction = std::make_shared<ExtensionsFunction>(mContext);

  logInstanceInfo();

  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// Debug
////////////////////////////////////////////////////////////////////////////////

bool Manager::initializeDebug() {
#ifdef DEBUG
  auto debugInfo = valid<XrDebugUtilsMessengerCreateInfoEXT>();
  debugInfo.messageTypes = XR_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           XR_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           XR_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                           XR_DEBUG_UTILS_MESSAGE_TYPE_CONFORMANCE_BIT_EXT;
  debugInfo.messageSeverities =
      XR_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      XR_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
      XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debugInfo.userCallback = [](XrDebugUtilsMessageSeverityFlagsEXT severity,
                              XrDebugUtilsMessageTypeFlagsEXT,
                              const XrDebugUtilsMessengerCallbackDataEXT *msg,
                              void *) {
    // Print the debug message we got! There's a bunch more info we could
    // add here too, but this is a pretty good start, and you can always
    // add a breakpoint this line!
    Logger::Level level = Logger::Level::Debug;
    if (severity & XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
      level = Logger::Level::Error;
    } else if (severity & XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      level = Logger::Level::Warning;
    } else if (severity & XR_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
      level = Logger::Level::Info;
    }

    const auto message = std::string("XrDebugUtilsMessengerEXT ") +
                         std::string(msg->functionName) + ' ' +
                         std::string(msg->message);

    switch (level) {
    case Logger::Level::Trace: {
      LOG_TRACE(message);
      break;
    }
    case Logger::Level::Debug: {
      LOG_DEBUG(message);
      break;
    }
    case Logger::Level::Info: {
      LOG_INFO(message);
      break;
    }
    case Logger::Level::Warning: {
      LOG_WARNING(message);
      break;
    }
    case Logger::Level::Error: {
      LOG_ERROR(message);
      break;
    }
    case Logger::Level::Fatal: {
      LOG_FATAL(message);
      break;
    }
    }

    // Returning XR_TRUE here will force the calling function to fail
    return static_cast<XrBool32>(XR_FALSE);
  };

  if (mExtensionsInfo->EXT_debug_utils) {
    return CHECK_XR(mExtensionsFunction->xrCreateDebugUtilsMessengerEXT(
        mContext.instance, &debugInfo, &mContext.debug));
  }
#endif

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
      mConfig.getValue<std::string>("xr.System.formFactor"));

  if (mFormFactor == XR_MAX_ENUM) {
    return false;
  }

  auto systemInfo = valid<XrSystemGetInfo>();
  systemInfo.formFactor = mFormFactor;

  if (!CHECK_XR(
          xrGetSystem(mContext.instance, &systemInfo, &mContext.system))) {
    return false;
  }

  LOG_INFO("Using system " + std::to_string(mContext.system) +
           " for form factor " + XrFormFactorToString(mFormFactor));

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
