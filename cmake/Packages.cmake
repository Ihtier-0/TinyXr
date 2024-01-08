CPMAddPackage(
  NAME OpenXR-SDK-Source
  VERSION 1.0.33
  GITHUB_REPOSITORY KhronosGroup/OpenXR-SDK-Source
  GIT_TAG release-1.0.33
  OPTIONS "BUILD_API_LAYERS OFF"
          "BUILD_TESTS OFF"
          "BUILD_CONFORMANCE_TESTS OFF"
)

CPMAddPackage(
  NAME cpptoml
  VERSION 0.1.1
  GITHUB_REPOSITORY skystrife/cpptoml
  OPTIONS "CPPTOML_BUILD_EXAMPLES OFF"
)

CPMAddPackage(
  NAME jsoncpp
  VERSION 1.9.5
  GITHUB_REPOSITORY open-source-parsers/jsoncpp
  GIT_TAG 1.9.5
  OPTIONS "JSONCPP_WITH_TESTS OFF"
          "JSONCPP_WITH_POST_BUILD_UNITTEST OFF"
          "JSONCPP_WITH_EXAMPLE OFF"
          "BUILD_STATIC_LIBS OFF"
          "BUILD_OBJECT_LIBS OFF"
)

CPMAddPackage(
  NAME glfw
  VERSION 3.3.9
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.3.9
  OPTIONS "GLFW_BUILD_EXAMPLES OFF"
          "GLFW_BUILD_TESTS OFF"
          "GLFW_BUILD_DOCS OFF"
)

find_package(GLEW REQUIRED) # 2.2.0

find_package(OpenGL REQUIRED)
