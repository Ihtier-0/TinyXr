cmake_minimum_required(VERSION 3.0)

project(TinyXr VERSION 0.1.0.0 LANGUAGES CXX)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tinyxr.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tinyxr.h
)

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(TinyXr SHARED ${SRCS} ${HDRS})
target_include_directories(
  TinyXr
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_property(TARGET TinyXr PROPERTY CXX_STANDARD 17)
set_property(TARGET TinyXr PROPERTY CXX_STANDARD_REQUIRED True)
set_property(TARGET TinyXr PROPERTY CXX_EXTENSIONS OFF)

target_compile_options(
  TinyXr
  PRIVATE
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
# -Werror

  -Wall
  -Wextra
  -Wpedantic

  -Wcast-align
  -Wcast-qual
  -Wconversion
  -Wenum-compare
  -Wfloat-equal
  -Wredundant-decls
  -Wsign-conversion
# https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html
  -Wctor-dtor-privacy
  -Wnon-virtual-dtor
#  -Wold-style-cast // disabled for XR_VERSION_MAJOR
  -Woverloaded-virtual
#  -Wsign-promo // disabled for XR_VERSION_MAJOR
)

find_package(Qt5 REQUIRED COMPONENTS Gui OpenGL)

target_link_libraries(
  TinyXr
  PUBLIC
  openxr_loader
  cpptoml
  Qt5::Gui # for QOpenGLContext
  Qt5::OpenGL # for QWGLNativeContext
  # TODO! Remove Opengl32
  Opengl32 # for wglGetCurrentDC & wglGetCurrentContext
)

add_subdirectory(lib)
add_subdirectory(examples)
