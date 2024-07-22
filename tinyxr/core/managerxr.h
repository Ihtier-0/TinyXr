#ifndef TINYXR_CORE_MANAGERXR_H
#define TINYXR_CORE_MANAGERXR_H

#include <memory>

#include "tinyxr/core/configxr.h"

TINYXR_NAMESPACE_OPEN

class ManagerXRImpl;

class IManagerXr {
public:
  IManagerXr() = default;
  virtual ~IManagerXr() = default;
};
using ManagerXrPtr = std::unique_ptr<IManagerXr>;

class IManagerXrFactory : public Object {
public:
  virtual ManagerXrPtr create(ConfigXrPtr &&confing) = 0;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_MANAGERXR_H
