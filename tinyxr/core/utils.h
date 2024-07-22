#ifndef TINYXR_CORE_UTILS_H
#define TINYXR_CORE_UTILS_H

#include <memory>
#include <utility>

#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

template <class TO, class FROM>
using unique_pointer_cast_result =
    std::pair<std::unique_ptr<TO>, std::unique_ptr<FROM>>;

template <class TO, class FROM>
unique_pointer_cast_result<TO, FROM> unique_pointer_cast(
    std::unique_ptr<FROM>&& ptr) noexcept {
  auto* rawPtr = ptr.release();
  auto* to = dynamic_cast<TO*>(rawPtr);
  if (to) {
    return unique_pointer_cast_result<TO, FROM>{to, nullptr};
  } else {
    return unique_pointer_cast_result<TO, FROM>{nullptr, rawPtr};
  }
}

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_UTILS_H
