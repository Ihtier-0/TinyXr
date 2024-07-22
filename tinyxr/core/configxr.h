#ifndef TINYXR_CORE_CONFIGXR_H
#define TINYXR_CORE_CONFIGXR_H

#include <memory>
#include <string>
#include <vector>

#include "tinyxr/core/api.h"
#include "tinyxr/core/object_factory.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

class IConfigXr {
public:
  IConfigXr() = default;
  virtual ~IConfigXr() = default;

  virtual std::string getString(const std::string &key,
                                const std::string &defaultValue = {}) const = 0;
  virtual std::vector<std::string> getStringVector(
      const std::string &key,
      const std::vector<std::string> &defaultValue = {}) const = 0;
};
using ConfigXrPtr = std::unique_ptr<IConfigXr>;

class IConfigXrFactory : public Object {
public:
  virtual ConfigXrPtr create(const std::string &filename) = 0;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_CONFIGXR_H
