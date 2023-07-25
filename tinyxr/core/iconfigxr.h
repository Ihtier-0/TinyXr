#ifndef TINYXR_CORE_ICONFIGXR_H
#define TINYXR_CORE_ICONFIGXR_H

#include <memory>
#include <string>
#include <vector>

#include "tinyxr/core/iobject.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

class IConfigXr {
public:
  using Strings = std::vector<std::string>;

  IConfigXr() = default;
  virtual ~IConfigXr() = default;

  virtual std::string getString(const std::string &key,
                                const std::string &defaultValue = {}) const = 0;
  virtual Strings getStrings(const std::string &key,
                             const Strings &defaultValue = {}) const = 0;

  virtual void setString(const std::string &key,
                         const std::string &value) const = 0;
  virtual void setStrings(const std::string &key,
                          const Strings &value) const = 0;

  virtual double getDouble(const std::string &key,
                           const double defaultValue = {}) const = 0;
  virtual void setDouble(const std::string &key, const double value) const = 0;
};
using IConfigXrPtr = std::unique_ptr<IConfigXr>;

class IConfigXrFactory : public IObject {
public:
  virtual IConfigXrPtr create(const std::string &filename) = 0;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_ICONFIGXR_H
