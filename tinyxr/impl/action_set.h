#ifndef TINYXR_IMPL_ACTION_SET_H
#define TINYXR_IMPL_ACTION_SET_H

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/action.h"
#include "tinyxr/impl/openxr.h"

#include <string>
#include <unordered_map>

TINYXR_NAMESPACE_OPEN

struct ActionSet {
  XrActionSet set = XR_NULL_HANDLE;

  ActionsMap actions;
};

using ActionSetName = std::string;
using ActionSetsMap = std::unordered_map<ActionSetName, ActionSet>;

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_IMPL_ACTION_SET_H
