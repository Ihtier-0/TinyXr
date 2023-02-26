#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <memory>
#include <vector>

#include "context.h"
#include "openxr.h"

// clang-format off

// based on:
// https://github.com/StereoKit/StereoKit/blob/master/StereoKitC/systems/platform/openxr_extensions.h
#if defined(XR_OPENXR)

#if defined(XR_USE_GRAPHICS_API_D3D11)
  #define XR_GFX_EXTENSION XR_KHR_D3D11_ENABLE_EXTENSION_NAME
  #define XrSwapchainImage XrSwapchainImageD3D11KHR
  #define XR_TYPE_SWAPCHAIN_IMAGE XR_TYPE_SWAPCHAIN_IMAGE_D3D11_KHR
  #define XrGraphicsRequirements XrGraphicsRequirementsD3D11KHR
  #define XR_TYPE_GRAPHICS_REQUIREMENTS XR_TYPE_GRAPHICS_REQUIREMENTS_D3D11_KHR
  #define xrGetGraphicsRequirementsKHR xrGetD3D11GraphicsRequirementsKHR
  #define PFN_xrGetGraphicsRequirementsKHR PFN_xrGetD3D11GraphicsRequirementsKHR
  #define NAME_xrGetGraphicsRequirementsKHR "xrGetD3D11GraphicsRequirementsKHR"
  #define XrGraphicsBinding XrGraphicsBindingD3D11KHR
  #define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_D3D11_KHR

#elif defined(XR_USE_PLATFORM_WIN32) && defined(XR_USE_GRAPHICS_API_OPENGL)
  #define XR_GFX_EXTENSION XR_KHR_OPENGL_ENABLE_EXTENSION_NAME
  #define XrSwapchainImage XrSwapchainImageOpenGLKHR
  #define XR_TYPE_SWAPCHAIN_IMAGE XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR
  #define XrGraphicsRequirements XrGraphicsRequirementsOpenGLKHR
  #define XR_TYPE_GRAPHICS_REQUIREMENTS XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR
  #define xrGetGraphicsRequirementsKHR xrGetOpenGLGraphicsRequirementsKHR
  #define PFN_xrGetGraphicsRequirementsKHR PFN_xrGetOpenGLGraphicsRequirementsKHR
  #define NAME_xrGetGraphicsRequirementsKHR "xrGetOpenGLGraphicsRequirementsKHR"
  #define XrGraphicsBinding XrGraphicsBindingOpenGLWin32KHR
  #define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR

#elif defined(XR_USE_PLATFORM_XLIB) && defined(XR_USE_GRAPHICS_API_OPENGL)
  #define XR_GFX_EXTENSION XR_KHR_OPENGL_ENABLE_EXTENSION_NAME
  #define XrSwapchainImage XrSwapchainImageOpenGLKHR
  #define XR_TYPE_SWAPCHAIN_IMAGE XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR
  #define XrGraphicsRequirements XrGraphicsRequirementsOpenGLKHR
  #define XR_TYPE_GRAPHICS_REQUIREMENTS XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR
  #define xrGetGraphicsRequirementsKHR xrGetOpenGLGraphicsRequirementsKHR
  #define PFN_xrGetGraphicsRequirementsKHR PFN_xrGetOpenGLGraphicsRequirementsKHR
  #define NAME_xrGetGraphicsRequirementsKHR "xrGetOpenGLGraphicsRequirementsKHR"
  #define XrGraphicsBinding XrGraphicsBindingOpenGLXlibKHR
  #define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_OPENGL_XLIB_KHR

#elif defined(XR_USE_GRAPHICS_API_VULKAN)
  #define XR_GFX_EXTENSION XR_KHR_VULKAN_ENABLE_EXTENSION_NAME
  #define XrSwapchainImage XrSwapchainImageVulkanKHR
  #define XR_TYPE_SWAPCHAIN_IMAGE XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR
  #define XrGraphicsRequirements XrGraphicsRequirementsVulkanKHR
  #define XR_TYPE_GRAPHICS_REQUIREMENTS XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR
  #define PFN_xrGetGraphicsRequirementsKHR PFN_xrGetVulkanGraphicsRequirementsKHR
  #define NAME_xrGetGraphicsRequirementsKHR "xrGetVulkanGraphicsRequirementsKHR"
  #define XrGraphicsBinding XrGraphicsBindingVulkanKHR
  #define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR

