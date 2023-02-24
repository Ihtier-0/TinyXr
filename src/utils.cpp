#include "utils.h"

////////////////////////////////////////////////////////////////////////////////
/// toString
////////////////////////////////////////////////////////////////////////////////

std::string toString(const XrVersion &version) {
  return std::to_string(XR_VERSION_MAJOR(version)) + "." +
         std::to_string(XR_VERSION_MINOR(version)) + "." +
         std::to_string(XR_VERSION_PATCH(version));
}

#define TO_STRING(name) #name

#define TO_STRING_CASE(name, val)                                              \
  case name:                                                                   \
    return TO_STRING(name);

#define TO_STRING_DEFINITION(type)                                             \
  std::string toString(const type &var) {                                      \
    switch (var) {                                                             \
      XR_LIST_ENUM_##type(TO_STRING_CASE) default                              \
          : return TO_STRING(Unknown##type);                                   \
    }                                                                          \
  }

TO_STRING_DEFINITION(XrResult)

#undef TO_STRING_DEFINITION
#undef TO_STRING_CASE
#undef TO_STRING
