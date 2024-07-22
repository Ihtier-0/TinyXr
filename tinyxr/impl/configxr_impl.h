#ifndef TINYXR_IMPL_CONFIGXR_H
#define TINYXR_IMPL_CONFIGXR_H

#include "tinyxr/core/configxr.h"

TINYXR_NAMESPACE_OPEN

class ConfigXrFactoryImpl final : public IConfigXrFactory {
public:
  ConfigXrPtr create(const std::string &filename) override;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_CONFIGXR_H
