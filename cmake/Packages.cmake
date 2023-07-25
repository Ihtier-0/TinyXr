Set(FETCHCONTENT_QUIET FALSE)

CPMAddPackage(
  NAME OpenXR
  VERSION 1.1.43
  GITHUB_REPOSITORY KhronosGroup/OpenXR-SDK-Source
  GIT_TAG release-1.1.43
  OPTIONS "BUILD_TESTS OFF"
          "BUILD_CONFORMANCE_TESTS OFF"
          "DYNAMIC_LOADER ON"
          # workaround for dynamic CRT linkage
          "JSONCPP_STATIC_WINDOWS_RUNTIME OFF"
          "CMAKE_SYSTEM_NAME WindowsStore")
find_targets(OpenXR_targets ${OpenXR_SOURCE_DIR})
make_folder("OpenXR" ${OpenXR_targets})

CPMAddPackage(
  NAME tomlplusplus
  VERSION 3.4.0
  GITHUB_REPOSITORY marzer/tomlplusplus)
find_targets(tomlplusplus_targets ${tomlplusplus_SOURCE_DIR})
make_folder("tomlplusplus" ${tomlplusplus_targets})

CPMAddPackage(
  NAME jsoncpp
  VERSION 1.9.6
  GITHUB_REPOSITORY open-source-parsers/jsoncpp
  GIT_TAG 1.9.6
  OPTIONS "JSONCPP_WITH_TESTS OFF"
          "JSONCPP_WITH_POST_BUILD_UNITTEST OFF"
          "JSONCPP_WITH_EXAMPLE OFF"
          "JSONCPP_WITH_WARNING_AS_ERROR ON"
          "JSONCPP_WITH_STRICT_ISO OFF"
          "JSONCPP_WITH_PKGCONFIG_SUPPORT OFF"
          "JSONCPP_WITH_CMAKE_PACKAGE OFF"
          "BUILD_SHARED_LIBS ON"
          "BUILD_STATIC_LIBS OFF"
          "BUILD_OBJECT_LIBS OFF")
find_targets(jsoncpp_targets ${jsoncpp_SOURCE_DIR})
make_folder("jsoncpp" ${jsoncpp_targets})

CPMAddPackage(
  NAME glfw
  VERSION 3.4
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.4
  OPTIONS "BUILD_SHARED_LIBS ON"
          "GLFW_BUILD_EXAMPLES OFF"
          "GLFW_BUILD_TESTS OFF"
          "GLFW_BUILD_DOCS OFF")
find_targets(glfw_targets ${glfw_SOURCE_DIR})
make_folder("glfw" ${glfw_targets})

CPMAddPackage(
  NAME glad
  GITHUB_REPOSITORY Dav1dde/glad
  VERSION 2.0.8
  DOWNLOAD_ONLY)
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${glad_SOURCE_DIR}/cmake)
include(GladConfig)
add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
set(GLAD_LIBRARY glad_gl_core_46)
# https://github.com/Dav1dde/glad/wiki/C#generating-during-build-process
glad_add_library(${GLAD_LIBRARY} SHARED API gl:core=4.6)
make_folder("glad" ${GLAD_LIBRARY})
install(TARGETS ${GLAD_LIBRARY}
  EXPORT ${GLAD_LIBRARY}-targets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)

find_package(OpenGL REQUIRED)
