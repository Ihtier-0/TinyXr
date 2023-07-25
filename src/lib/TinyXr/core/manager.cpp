#include "TinyXr/core/manager.h"

#include "TinyXr/impl/manager.h"

TINYXR_NAMESPACE_OPEN

ManagerXr::ManagerXr() { m_impl = std::make_unique<ManagerXRImpl>(); }

ManagerXr::~ManagerXr() { }

TINYXR_NAMESPACE_CLOSE
