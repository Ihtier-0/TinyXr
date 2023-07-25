#ifndef TINYXR_CORE_IMANAGERXR_H
#define TINYXR_CORE_IMANAGERXR_H

#include <memory>

#include "tinyxr/core/iconfigxr.h"

TINYXR_NAMESPACE_OPEN

class ManagerXRImpl;

class IManagerXr {
public:
  IManagerXr() = default;
  virtual ~IManagerXr() = default;

  virtual int exec() = 0;
};
using IManagerXrPtr = std::unique_ptr<IManagerXr>;

class IManagerXrFactory : public IObject {
public:
  virtual IManagerXrPtr create(IConfigXrPtr &&confing) = 0;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_IMANAGERXR_H
