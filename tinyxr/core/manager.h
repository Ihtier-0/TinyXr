#ifndef TINYXR_CORE_MANAGER_H
#define TINYXR_CORE_MANAGER_H

#include "tinyxr/core/api.h"
#include "tinyxr/core/tinyxr.h"

#include <memory>

TINYXR_NAMESPACE_OPEN

class ManagerXRImpl;

class TINYXR_API ManagerXr {
public:
  ManagerXr();
  ~ManagerXr();

private:
  std::unique_ptr<ManagerXRImpl> m_impl;
};

TINYXR_NAMESPACE_CLOSE

#endif // TINYXR_CORE_MANAGER_H
