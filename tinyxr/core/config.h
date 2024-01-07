#ifndef TINYXR_CORE_CONFIG_H
#define TINYXR_CORE_CONFIG_H

#include <cpptoml.h>

#include <memory>
#include <vector>

#include "tinyxr/core/api.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

class TINYXR_API Config {
public:
  Config(const std::string &filename);
  ~Config();

  template <class T>
  T getValue(const std::string &key, const T &defaultValue = T()) const {
    if (!isValid()) {
      std::cout << "Attempt to get value '" << key
                << "' from in invalid config." << std::endl;
      return defaultValue;
    }

    return mTable->get_qualified_as<T>(key).value_or(defaultValue);
  }

  template <class T>
  std::vector<T> getVector(const std::string &key,
                           const std::vector<T> &defaultValue = {}) const {
    if (!isValid()) {
      std::cout << "Attempt to get value '" << key
                << "' from in invalid config." << std::endl;
      return defaultValue;
    }

    return mTable->get_qualified_array_of<T>(key).value_or(defaultValue);
  }

  bool isValid() const;

private:
  using TomlTablePtr = std::shared_ptr<cpptoml::table>;
  TomlTablePtr mTable;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_CONFIG_H
