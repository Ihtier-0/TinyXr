#include "tinyxr/impl/action.h"

TINYXR_NAMESPACE_OPEN

static const std::string leftHandSubactionPaths = "/user/hand/left";
static const std::string rightHandSubactionPaths = "/user/hand/right";

Side SideFromPath(const std::string &path) {
  if (path == leftHandSubactionPaths) {
    return Side::Left;
  } else if (path == rightHandSubactionPaths) {
    return Side::Right;
  } else {
    return Side::Unknown;
  }
}

TINYXR_NAMESPACE_CLOSE
