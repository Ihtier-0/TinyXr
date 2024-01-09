#ifndef TINYXR_CORE_IRENDERER_H
#define TINYXR_CORE_IRENDERER_H

#include <memory>

#include "tinyxr/core/api.h"
#include "tinyxr/core/render_context.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

class TINYXR_API IRenderer {
public:
  IRenderer();
  virtual ~IRenderer();

  virtual bool render(const RenderContext& context) = 0;
};
using IRendererPtr = std::shared_ptr<IRenderer>;

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_IRENDERER_H
