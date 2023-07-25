#include "tinyxr/impl/stringutils.h"

TINYXR_NAMESPACE_OPEN

std::vector<std::string> split(const std::string &str, const char &delim) {
  std::vector<std::string> result;

  size_t start;
  size_t end = 0;

  while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
    end = str.find(delim, start);
    result.push_back(str.substr(start, end - start));
  }

  return result;
}

TINYXR_NAMESPACE_CLOSE
