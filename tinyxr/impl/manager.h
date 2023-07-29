#ifndef TINYXR_IMPL_MANAGER_H
#define TINYXR_IMPL_MANAGER_H

#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/context.h"

TINYXR_NAMESPACE_OPEN

class ManagerXRImpl {
  ManagerXRImpl(const ManagerXRImpl &) = delete;
  ManagerXRImpl(const ManagerXRImpl &&) = delete;
  ManagerXRImpl &operator=(const ManagerXRImpl &) = delete;
  ManagerXRImpl &operator=(const ManagerXRImpl &&) = delete;

public:
  ManagerXRImpl() = default;
  ~ManagerXRImpl() = default;

  bool init();

private:
  // Instance
  bool logInstanceInfo();
  bool createInstanceImpl();
  bool logLayersAndExtensions();
  bool createInstance();

  Context mContext;
};

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_IMPL_MANAGER_H
