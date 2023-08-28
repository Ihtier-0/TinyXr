#ifndef TINYXR_IMPL_OPENXR_UTILS_H
#define TINYXR_IMPL_OPENXR_UTILS_H

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/openxr.h"
#include "tinyxr/impl/openxr_math.h"

#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <typeindex>
#include <unordered_map>

TINYXR_NAMESPACE_OPEN

////////////////////////////////////////////////////////////////////////////////
/// ToString
////////////////////////////////////////////////////////////////////////////////

#define TO_STRING_DECLARATION(type) std::string type##ToString(const type &var);

TO_STRING_DECLARATION(XrVersion)
TO_STRING_DECLARATION(XrResult)
TO_STRING_DECLARATION(XrFormFactor)
TO_STRING_DECLARATION(XrViewConfigurationType)
TO_STRING_DECLARATION(XrEnvironmentBlendMode)
TO_STRING_DECLARATION(XrReferenceSpaceType)
TO_STRING_DECLARATION(XrActionType)

#undef TO_STRING_DECLARATION

////////////////////////////////////////////////////////////////////////////////
/// FromString
////////////////////////////////////////////////////////////////////////////////

#define FROM_STRING_DECLARATION(type)                                          \
  type type##FromString(const std::string &string);

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

template <class T> inline void validFields(T &t) {}

template <>
inline void validFields<XrReferenceSpaceCreateInfo>(
    XrReferenceSpaceCreateInfo &createInfo) {
  createInfo.poseInReferenceSpace = XrPosefIdentity();
}

/**
 * @return XrStruct with the correct type and all other fields set to zero.
 */
template <class T> T valid() {
#define TO_MAP_VALUE(xrStruct, type) {typeid(xrStruct), type},
  static const std::unordered_map<std::type_index, XrStructureType> map = {
      XR_LIST_STRUCTURE_TYPES(TO_MAP_VALUE)};
#undef TO_MAP_VALUE

  if (map.find(typeid(T)) == map.end()) {
    return {};
  }

  T result;
  std::memset(&result, 0, sizeof(result));
  result.type = map.at(typeid(T));
  validFields(result);

  return result;
}

////////////////////////////////////////////////////////////////////////////////
/// TWO_CALL
////////////////////////////////////////////////////////////////////////////////

template <class T>
using XrFunction =
    std::function<XrResult(uint32_t input, uint32_t *output, T *info)>;

template <class T>
bool twoCall(const XrFunction<T> &function, const std::string &functionName,
             std::vector<T> &vector) {
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
#define TWO_CALL(function, result)                                             \
  (twoCall<decltype(result)::value_type>((function), (#function), (result)))

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_IMPL_OPENXR_UTILS_H
