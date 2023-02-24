cmake_minimum_required(VERSION 3.0)

project(TinyXr VERSION 0.1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE HDRS src/*.h)

add_executable(TinyXr ${SRCS} ${HDRS})

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

target_link_libraries(TinyXr PRIVATE openxr_loader)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
target_link_libraries(TinyXr PRIVATE Qt5::Widgets)

add_subdirectory(lib)
