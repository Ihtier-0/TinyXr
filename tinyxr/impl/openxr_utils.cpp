#include "tinyxr/impl/openxr_utils.h"
#include "tinyxr/impl/utils.h"

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

#define TO_STRING_CASE(name, val)                                              \
  case name:                                                                   \
    return TO_STRING(name);

#define TO_STRING_DEFINITION(type)                                             \
  std::string type##ToString(const type &var) {                                \
    switch (var) {                                                             \
      XR_LIST_ENUM_##type(TO_STRING_CASE) default                              \
          : return TO_STRING(Unknown##type);                                   \
    }                                                                          \
  }

TO_STRING_DEFINITION(XrResult)

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

TINYXR_NAMESPACE_CLOSE