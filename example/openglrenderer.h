#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <memory>

#include "tinyxr/core/irenderer.h"

struct GLFWwindow;

class OpenGlRenderer : public TXR_NS::IRenderer {
public:
  static std::string name();

  OpenGlRenderer();
  ~OpenGlRenderer() override;

  TXR_NS::Version getVersion() const override;

  void *getDeviceContext() const override;
  void *getHandleRenderContext() const override;

  int64_t selectSwapchainFormat(
      const std::vector<int64_t> &swapchainFormats) const override;

  bool render(const TXR_NS::RenderContext &renderContext,
              const uint32_t image) override;

private:
  void logSwapchainFormats(int64_t selectedSwapchainFormat,
                           const std::vector<int64_t> &swapchainFormats) const;

  GLFWwindow *mWindow = nullptr;
  unsigned int m_fbo = -1;
  unsigned int m_program = -1;
  unsigned int m_vbo = -1;
  unsigned int m_vao = -1;
  unsigned int m_ebo = -1;
};

#endif  // OPENGL_RENDERER_H
