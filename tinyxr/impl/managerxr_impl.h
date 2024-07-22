#ifndef TINYXR_IMPL_MANAGERXR_H
#define TINYXR_IMPL_MANAGERXR_H

#include "tinyxr/core/managerxr.h"

TINYXR_NAMESPACE_OPEN

class ManagerXrFactoryImpl final : public IManagerXrFactory {
public:
  ManagerXrPtr create(ConfigXrPtr &&confing) override;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_MANAGERXR_H
