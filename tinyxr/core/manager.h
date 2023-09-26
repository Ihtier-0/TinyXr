#ifndef TINYXR_CORE_MANAGER_H
#define TINYXR_CORE_MANAGER_H

#include "tinyxr/core/api.h"
#include "tinyxr/core/config.h"
#include "tinyxr/core/tinyxr.h"

#include <memory>

TINYXR_NAMESPACE_OPEN

class ManagerXRImpl;

class TINYXR_API ManagerXr {
  ManagerXr(const ManagerXr &) = delete;
  ManagerXr(const ManagerXr &&) = delete;
  ManagerXr &operator=(const ManagerXr &) = delete;
  ManagerXr &operator=(const ManagerXr &&) = delete;

public:
  ManagerXr(const Config &confing);
  ~ManagerXr();

  bool startRender();

  void requestExit();

private:
  Config mConfig;

  std::unique_ptr<ManagerXRImpl> mImpl;
};

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_CORE_MANAGER_H
