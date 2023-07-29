#include "tinyxr/core/manager.h"

#include "tinyxr/impl/manager.h"

TINYXR_NAMESPACE_OPEN

ManagerXr::ManagerXr() { m_impl = std::make_unique<ManagerXRImpl>(); }

ManagerXr::~ManagerXr() {}

bool ManagerXr::init() { return m_impl->init(); }

TINYXR_NAMESPACE_CLOSE
