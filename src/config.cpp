#include "config.h"

#include <iostream>

Config::Config(const std::string &filename) {
  try {
    mTable = cpptoml::parse_file(filename);
  } catch (cpptoml::parse_exception e) {
    mTable.reset();

    std::cout << "Exception " << e.what() << std::endl
              << "While read toml config from: " << filename << std::endl;
  }
}

bool Config::valid() { return !!mTable; }
