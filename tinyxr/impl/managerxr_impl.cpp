#include "tinyxr/impl/managerxr_impl.h"

TINYXR_NAMESPACE_OPEN

class ManagerXrImpl : public IManagerXr {
public:
  ManagerXrImpl(ConfigXrPtr&& confing) : mConfing(std::move(confing)) {}
  ~ManagerXrImpl() {}

private:
  ConfigXrPtr mConfing;
};

////////////////////////////////////////////////////////////////////////////////
/// ManagerXrFactoryImpl
////////////////////////////////////////////////////////////////////////////////

ManagerXrPtr ManagerXrFactoryImpl::create(
    ConfigXrPtr&& confing) /* override */ {
  return std::make_unique<ManagerXrImpl>(std::move(confing));
}

TINYXR_NAMESPACE_CLOSE
