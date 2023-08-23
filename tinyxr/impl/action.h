#ifndef TINYXR_IMPL_ACTION_H
#define TINYXR_IMPL_ACTION_H

#include "tinyxr/core/tinyxr.h"

#include <string>
#include <unordered_map>

TINYXR_NAMESPACE_OPEN

struct Action {};
using Actions = std::unordered_map<std::string, Action>;

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_IMPL_ACTION_H
