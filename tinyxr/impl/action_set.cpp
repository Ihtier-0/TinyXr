#include "tinyxr/impl/action_set.h"

#include "json/json.h"

#include <fstream>

TINYXR_NAMESPACE_OPEN

Json::Value readFile(const std::string &filePath) {
  Json::Value root;

  std::ifstream file(filePath);
  if (file.bad()) {
    return root;
  }

  file >> root;
  return root;
}

TINYXR_NAMESPACE_CLOSE
