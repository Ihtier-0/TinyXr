#ifndef TINYXR_IMPL_SWAPCHAIN_H
#define TINYXR_IMPL_SWAPCHAIN_H

#include <vector>

#include "tinyxr/impl/openxr.h"
#include "tinyxr/impl/openxrextensions.h"

struct Swapchain {
  XrSwapchain handle = XR_NULL_HANDLE;

  int32_t width = -1;
  int32_t height = -1;

  using Images = std::vector<XrSwapchainImage>;
  Images images;
};
using Swapchains = std::vector<Swapchain>;

#endif  // TINYXR_IMPL_SWAPCHAIN_H
