#include "tinyxr/core/config.h"

#include <iostream>

TINYXR_NAMESPACE_OPEN

Config::Config(const std::string &filename) {
  try {
    mTable = cpptoml::parse_file(filename);
  } catch (cpptoml::parse_exception e) {
    mTable.reset();

    std::cout << "Exception " << e.what() << '\n'
              << "While read toml config from: " << filename << std::endl;
  }
}

Config::~Config() {}

bool Config::isValid() const { return !!mTable; }

TINYXR_NAMESPACE_CLOSE
