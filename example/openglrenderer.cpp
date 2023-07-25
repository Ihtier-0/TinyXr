#include "openglrenderer.h"

#include <glad/gl.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
// GLFW (include after glad/gl.h)
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <algorithm>
#include <cassert>
#include <iostream>

static std::string swapchainFormatToString(int64_t swapchainFormat) {
  switch (swapchainFormat) {
  case GL_R8:
    return "GL_R8";
  case GL_R8_SNORM:
    return "GL_R8_SNORM";
  case GL_R16:
    return "GL_R16";
  case GL_R16_SNORM:
    return "GL_R16_SNORM";
  case GL_RG8:
    return "GL_RG8";
  case GL_RG8_SNORM:
    return "GL_RG8_SNORM";
  case GL_RG16:
    return "GL_RG16";
  case GL_RG16_SNORM:
    return "GL_RG16_SNORM";
  case GL_R3_G3_B2:
    return "GL_R3_G3_B2";
  case GL_RGB4:
    return "GL_RGB4";
  case GL_RGB5:
    return "GL_RGB5";
  case GL_RGB8:
    return "GL_RGB8";
  case GL_RGB8_SNORM:
    return "GL_RGB8_SNORM";
  case GL_RGB10:
    return "GL_RGB10";
  case GL_RGB12:
    return "GL_RGB12";
  case GL_RGB16_SNORM:
    return "GL_RGB16_SNORM";
  case GL_RGBA2:
    return "GL_RGBA2";
  case GL_RGBA4:
    return "GL_RGBA4";
  case GL_RGB5_A1:
    return "GL_RGB5_A1";
  case GL_RGBA8:
    return "GL_RGBA8";
  case GL_RGBA8_SNORM:
    return "GL_RGBA8_SNORM";
  case GL_RGB10_A2:
    return "GL_RGB10_A2";
  case GL_RGB10_A2UI:
    return "GL_RGB10_A2UI";
  case GL_RGBA12:
    return "GL_RGBA12";
  case GL_RGBA16:
    return "GL_RGBA16";
  case GL_SRGB8:
    return "GL_SRGB8";
  case GL_SRGB8_ALPHA8:
    return "GL_SRGB8_ALPHA8";
  case GL_R16F:
    return "GL_R16F";
  case GL_RG16F:
    return "GL_RG16F";
  case GL_RGB16F:
    return "GL_RGB16F";
  case GL_RGBA16F:
    return "GL_RGBA16F";
  case GL_R32F:
    return "GL_R32F";
  case GL_RG32F:
    return "GL_RG32F";
  case GL_RGB32F:
    return "GL_RGB32F";
  case GL_RGBA32F:
    return "GL_RGBA32F";
  case GL_R11F_G11F_B10F:
    return "GL_R11F_G11F_B10F";
  case GL_RGB9_E5:
    return "GL_RGB9_E5";
  case GL_R8I:
    return "GL_R8I";
  case GL_R8UI:
    return "GL_R8UI";
  case GL_R16I:
    return "GL_R16I";
  case GL_R16UI:
    return "GL_R16UI";
  case GL_R32I:
    return "GL_R32I";
  case GL_R32UI:
    return "GL_R32UI";
  case GL_RG8I:
    return "GL_RG8I";
  case GL_RG8UI:
    return "GL_RG8UI";
  case GL_RG16I:
    return "GL_RG16I";
  case GL_RG16UI:
    return "GL_RG16UI";
  case GL_RG32I:
    return "GL_RG32I";
  case GL_RG32UI:
    return "GL_RG32UI";
  case GL_RGB8I:
    return "GL_RGB8I";
  case GL_RGB8UI:
    return "GL_RGB8UI";
  case GL_RGB16I:
    return "GL_RGB16I";
  case GL_RGB16UI:
    return "GL_RGB16UI";
  case GL_RGB32I:
    return "GL_RGB32I";
  case GL_RGB32UI:
    return "GL_RGB32UI";
  case GL_RGBA8I:
    return "GL_RGBA8I";
  case GL_RGBA8UI:
    return "GL_RGBA8UI";
  case GL_RGBA16I:
    return "GL_RGBA16I";
  case GL_RGBA16UI:
    return "GL_RGBA16UI";
  case GL_RGBA32I:
    return "GL_RGBA32I";
  case GL_RGBA32UI:
    return "GL_RGBA32UI";
  default:
    return "UNDEFINE";
  }
}

static int positionLocation = 0;
static int viewLocation = 1;
static int projectionLocation = 2;

static const char* vertexShaderSource = R"_(
#version 460 core

