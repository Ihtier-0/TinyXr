#ifndef CONFIG_H
#define CONFIG_H

#include <limits>
#include <memory>

#include <cpptoml.h>

class Config {
public:
  Config(const std::string &filename);

  template <class T>
  T getValue(const std::string &key, const T &defaultValue = T()) const {
    return mTable->get_qualified_as<T>(key).value_or(defaultValue);
  }

  template <class T>
  std::vector<T> getArray(const std::string &key,
                          const std::vector<T> &defaultValue = {}) const {
    return mTable->get_qualified_array_of<T>(key).value_or(defaultValue);
  }

  bool valid();

private:
  std::shared_ptr<cpptoml::table> mTable;
};

#endif // CONFIG_H
