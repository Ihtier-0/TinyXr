#ifndef TINYXR_CORE_RENDERCONTEXT_H
#define TINYXR_CORE_RENDERCONTEXT_H

#include "tinyxr/core/matrix4x4.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

struct Viewport {
  int x;
  int y;
  int width;
  int height;
};

struct RenderContext {
  Viewport viewport;
  Matrix4x4 projection;
  Matrix4x4 view;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_RENDER_CONTEXT_H
