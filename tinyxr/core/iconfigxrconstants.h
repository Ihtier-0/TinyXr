#ifndef TINYXR_CORE_ICONFIGXRCONSTANTS_H
#define TINYXR_CORE_ICONFIGXRCONSTANTS_H

#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

constexpr const auto currentRenderer = "xr.currentRenderer";
constexpr const auto userRequestExtensions =
    "xr.instance.userRequestExtensions";
constexpr const auto applicationName =
    "xr.instance.applicationInfo.applicationName";
constexpr const auto formFactor = "xr.system.formFactor";
constexpr const auto viewConfigType = "xr.viewConfigType";
constexpr const auto environmentBlendMode = "xr.environmentBlendMode";
constexpr const auto actionSetsConfig = "xr.actionSetsConfig";
constexpr const auto interactionProfilesConfig = "xr.interactionProfilesConfig";
constexpr const auto appSpace = "xr.appSpace";
constexpr const auto nearZ = "xr.nearZ";
constexpr const auto farZ = "xr.farZ";

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_ICONFIGXRCONSTANTS_H
