#include "tinyxr/core/config.h"

#include "tinyxr/impl/config.h"

TINYXR_NAMESPACE_OPEN

Config::Config(const std::string &filename) {
  mImpl = std::make_shared<ConfigImpl>(filename);
}

Config::~Config() {}

std::shared_ptr<ConfigImpl> Config::getImpl() { return mImpl; }

bool Config::isValid() const { return mImpl->isValid(); }

TINYXR_NAMESPACE_CLOSE
