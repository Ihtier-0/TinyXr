#include "utils.h"

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
TO_STRING_DEFINITION(XrFormFactor)

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

#define FROM_STRING_IF(name, value)                                            \
  if (#name == string) {                                                       \
    return name;                                                               \
  }

#define FROM_STRING_DEFINITION(type)                                           \
  type type##FromString(const std::string &string) {                           \
    XR_LIST_ENUM_##type(FROM_STRING_IF) return static_cast<type>(XR_MAX_ENUM); \
  }

FROM_STRING_DEFINITION(XrFormFactor)

#undef FROM_STRING_DEFINITION
#undef FROM_STRING_IF
#undef XR_MAX_ENUM

////////////////////////////////////////////////////////////////////////////////
/// split (fuck you, cpp)
////////////////////////////////////////////////////////////////////////////////

// based on: https://www.techiedelight.com/split-string-cpp-using-delimiter/
std::vector<std::string> split(const std::string &str, const char &delim) {
  std::vector<std::string> result;

  size_t start;
  size_t end = 0;

  while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
    end = str.find(delim, start);
    result.push_back(str.substr(start, end - start));
  }

  return result;
}
