#include "tinyxr/core/manager.h"

#include "tinyxr/impl/manager.h"

TINYXR_NAMESPACE_OPEN

ManagerXr::ManagerXr(const Config &confing, IRendererPtr renderer)
    : mConfig(confing), mRenderer(renderer) {}

ManagerXr::~ManagerXr() {}

bool ManagerXr::startRender() {
  bool requestExit = false;
  bool requestRestart = false;
  bool requestExitRenderLoop = false;

  do {
    mImpl = std::make_unique<ManagerXRImpl>(mConfig, mRenderer);

    if (!mImpl->init()) {
      return false;
    }

    while (!requestExit) {
      if (!mImpl->pollEvents()) {
        continue;
      }

      requestExit = mImpl->exitRequested();
      requestRestart = mImpl->restartRequested();
      requestExitRenderLoop = mImpl->exitRenderLoopRequested();

      if (requestExitRenderLoop) {
        break;
      }

      if (mImpl->sessionRunning()) {
        mImpl->pollActions();

        mImpl->beforeFrames();
        mImpl->renderFrames();
        mImpl->afterFrames();
      } else {
      }
    }

    mImpl.reset(nullptr);
  } while (requestRestart && !requestExit);

  return false;
}

void ManagerXr::requestExit() {
  if (mImpl) {
    mImpl->requestExit();
  }
}

TINYXR_NAMESPACE_CLOSE
