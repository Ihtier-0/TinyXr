#ifndef TINYXR_IMPL_ACTION_H
#define TINYXR_IMPL_ACTION_H

#include <string>
#include <unordered_map>

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/openxr.h"

TINYXR_NAMESPACE_OPEN

using Path = std::string;
using SubactionPathMap = std::unordered_map<Path, XrPath>;

struct Action {
  XrActionType type;
  XrAction action = XR_NULL_HANDLE;

  SubactionPathMap subactionPaths;
};
using ActionName = std::string;
using ActionsMap = std::unordered_map<ActionName, Action>;

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_ACTION_H
