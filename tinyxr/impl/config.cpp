#include "tinyxr/impl/config.h"

#include <iostream>

TINYXR_NAMESPACE_OPEN

ConfigImpl::ConfigImpl(const std::string &filename) {
  try {
    mTable = cpptoml::parse_file(filename);
  } catch (cpptoml::parse_exception e) {
    mTable.reset();

    std::cout << "Exception " << e.what() << '\n'
              << "While read toml config from: " << filename << std::endl;
  }
}

ConfigImpl::~ConfigImpl() {}

bool ConfigImpl::isValid() const { return !!mTable; }

TINYXR_NAMESPACE_CLOSE