layout(location = 0) in vec3 position;

layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;

void main()
{
  gl_Position = projection * view * vec4(position.x, position.y, position.z, 1.0);
}
)_";

static const char* fragmentShaderSource = R"_(
#version 460 core

out vec4 color;

void main()
{
  color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)_";

static void checkShader(GLuint shader) {
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    assert(infoLog);
  }
}

static void checkProgram(GLuint program) {
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar infoLog[512];
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    assert(infoLog);
  }
}

std::string OpenGlRenderer::name() { return "OpenGlRenderer"; }

OpenGlRenderer::OpenGlRenderer() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Offscreen contexts
  // (https://www.glfw.org/docs/latest/context.html#context_offscreen)
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  mWindow = glfwCreateWindow(100, 100, "TinyXrExample", NULL, NULL);
  glfwMakeContextCurrent(mWindow);
  gladLoadGL((GLADloadfunc)glfwGetProcAddress);

  glGenFramebuffers(1, &m_fbo);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);
  checkShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);
  checkShader(fragmentShader);

  m_program = glCreateProgram();
  glAttachShader(m_program, vertexShader);
  glAttachShader(m_program, fragmentShader);
  glLinkProgram(m_program);
  checkProgram(m_program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLfloat vertices[] = {
      // Top
      0.5f,
      0.5f,
      0.0f,
      // Bottom Right
      0.5f,
      -0.5f,
      0.0f,
      // Bottom Left
      -0.5f,
      -0.5f,
      0.0f,
      // Top Left
      -0.5f,
      0.5f,
      0.0f,
  };
  GLuint indices[] = {
      // 1
      0,
      1,
      3,
      // 2
      1,
      2,
      3,
  };
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);
  glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE,
                        3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(positionLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glfwMakeContextCurrent(nullptr);
}

OpenGlRenderer::~OpenGlRenderer() { glfwDestroyWindow(mWindow); }

TXR_NS::Version OpenGlRenderer::getVersion() const {
  glfwMakeContextCurrent(mWindow);
  GLint major = 0;
  GLint minor = 0;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  glfwMakeContextCurrent(nullptr);
  return {major, minor, 0};
}

void* OpenGlRenderer::getDeviceContext() const {
  return GetDC(glfwGetWin32Window(mWindow));
}

void* OpenGlRenderer::getHandleRenderContext() const {
  return glfwGetWGLContext(mWindow);
}

int64_t OpenGlRenderer::selectSwapchainFormat(
    const std::vector<int64_t>& swapchainFormats) const {
  static const int64_t supportedColorSwapchainFormats[] = {
      GL_RGBA16F,
      // VRCompositorError_TextureUsesUnsupportedFormat with OpenVR.
      // GL_RGBA16,
      GL_RGB16F,
  };

  auto swapchainFormatIt =
      std::find_first_of(swapchainFormats.begin(), swapchainFormats.end(),
                         std::begin(supportedColorSwapchainFormats),
                         std::end(supportedColorSwapchainFormats));
  const auto selectedFormat = swapchainFormatIt == swapchainFormats.end()
                                  ? swapchainFormats[0]
                                  : *swapchainFormatIt;

  logSwapchainFormats(selectedFormat, swapchainFormats);

  return selectedFormat;
}

bool OpenGlRenderer::render(const TXR_NS::RenderContext& renderContext,
                            const uint32_t image) {
  glfwMakeContextCurrent(mWindow);

  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         image, 0);

  glViewport(static_cast<GLint>(renderContext.viewport.x),
             static_cast<GLint>(renderContext.viewport.y),
             static_cast<GLsizei>(renderContext.viewport.width),
             static_cast<GLsizei>(renderContext.viewport.height));
  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(m_program);
  glBindVertexArray(m_vao);
  glUniformMatrix4fv(viewLocation, 1, GL_TRUE, renderContext.view.data());
  glUniformMatrix4fv(projectionLocation, 1, GL_TRUE,
                     renderContext.projection.data());
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glfwMakeContextCurrent(nullptr);

  return true;
}

void OpenGlRenderer::logSwapchainFormats(
    int64_t selectedSwapchainFormat,
    const std::vector<int64_t>& swapchainFormats) const {
  std::cout << "Available Swapchain Formats: (" << swapchainFormats.size()
            << ")" << std::endl;
  for (const auto swapchainFormat : swapchainFormats) {
    std::cout << "  " << swapchainFormatToString(swapchainFormat) << ' '
              << swapchainFormat
              << (swapchainFormat == selectedSwapchainFormat ? " [selected]"
                                                             : "")
              << std::endl;
  }
}
