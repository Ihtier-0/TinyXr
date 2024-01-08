#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <memory>

#include "tinyxr/core/irenderer.h"

class OpenGlRenderer : public TXR_NS::IRenderer {
public:
  bool render(const RenderContext& context);
};

#endif  // OPENGL_RENDERER_H
