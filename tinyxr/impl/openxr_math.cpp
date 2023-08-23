#include "tinyxr/impl/openxr_math.h"

TINYXR_NAMESPACE_OPEN

XrPosef XrPosefIdentity() {
  XrPosef pose = {};
  pose.orientation.w = 1;
  return pose;
}

TINYXR_NAMESPACE_CLOSE
