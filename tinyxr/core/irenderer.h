#ifndef TINYXR_CORE_IRENDERER_H
#define TINYXR_CORE_IRENDERER_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "tinyxr/core/iobject.h"
#include "tinyxr/core/rendercontext.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

struct Version {
  int major;
  int minor;
  int patch;
};

class IRenderer {
public:
  virtual ~IRenderer() = default;

  virtual Version getVersion() const = 0;

  virtual void *getDeviceContext() const = 0;
  virtual void *getHandleRenderContext() const = 0;

  virtual int64_t selectSwapchainFormat(
      const std::vector<int64_t> &swapchainFormats) const = 0;

  virtual bool render(const RenderContext &renderContext,
                      const uint32_t image) = 0;
};
using IRendererPtr = std::unique_ptr<IRenderer>;

class IRendererFactory : public IObject {
public:
  using RendererCreator = std::function<IRendererPtr()>;

  virtual IRendererPtr create(const std::string &name) = 0;
  virtual void registerRenderer(const std::string &name,
                                const RendererCreator &creator) = 0;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_IRENDERER_H
