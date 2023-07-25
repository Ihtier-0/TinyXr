#ifndef TINYXR_IMPL_OPENXR_UTILS_H
#define TINYXR_IMPL_OPENXR_UTILS_H

#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/openxr.h"
#include "tinyxr/impl/openxrmath.h"

TINYXR_NAMESPACE_OPEN

////////////////////////////////////////////////////////////////////////////////
/// ToString
////////////////////////////////////////////////////////////////////////////////

#define TO_STRING_DECLARATION(type) std::string type##ToString(const type& var);

TO_STRING_DECLARATION(XrVersion)

// TODO! add platform definitions
// enum
TO_STRING_DECLARATION(XrResult)
TO_STRING_DECLARATION(XrStructureType)
TO_STRING_DECLARATION(XrFormFactor)
TO_STRING_DECLARATION(XrViewConfigurationType)
TO_STRING_DECLARATION(XrEnvironmentBlendMode)
TO_STRING_DECLARATION(XrReferenceSpaceType)
TO_STRING_DECLARATION(XrActionType)
TO_STRING_DECLARATION(XrEyeVisibility)
TO_STRING_DECLARATION(XrSessionState)
TO_STRING_DECLARATION(XrObjectType)
// TO_STRING_DECLARATION(XrAndroidThreadTypeKHR)
TO_STRING_DECLARATION(XrVisibilityMaskTypeKHR)
TO_STRING_DECLARATION(XrPerfSettingsDomainEXT)
TO_STRING_DECLARATION(XrPerfSettingsSubDomainEXT)
TO_STRING_DECLARATION(XrPerfSettingsLevelEXT)
TO_STRING_DECLARATION(XrPerfSettingsNotificationLevelEXT)
TO_STRING_DECLARATION(XrBlendFactorFB)
TO_STRING_DECLARATION(XrSpatialGraphNodeTypeMSFT)
TO_STRING_DECLARATION(XrHandEXT)
TO_STRING_DECLARATION(XrHandJointEXT)
TO_STRING_DECLARATION(XrHandJointSetEXT)
TO_STRING_DECLARATION(XrHandPoseTypeMSFT)
TO_STRING_DECLARATION(XrReprojectionModeMSFT)
TO_STRING_DECLARATION(XrBodyJointFB)
TO_STRING_DECLARATION(XrBodyJointSetFB)
TO_STRING_DECLARATION(XrHandJointsMotionRangeEXT)
TO_STRING_DECLARATION(XrSceneComputeFeatureMSFT)
TO_STRING_DECLARATION(XrSceneComputeConsistencyMSFT)
TO_STRING_DECLARATION(XrMeshComputeLodMSFT)
TO_STRING_DECLARATION(XrSceneComponentTypeMSFT)
TO_STRING_DECLARATION(XrSceneObjectTypeMSFT)
TO_STRING_DECLARATION(XrScenePlaneAlignmentTypeMSFT)
TO_STRING_DECLARATION(XrSceneComputeStateMSFT)
TO_STRING_DECLARATION(XrEyeExpressionHTC)
TO_STRING_DECLARATION(XrLipExpressionHTC)
TO_STRING_DECLARATION(XrFacialTrackingTypeHTC)
TO_STRING_DECLARATION(XrColorSpaceFB)
TO_STRING_DECLARATION(XrSpaceComponentTypeFB)
TO_STRING_DECLARATION(XrFoveationLevelFB)
TO_STRING_DECLARATION(XrFoveationDynamicFB)
TO_STRING_DECLARATION(XrWindingOrderFB)
TO_STRING_DECLARATION(XrPassthroughLayerPurposeFB)
TO_STRING_DECLARATION(XrHandForearmJointULTRALEAP)
TO_STRING_DECLARATION(XrSpaceQueryActionFB)
TO_STRING_DECLARATION(XrSpaceStorageLocationFB)
TO_STRING_DECLARATION(XrSpacePersistenceModeFB)
TO_STRING_DECLARATION(XrFaceExpressionFB)
TO_STRING_DECLARATION(XrFaceExpressionSetFB)
TO_STRING_DECLARATION(XrFaceConfidenceFB)
TO_STRING_DECLARATION(XrEyePositionFB)
// TO_STRING_DECLARATION(XrCompareOpFB)
TO_STRING_DECLARATION(XrLocalDimmingModeMETA)
TO_STRING_DECLARATION(XrVirtualKeyboardLocationTypeMETA)
TO_STRING_DECLARATION(XrVirtualKeyboardInputSourceMETA)
// TO_STRING_DECLARATION(XrExternalCameraAttachedToDeviceOCULUS)
TO_STRING_DECLARATION(XrPerformanceMetricsCounterUnitMETA)
TO_STRING_DECLARATION(XrPassthroughColorLutChannelsMETA)
TO_STRING_DECLARATION(XrTrackingOptimizationSettingsDomainQCOM)
TO_STRING_DECLARATION(XrTrackingOptimizationSettingsHintQCOM)
TO_STRING_DECLARATION(XrPassthroughFormHTC)
TO_STRING_DECLARATION(XrFoveationModeHTC)
TO_STRING_DECLARATION(XrFoveationLevelHTC)
TO_STRING_DECLARATION(XrForceFeedbackCurlLocationMNDX)
TO_STRING_DECLARATION(XrHandTrackingDataSourceEXT)
TO_STRING_DECLARATION(XrPlaneDetectorOrientationEXT)
TO_STRING_DECLARATION(XrPlaneDetectorSemanticTypeEXT)
TO_STRING_DECLARATION(XrPlaneDetectionStateEXT)

