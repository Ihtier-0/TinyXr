#ifndef TINYXR_IMPL_ACTION_H
#define TINYXR_IMPL_ACTION_H

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/openxr.h"

#include <string>
#include <unordered_map>

TINYXR_NAMESPACE_OPEN

enum class Side {
  Left,
  Right,
  Unknown,
};
Side SideFromPath(const std::string &path);

struct SubactionPath {
  XrPath path;
  Side side;
};
using Path = std::string;
using SubactionPathMap = std::unordered_map<Path, SubactionPath>;

struct Action {
  XrActionType type;
  XrAction action = XR_NULL_HANDLE;

  SubactionPathMap subactionPaths;
};
using ActionName = std::string;
using ActionsMap = std::unordered_map<ActionName, Action>;

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_IMPL_ACTION_H
