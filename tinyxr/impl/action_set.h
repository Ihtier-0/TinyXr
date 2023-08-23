#ifndef TINYXR_IMPL_ACTION_SET_H
#define TINYXR_IMPL_ACTION_SET_H

#include "tinyxr/core/tinyxr.h"

#include <string>
#include <unordered_map>

TINYXR_NAMESPACE_OPEN

struct ActionSet {};
using ActionSets = std::unordered_map<std::string, ActionSet>;

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_IMPL_ACTION_SET_H
