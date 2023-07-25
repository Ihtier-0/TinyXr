#include "tinyxr/impl/openxrutils.h"

#include "tinyxr/impl/stringutils.h"

TINYXR_NAMESPACE_OPEN

////////////////////////////////////////////////////////////////////////////////
/// ToString
////////////////////////////////////////////////////////////////////////////////

std::string XrVersionToString(const XrVersion &version) {
  return std::to_string(XR_VERSION_MAJOR(version)) + "." +
         std::to_string(XR_VERSION_MINOR(version)) + "." +
         std::to_string(XR_VERSION_PATCH(version));
}

#define TO_STRING(name) #name

#define TO_STRING_CASE(name, val) \
  case name:                      \
    return TO_STRING(name);

#define TO_STRING_DEFINITION(type)                \
  std::string type##ToString(const type &var) {   \
    switch (var) {                                \
      XR_LIST_ENUM_##type(TO_STRING_CASE) default \
          : return TO_STRING(Unknown##type);      \
    }                                             \
  }

TO_STRING_DEFINITION(XrResult)
TO_STRING_DEFINITION(XrStructureType)
TO_STRING_DEFINITION(XrFormFactor)
TO_STRING_DEFINITION(XrViewConfigurationType)
TO_STRING_DEFINITION(XrEnvironmentBlendMode)
TO_STRING_DEFINITION(XrReferenceSpaceType)
TO_STRING_DEFINITION(XrActionType)
TO_STRING_DEFINITION(XrEyeVisibility)
TO_STRING_DEFINITION(XrSessionState)
TO_STRING_DEFINITION(XrObjectType)
// TO_STRING_DEFINITION(XrAndroidThreadTypeKHR)
TO_STRING_DEFINITION(XrVisibilityMaskTypeKHR)
TO_STRING_DEFINITION(XrPerfSettingsDomainEXT)
TO_STRING_DEFINITION(XrPerfSettingsSubDomainEXT)
TO_STRING_DEFINITION(XrPerfSettingsLevelEXT)
TO_STRING_DEFINITION(XrPerfSettingsNotificationLevelEXT)
TO_STRING_DEFINITION(XrBlendFactorFB)
TO_STRING_DEFINITION(XrSpatialGraphNodeTypeMSFT)
TO_STRING_DEFINITION(XrHandEXT)
TO_STRING_DEFINITION(XrHandJointEXT)
TO_STRING_DEFINITION(XrHandJointSetEXT)
TO_STRING_DEFINITION(XrHandPoseTypeMSFT)
TO_STRING_DEFINITION(XrReprojectionModeMSFT)
TO_STRING_DEFINITION(XrBodyJointFB)
TO_STRING_DEFINITION(XrBodyJointSetFB)
TO_STRING_DEFINITION(XrHandJointsMotionRangeEXT)
TO_STRING_DEFINITION(XrSceneComputeFeatureMSFT)
TO_STRING_DEFINITION(XrSceneComputeConsistencyMSFT)
TO_STRING_DEFINITION(XrMeshComputeLodMSFT)
TO_STRING_DEFINITION(XrSceneComponentTypeMSFT)
TO_STRING_DEFINITION(XrSceneObjectTypeMSFT)
TO_STRING_DEFINITION(XrScenePlaneAlignmentTypeMSFT)
TO_STRING_DEFINITION(XrSceneComputeStateMSFT)
TO_STRING_DEFINITION(XrEyeExpressionHTC)
TO_STRING_DEFINITION(XrLipExpressionHTC)
TO_STRING_DEFINITION(XrFacialTrackingTypeHTC)
TO_STRING_DEFINITION(XrColorSpaceFB)
TO_STRING_DEFINITION(XrSpaceComponentTypeFB)
TO_STRING_DEFINITION(XrFoveationLevelFB)
TO_STRING_DEFINITION(XrFoveationDynamicFB)
TO_STRING_DEFINITION(XrWindingOrderFB)
TO_STRING_DEFINITION(XrPassthroughLayerPurposeFB)
TO_STRING_DEFINITION(XrHandForearmJointULTRALEAP)
TO_STRING_DEFINITION(XrSpaceQueryActionFB)
TO_STRING_DEFINITION(XrSpaceStorageLocationFB)
TO_STRING_DEFINITION(XrSpacePersistenceModeFB)
TO_STRING_DEFINITION(XrFaceExpressionFB)
TO_STRING_DEFINITION(XrFaceExpressionSetFB)
TO_STRING_DEFINITION(XrFaceConfidenceFB)
TO_STRING_DEFINITION(XrEyePositionFB)
// TO_STRING_DEFINITION(XrCompareOpFB)
TO_STRING_DEFINITION(XrLocalDimmingModeMETA)
TO_STRING_DEFINITION(XrVirtualKeyboardLocationTypeMETA)
TO_STRING_DEFINITION(XrVirtualKeyboardInputSourceMETA)
// TO_STRING_DEFINITION(XrExternalCameraAttachedToDeviceOCULUS)
TO_STRING_DEFINITION(XrPerformanceMetricsCounterUnitMETA)
TO_STRING_DEFINITION(XrPassthroughColorLutChannelsMETA)
TO_STRING_DEFINITION(XrTrackingOptimizationSettingsDomainQCOM)
TO_STRING_DEFINITION(XrTrackingOptimizationSettingsHintQCOM)
TO_STRING_DEFINITION(XrPassthroughFormHTC)
TO_STRING_DEFINITION(XrFoveationModeHTC)
TO_STRING_DEFINITION(XrFoveationLevelHTC)
TO_STRING_DEFINITION(XrForceFeedbackCurlLocationMNDX)
TO_STRING_DEFINITION(XrHandTrackingDataSourceEXT)
TO_STRING_DEFINITION(XrPlaneDetectorOrientationEXT)
TO_STRING_DEFINITION(XrPlaneDetectorSemanticTypeEXT)
TO_STRING_DEFINITION(XrPlaneDetectionStateEXT)

#undef TO_STRING_DEFINITION
#undef TO_STRING_CASE
#undef TO_STRING

////////////////////////////////////////////////////////////////////////////////
/// FromString
////////////////////////////////////////////////////////////////////////////////

XrVersion XrVersionFromString(const std::string &string) {
  std::vector<std::string> splitString = split(string, '.');

  if (splitString.size() != 3) {
    return {};
  }

  try {
    return XR_MAKE_VERSION(std::stoull(splitString[0]),
                           std::stoull(splitString[1]),
                           std::stoull(splitString[2]));
  } catch (...) {
    return {};
  }
}

#define FROM_STRING_IF(name, value) \
  if (#name == string) {            \
    return name;                    \
  }

#define FROM_STRING_DEFINITION(type)                                           \
  type type##FromString(const std::string &string) {                           \
    XR_LIST_ENUM_##type(FROM_STRING_IF) return static_cast<type>(XR_MAX_ENUM); \
  }

FROM_STRING_DEFINITION(XrFormFactor)
FROM_STRING_DEFINITION(XrViewConfigurationType)
FROM_STRING_DEFINITION(XrEnvironmentBlendMode)
FROM_STRING_DEFINITION(XrReferenceSpaceType)
FROM_STRING_DEFINITION(XrActionType)

#undef FROM_STRING_DEFINITION
#undef FROM_STRING_IF

TINYXR_NAMESPACE_CLOSE
