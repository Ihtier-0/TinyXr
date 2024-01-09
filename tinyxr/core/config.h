#ifndef TINYXR_CORE_CONFIG_H
#define TINYXR_CORE_CONFIG_H

#include <memory>
#include <string>

#include "tinyxr/core/api.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

class ConfigImpl;

class Config {
public:
  TINYXR_API Config(const std::string &filename);
  TINYXR_API  ~Config();

  TINYXR_API bool isValid() const;

  std::shared_ptr<ConfigImpl> getImpl();

private:
  std::shared_ptr<ConfigImpl> mImpl;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_CONFIG_H