#undef TO_STRING_DECLARATION

////////////////////////////////////////////////////////////////////////////////
/// FromString
////////////////////////////////////////////////////////////////////////////////

#define FROM_STRING_DECLARATION(type) \
  type type##FromString(const std::string& string);

FROM_STRING_DECLARATION(XrVersion)
FROM_STRING_DECLARATION(XrFormFactor)
FROM_STRING_DECLARATION(XrViewConfigurationType)
FROM_STRING_DECLARATION(XrEnvironmentBlendMode)
FROM_STRING_DECLARATION(XrReferenceSpaceType)
FROM_STRING_DECLARATION(XrActionType)

#undef FROM_STRING_DECLARATION

////////////////////////////////////////////////////////////////////////////////
/// valid
////////////////////////////////////////////////////////////////////////////////

template <class T, class = void>
struct TypeSetter {
  static void set(T& t) {}
};

template <class T>
struct TypeSetter<T, std::enable_if_t<std::is_same_v<
                         decltype(std::declval<T>().type), XrStructureType>>> {
  static void set(T& t) {
#define TO_MAP_VALUE(xrStruct, type) {typeid(xrStruct), type},
    static const std::unordered_map<std::type_index, XrStructureType> map = {
        XR_LIST_STRUCTURE_TYPES(TO_MAP_VALUE)};
#undef TO_MAP_VALUE

    const auto find = map.find(typeid(T));
    if (find == map.end()) {
      return;
    }

    t.type = find->second;
  }
};

template <class T>
inline void setValidFields(T& t) {}

template <>
inline void setValidFields<XrReferenceSpaceCreateInfo>(
    XrReferenceSpaceCreateInfo& info) {
  info.poseInReferenceSpace = XrPosefIdentity();
}

/**
 * @return XrStruct with the correct type and all other fields set to zero.
 */
template <class T>
T valid() {
  T result;
  std::memset(&result, 0, sizeof(result));
  TypeSetter<T>::set(result);
  setValidFields(result);
  return result;
}

////////////////////////////////////////////////////////////////////////////////
/// TWO_CALL
////////////////////////////////////////////////////////////////////////////////

template <class T>
using XrFunction =
    std::function<XrResult(uint32_t input, uint32_t* output, T* info)>;

template <class T>
bool twoCall(const XrFunction<T>& function, const std::string& functionName,
             std::vector<T>& vector) {
  XrResult result;
  uint32_t input;

  result = function(0, &input, nullptr);
  if (XR_FAILED(result)) {
    std::cout << functionName +
                     " first call failed: " + XrResultToString(result)
              << std::endl;
    return false;
  }

  if constexpr (std::is_class<T>::value) {
    vector.resize(input, valid<T>());
  } else {
    vector.resize(input);
  }

  result = function(input, &input, vector.data());
  if (XR_FAILED(result)) {
    std::cout << functionName +
                     " second call failed: " + XrResultToString(result)
              << std::endl;
    return false;
  }

  return true;
}

/**
 * @brief simplify the OpenXR two-call idiom.
 */
#define TWO_CALL(function, result) \
  (twoCall<decltype(result)::value_type>((function), (#function), (result)))

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_OPENXR_UTILS_H
