#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "openxr.h"

////////////////////////////////////////////////////////////////////////////////
/// toString
////////////////////////////////////////////////////////////////////////////////

#define TO_STRING_DECLARATION(type) std::string toString(const type &var);

TO_STRING_DECLARATION(XrVersion)
TO_STRING_DECLARATION(XrResult)
TO_STRING_DECLARATION(XrFormFactor)

#undef TO_STRING_DECLARATION

////////////////////////////////////////////////////////////////////////////////
/// valid
////////////////////////////////////////////////////////////////////////////////

/**
 * return an XrStruct with the correct type and all other fields set to zero.
 */
template <class T> T valid() {
#define TO_MAP_VALUE(xrStruct, type) {typeid(xrStruct), type},

  static const std::unordered_map<std::type_index, XrStructureType> map = {
      XR_LIST_STRUCTURE_TYPES_CORE(TO_MAP_VALUE)};

#undef TO_MAP_VALUE

  if (map.find(typeid(T)) == map.end()) {
    return {};
  }

  T result;
  std::memset(&result, 0, sizeof(result));
  result.type = map.at(typeid(T));

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
             std::vector<T> &result) {
  XrResult xrResult;

  uint32_t input;

  xrResult = function(0, &input, nullptr);

  if (XR_FAILED(xrResult)) {
    std::cout << functionName << " first call failed: " << toString(xrResult);
    return false;
  }

  if constexpr (std::is_class<T>::value) {
    result.resize(input, valid<T>());
  } else {
    result.resize(input);
  }

  xrResult = function(input, &input, result.data());

  if (XR_FAILED(xrResult)) {
    std::cout << functionName << " second call failed: " << toString(xrResult);
    return false;
  }

  return true;
}

/**
 * @brief simplify the OpenXR two-call idiom.
 */
#define TWO_CALL(function, result)                                             \
  (twoCall<decltype(result)::value_type>((function), (#function), (result)))

#endif // UTILS_H
