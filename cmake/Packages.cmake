CPMAddPackage(
  NAME OpenXR-SDK-Source
  VERSION 1.0.33
  GITHUB_REPOSITORY KhronosGroup/OpenXR-SDK-Source
  GIT_TAG release-1.0.33
  OPTIONS "BUILD_WITH_SYSTEM_JSONCPP OFF"
)

CPMAddPackage(
  NAME cpptoml
  VERSION 0.1.1
  GITHUB_REPOSITORY skystrife/cpptoml
)

CPMAddPackage(
  NAME jsoncpp
  VERSION 1.9.5
  GITHUB_REPOSITORY open-source-parsers/jsoncpp
  GIT_TAG 1.9.5
)

CPMAddPackage(
  NAME glfw
  VERSION 3.3.9
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.3.9
)

find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)