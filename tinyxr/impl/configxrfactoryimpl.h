#ifndef TINYXR_IMPL_CONFIGXRFACTORYIMPL_H
#define TINYXR_IMPL_CONFIGXRFACTORYIMPL_H

#include "tinyxr/core/iconfigxr.h"

TINYXR_NAMESPACE_OPEN

class ConfigXrFactoryImpl final : public IConfigXrFactory {
public:
  IConfigXrPtr create(const std::string &filename) override;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_CONFIGXRFACTORYIMPL_H
