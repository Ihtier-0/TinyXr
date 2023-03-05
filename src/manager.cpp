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

Manager::~Manager() {
  if (mContext.session != XR_NULL_HANDLE) {
    CHECK_XR(xrDestroySession(mContext.session));
  }

  // XrSystemId does not need to be destroyed
  mContext.system = XR_NULL_SYSTEM_ID;

#ifdef DEBUG
  if (mContext.debug != XR_NULL_HANDLE) {
    CHECK_XR(
        mExtensionsFunction->xrDestroyDebugUtilsMessengerEXT(mContext.debug));
  }
#endif

  if (mContext.instance != XR_NULL_HANDLE) {
    CHECK_XR(xrDestroyInstance(mContext.instance));
  }
}

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

  if (!createSession()) {
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

bool Manager::initializeSystemInternal() {
  mFormFactor = XrFormFactorFromString(
      mConfig.getValue<std::string>("xr.System.formFactor"));

  if (mFormFactor == XR_MAX_ENUM) {
    return false;
  }

  auto systemInfo = valid<XrSystemGetInfo>();
  systemInfo.formFactor = mFormFactor;

  return CHECK_XR(
      xrGetSystem(mContext.instance, &systemInfo, &mContext.system));
}

void Manager::getSystemProperties() {
  auto properties = valid<XrSystemProperties>();
  auto propertiesTracking = valid<XrSystemHandTrackingPropertiesEXT>();
  auto propertiesEyeGaze = valid<XrSystemEyeGazeInteractionPropertiesEXT>();
  properties.next = &propertiesTracking;
  propertiesTracking.next = &propertiesEyeGaze;

  if (!CHECK_XR(xrGetSystemProperties(mContext.instance, mContext.system,
                                      &properties))) {
    return;
  }

  LOG_INFO("Using system " + std::string(properties.systemName) +
           " for form factor " + XrFormFactorToString(mFormFactor));

  mContext.systemProperties.handTracking =
      mExtensionsInfo->EXT_hand_tracking &&
      propertiesTracking.supportsHandTracking;
  mContext.systemProperties.depthLayer =
      mExtensionsInfo->KHR_composition_layer_depth;
  mContext.systemProperties.eyeTracking =
      mExtensionsInfo->EXT_eye_gaze_interaction &&
      propertiesEyeGaze.supportsEyeGazeInteraction;

  if (mContext.systemProperties.handTracking) {
    LOG_INFO("OpenXR hands tracking extension enabled!");
  }
  if (mContext.systemProperties.depthLayer) {
    LOG_INFO("OpenXR depth layer extension enabled!");
  }
  if (mContext.systemProperties.eyeTracking) {
    LOG_INFO("OpenXR eye tracking extension enabled!");
  }
}

bool Manager::initializeSystem() {
  if (mContext.instance == XR_NULL_HANDLE ||
      mContext.system != XR_NULL_SYSTEM_ID) {
    return false;
  }

  if (!initializeSystemInternal()) {
    return false;
  }

  getSystemProperties();

  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// Session
////////////////////////////////////////////////////////////////////////////////

bool Manager::createSession() {
  // only OpenGL now
  auto requirement = valid<XrGraphicsRequirements>();
  if (!CHECK_XR(mExtensionsFunction->xrGetGraphicsRequirementsKHR(
          mContext.instance, mContext.system, &requirement))) {
    return false;
  }

  auto sessionInfo = valid<XrSessionCreateInfo>();

#if defined(XR_USE_PLATFORM_WIN32) && defined(XR_USE_GRAPHICS_API_OPENGL)
  QSurfaceFormat format;
  format.setSamples(4);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setMajorVersion(XR_VERSION_MAJOR(requirement.minApiVersionSupported));
  format.setMinorVersion(XR_VERSION_MINOR(requirement.minApiVersionSupported));
  mSurface.setFormat(format);
  mSurface.create();
  mOpenGLContext.setFormat(format);
  mOpenGLContext.setShareContext(QOpenGLContext::globalShareContext());
  if (!mOpenGLContext.create()) {
    LOG_FATAL("Unable to create OpenGL context");
    return false;
  }

  if (!mOpenGLContext.makeCurrent(&mSurface)) {
    LOG_FATAL("Unable to make current OpenGL context");
    return false;
  }

  LOG_INFO(
      "Use OpenGL " +
      std::to_string(XR_VERSION_MAJOR(requirement.minApiVersionSupported)) +
      "." +
      std::to_string(XR_VERSION_MINOR(requirement.minApiVersionSupported)));

  // not work
  // const auto nativeHandle = mOpenGLContext.nativeHandle();
  // const auto nativeContext = nativeHandle.value<QWGLNativeContext>();

  // TODO! Remove the Qt dependency
  // and
  // use a library that explicitly provides HDC and HGLRC.
  HDC dc = wglGetCurrentDC();
  if (!dc) {
    return false;
  }
  HGLRC glrc = wglGetCurrentContext();
  if (!glrc) {
    return false;
  }

  auto graphicsBinding = valid<XrGraphicsBinding>();
  graphicsBinding.hDC = dc;
  graphicsBinding.hGLRC = glrc;

  sessionInfo.next = &graphicsBinding;
  sessionInfo.systemId = mContext.system;
#else
  LOG_FATAL("TinyXr currently supports only Windows and OpenGL");
  return false;
#endif

  return CHECK_XR(
      xrCreateSession(mContext.instance, &sessionInfo, &mContext.session));
}
