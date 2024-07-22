#include "tinyxr/impl/configxr_impl.h"

#include <cpptoml.h>

#include <iostream>

TINYXR_NAMESPACE_OPEN

////////////////////////////////////////////////////////////////////////////////
/// ConfigXrImpl
////////////////////////////////////////////////////////////////////////////////

class ConfigXrImpl : public IConfigXr {
public:
  ConfigXrImpl(const std::string& filename) {
    try {
      mTable = cpptoml::parse_file(filename);
    } catch (cpptoml::parse_exception e) {
      mTable.reset();
      std::cout << "Exception " << e.what() << '\n'
                << "While read toml config from: " << filename << std::endl;
    }
  }

  bool isValid() const { return !!mTable; }

  template <class T>
  T getValue(const std::string& key, const T& defaultValue = T()) const {
    if (!isValid()) {
      std::cout << "Attempt to get value '" << key
                << "' from in invalid config." << std::endl;
      return defaultValue;
    }

    return mTable->get_qualified_as<T>(key).value_or(defaultValue);
  }

  template <class T>
  std::vector<T> getVector(const std::string& key,
                           const std::vector<T>& defaultValue = {}) const {
    if (!isValid()) {
      std::cout << "Attempt to get value '" << key
                << "' from in invalid config." << std::endl;
      return defaultValue;
    }

    return mTable->get_qualified_array_of<T>(key).value_or(defaultValue);
  }

  std::string getString(const std::string& key,
                        const std::string& defaultValue = {}) const override {
    return getValue(key, defaultValue);
  }
  std::vector<std::string> getStringVector(
      const std::string& key,
      const std::vector<std::string>& defaultValue = {}) const override {
    return getVector(key, defaultValue);
  }

private:
  std::shared_ptr<cpptoml::table> mTable;
};

////////////////////////////////////////////////////////////////////////////////
/// ConfigXrFactoryImpl
////////////////////////////////////////////////////////////////////////////////

ConfigXrPtr ConfigXrFactoryImpl::create(
    const std::string& filename) /* override */ {
  auto config = std::make_unique<ConfigXrImpl>(filename);
  return config->isValid() ? std::move(config) : nullptr;
}

TINYXR_NAMESPACE_CLOSE
