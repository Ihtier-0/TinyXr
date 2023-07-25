#ifndef TINYXR_IMPL_STRING_UTILS_H
#define TINYXR_IMPL_STRING_UTILS_H

#include <string>
#include <vector>

#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

std::vector<std::string> split(const std::string &str, const char &delim);

template <std::size_t N>
void copyStringToCharArray(const std::string &source, char (&destination)[N]) {
  auto copySize = std::min(source.size(), N);
  if (copySize == N) {
    copySize -= 1;
  }

  std::size_t length = source.copy(destination, copySize);
  destination[length] = '\0';
}

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_STRING_UTILS_H
