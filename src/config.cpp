#include "config.h"

#include "logger.h"

Config::Config(const std::string &filename) {
  try {
    mTable = cpptoml::parse_file(filename);
  } catch (cpptoml::parse_exception e) {
    mTable.reset();

    LOG_WARNING("Exception " + std::string(e.what()) + '\n' +
                "While read toml config from: " + filename);
  }
}

bool Config::valid() { return !!mTable; }
