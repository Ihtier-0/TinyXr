#ifndef TINYXR_IMPL_CONFIG_H
#define TINYXR_IMPL_CONFIG_H

#include <cpptoml.h>

#include <memory>
#include <vector>

#include "tinyxr/core/api.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

class ConfigImpl {
public:
  ConfigImpl(const std::string &filename);
  ~ConfigImpl();

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

#endif  // TINYXR_IMPL_CONFIG_H