#elif defined(XR_USE_GRAPHICS_API_OPENGL_ES)
  #define XR_GFX_EXTENSION XR_KHR_OPENGL_ES_ENABLE_EXTENSION_NAME
  #define XrSwapchainImage XrSwapchainImageOpenGLESKHR
  #define XR_TYPE_SWAPCHAIN_IMAGE XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_ES_KHR
  #define XrGraphicsRequirements XrGraphicsRequirementsOpenGLESKHR
  #define XR_TYPE_GRAPHICS_REQUIREMENTS XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_ES_KHR
  #define xrGetGraphicsRequirementsKHR xrGetOpenGLESGraphicsRequirementsKHR
  #define PFN_xrGetGraphicsRequirementsKHR PFN_xrGetOpenGLESGraphicsRequirementsKHR
  #define NAME_xrGetGraphicsRequirementsKHR "xrGetOpenGLESGraphicsRequirementsKHR"

  #if defined(OS_ANDROID)
    #define XrGraphicsBinding XrGraphicsBindingOpenGLESAndroidKHR
    #define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_OPENGL_ES_ANDROID_KHR

  #elif defined(OS_LINUX)
    #define XrGraphicsBinding XrGraphicsBindingEGLMNDX
    #define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_EGL_MNDX

  #endif

#endif

// clang-format on

////////////////////////////////////////////////////////////////////////////////

#if defined(OS_WINDOWS_UWP)
#define EXT_AVAILABLE_UWP true
#else
#define EXT_AVAILABLE_UWP false
#endif

#if defined(DEBUG)
#define EXT_AVAILABLE_DEBUG true
#else
#define EXT_AVAILABLE_DEBUG false
#endif

#if defined(OS_ANDROID)
#define EXT_AVAILABLE_ANDROID true
#else
#define EXT_AVAILABLE_ANDROID false
#endif

#if defined(OS_LINUX)
#define EXT_AVAILABLE_LINUX true
#else
#define EXT_AVAILABLE_LINUX false
#endif

////////////////////////////////////////////////////////////////////////////////
// Extension availability
////////////////////////////////////////////////////////////////////////////////

// Extensions that are available for all supported platforms
#define FOR_EACH_EXT_ALL(_)                                                    \
  _(KHR_composition_layer_depth, true)                                         \
  _(EXT_hand_tracking, true)                                                   \
  _(EXT_eye_gaze_interaction, true)                                            \
  _(FB_color_space, true)                                                      \
  _(OCULUS_audio_device_guid, true)                                            \
  _(MSFT_unbounded_reference_space, true)                                      \
  _(MSFT_hand_interaction, true)                                               \
  _(MSFT_hand_tracking_mesh, true)                                             \
  _(MSFT_spatial_anchor, true)                                                 \
  _(MSFT_spatial_graph_bridge, true)                                           \
  _(MSFT_secondary_view_configuration, true)                                   \
  _(MSFT_first_person_observer, true)                                          \
  _(MSFT_scene_understanding, true)                                            \
  _(EXT_hp_mixed_reality_controller, true)                                     \
  _(EXTX_overlay, true)

// UWP platform only
#define FOR_EACH_EXT_UWP(_) _(MSFT_perception_anchor_interop, EXT_AVAILABLE_UWP)

// Android platform only
#define FOR_EACH_EXT_ANDROID(_)                                                \
  _(KHR_android_create_instance, EXT_AVAILABLE_ANDROID)

#if defined(LINUX_EGL)
// Linux platform only
#define FOR_EACH_EXT_LINUX(_) _(MNDX_egl_enable, EXT_AVAILABLE_LINUX)
#else
#define FOR_EACH_EXT_LINUX(_)
#endif

