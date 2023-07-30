#include "tinyxr/core/manager.h"

#include "tinyxr/impl/manager.h"

TINYXR_NAMESPACE_OPEN

ManagerXr::ManagerXr(const Config &confing) {
  mImpl = std::make_unique<ManagerXRImpl>(confing);
}

ManagerXr::~ManagerXr() {}

bool ManagerXr::init() { return mImpl->init(); }

TINYXR_NAMESPACE_CLOSE
