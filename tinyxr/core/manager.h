#ifndef TINYXR_CORE_MANAGER_H
#define TINYXR_CORE_MANAGER_H

#include <memory>

#include "tinyxr/core/api.h"
#include "tinyxr/core/irenderer.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

class ManagerXRImpl;

class TINYXR_API ManagerXr {
  ManagerXr(const ManagerXr &) = delete;
  ManagerXr(const ManagerXr &&) = delete;
  ManagerXr &operator=(const ManagerXr &) = delete;
  ManagerXr &operator=(const ManagerXr &&) = delete;

public:
  ManagerXr(IConfigPtr confing, IRendererPtr renderer);
  ~ManagerXr();

  bool startRender();

  void requestExit();

private:
  IConfigPtr mConfig;

  std::unique_ptr<ManagerXRImpl> mImpl;
  IRendererPtr mRenderer;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_MANAGER_H