// Debug builds only
#define FOR_EACH_EXT_DEBUG(_) _(EXT_debug_utils, EXT_AVAILABLE_DEBUG)

////////////////////////////////////////////////////////////////////////////////
// Extension functions
////////////////////////////////////////////////////////////////////////////////

#define FOR_EACH_EXTENSION_FUNCTION(_)                                         \
  _(xrCreateSpatialAnchorMSFT)                                                 \
  _(xrCreateSpatialAnchorSpaceMSFT)                                            \
  _(xrDestroySpatialAnchorMSFT)                                                \
  _(xrCreateSceneObserverMSFT)                                                 \
  _(xrDestroySceneObserverMSFT)                                                \
  _(xrCreateSceneMSFT)                                                         \
  _(xrDestroySceneMSFT)                                                        \
  _(xrComputeNewSceneMSFT)                                                     \
  _(xrGetSceneComputeStateMSFT)                                                \
  _(xrGetSceneComponentsMSFT)                                                  \
  _(xrLocateSceneComponentsMSFT)                                               \
  _(xrEnumerateSceneComputeFeaturesMSFT)                                       \
  _(xrGetSceneMeshBuffersMSFT)                                                 \
  _(xrGetVisibilityMaskKHR)                                                    \
  _(xrCreateHandTrackerEXT)                                                    \
  _(xrDestroyHandTrackerEXT)                                                   \
  _(xrLocateHandJointsEXT)                                                     \
  _(xrCreateHandMeshSpaceMSFT)                                                 \
  _(xrUpdateHandMeshMSFT)                                                      \
  _(xrEnumerateColorSpacesFB)                                                  \
  _(xrSetColorSpaceFB)                                                         \
  _(xrCreateSpatialGraphNodeSpaceMSFT)                                         \
  _(xrCreateDebugUtilsMessengerEXT)                                            \
  _(xrDestroyDebugUtilsMessengerEXT)

#if defined(OS_WINDOWS)
#define FOR_EACH_PLATFORM_FUNCTION(_)                                          \
  _(xrConvertWin32PerformanceCounterToTimeKHR)                                 \
  _(xrConvertTimeToWin32PerformanceCounterKHR)                                 \
  _(xrCreateSpatialAnchorFromPerceptionAnchorMSFT)                             \
  _(xrGetAudioOutputDeviceGuidOculus)                                          \
  _(xrGetAudioInputDeviceGuidOculus)
#else
#define FOR_EACH_PLATFORM_FUNCTION(_)                                          \
  _(xrConvertTimespecTimeToTimeKHR)                                            \
  _(xrConvertTimeToTimespecTimeKHR)
#endif

////////////////////////////////////////////////////////////////////////////////

#define DEFINE_EXT_INFO(name, available) bool name = false;
struct ExtensionsInfo {
  /**
   * The lifetime of userRequestExtensions and ExtensionsInfo should be the
   * same.
   */
  ExtensionsInfo(const std::vector<const char *> &userRequestExtensions);

  bool graphicExtension;
  bool timeExtension;
  FOR_EACH_EXT_ALL(DEFINE_EXT_INFO)
  FOR_EACH_EXT_UWP(DEFINE_EXT_INFO)
  FOR_EACH_EXT_ANDROID(DEFINE_EXT_INFO)
  FOR_EACH_EXT_LINUX(DEFINE_EXT_INFO)
  FOR_EACH_EXT_DEBUG(DEFINE_EXT_INFO)

  std::vector<const char *> extensions;
};
#undef DEFINE_EXT_INFO

#define DEFINE_PROC_MEMBER(name) PFN_##name name = nullptr;
struct ExtensionsFunction {
  ExtensionsFunction(const Context &context);

  FOR_EACH_EXTENSION_FUNCTION(DEFINE_PROC_MEMBER)
  FOR_EACH_PLATFORM_FUNCTION(DEFINE_PROC_MEMBER)
  PFN_xrGetGraphicsRequirementsKHR xrGetGraphicsRequirementsKHR;
};
#undef DEFINE_PROC_MEMBER

#endif

#endif // EXTENSIONS_H
