#ifndef TINYXR_IMPL_MANAGERXRFACTORYIMPL_H
#define TINYXR_IMPL_MANAGERXRFACTORYIMPL_H

#include "tinyxr/core/imanagerxr.h"

TINYXR_NAMESPACE_OPEN

class ManagerXrFactoryImpl final : public IManagerXrFactory {
public:
  IManagerXrPtr create(IConfigXrPtr &&confing) override;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_MANAGERXRFACTORYIMPL_H
