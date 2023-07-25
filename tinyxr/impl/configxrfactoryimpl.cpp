#include "tinyxr/impl/configxrfactoryimpl.h"

#include <iostream>
#include <toml++/toml.hpp>

#include "tinyxr/impl/stringutils.h"

TINYXR_NAMESPACE_OPEN

static void insertAtPath(toml::table& root, const std::string& path,
                         const toml::node& value) {
  size_t pos = path.find('.');
  if (pos == std::string::npos) {
    root.insert(path, value);
    return;
  }

  std::string first = path.substr(0, pos);
  std::string rest = path.substr(pos + 1);

  toml::table* subtable =
      root[first].is_table() ? root[first].as_table() : nullptr;
  if (!subtable) {
    root.insert(first, toml::table{});
    subtable = root[first].as_table();
  }

  if (subtable) {
    insertAtPath(*subtable, rest, value);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// ConfigXrImpl
////////////////////////////////////////////////////////////////////////////////

class ConfigXrImpl final : public IConfigXr {
public:
  ConfigXrImpl(const std::string& filename) {
    try {
      mTable.reset(new toml::table(toml::parse_file(filename)));
    } catch (toml::parse_error e) {
      mTable.reset();
      std::cout << "Exception " << e.what() << '\n'
                << "While read toml config from: " << filename << std::endl;
    }
  }

  bool isValid() const { return !!mTable; }

  std::string getString(const std::string& key,
                        const std::string& defaultValue = {}) const override {
    const auto node = mTable->at_path(key);
    if (!node) {
      return defaultValue;
    }

    const auto string = node.as_string();
    if (!string) {
      return defaultValue;
    }

    return string->value_or(defaultValue);
  }
  Strings getStrings(const std::string& key,
                     const Strings& defaultValue = {}) const override {
    const auto node = mTable->at_path(key);
    if (!node) {
      return defaultValue;
    }

    const auto* array = node.as_array();
    if (!array) {
      return defaultValue;
    }

    Strings out;
    for (const auto& v : *array) {
      const auto string = v.value<std::string>();
      if (string.has_value()) {
        out.emplace_back(string.value());
      }
    }
    return out.empty() ? defaultValue : out;
  }

  void setString(const std::string& key,
                 const std::string& value) const override {
    insertAtPath(*mTable, key, toml::value<std::string>(value));
  }
  void setStrings(const std::string& key,
                  const Strings& values) const override {
    toml::array array;
    for (const auto& value : values) {
      array.push_back(value);
    }
    insertAtPath(*mTable, key, array);
  }

  double getDouble(const std::string& key,
                   const double defaultValue = {}) const override {
    const auto node = mTable->at_path(key);
    if (!node) {
      return defaultValue;
    }

    const auto number = node.as_floating_point();
    if (!number) {
      return defaultValue;
    }

    return number->value_or(defaultValue);
  }
  virtual void setDouble(const std::string& key,
                         const double value) const override {
    insertAtPath(*mTable, key, toml::value<double>(value));
  }

private:
  std::shared_ptr<toml::table> mTable;
};

////////////////////////////////////////////////////////////////////////////////
/// ConfigXrFactoryImpl
////////////////////////////////////////////////////////////////////////////////

IConfigXrPtr ConfigXrFactoryImpl::create(
    const std::string& filename) /* override */ {
  auto config = std::make_unique<ConfigXrImpl>(filename);
  return config->isValid() ? std::move(config) : nullptr;
}

TINYXR_NAMESPACE_CLOSE
