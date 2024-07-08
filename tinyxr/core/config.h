#ifndef TINYXR_CORE_CONFIG_H
#define TINYXR_CORE_CONFIG_H

#include <memory>
#include <string>
#include <vector>

#include "tinyxr/core/api.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

class IConfig;
using IConfigPtr = std::unique_ptr<IConfig>;

class IConfig {
public:
  TINYXR_API static IConfigPtr create(const std::string &filename);

  virtual std::string getString(const std::string &key,
                                const std::string &defaultValue = {}) const = 0;
  virtual std::vector<std::string> getStringVector(
      const std::string &key,
      const std::vector<std::string> &defaultValue = {}) const = 0;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_CONFIG_H
