cmake_minimum_required (VERSION 3.18)

project(TinyXr VERSION 0.0.1.0
  LANGUAGES CXX
  HOMEPAGE_URL https://github.com/Ihtier-0/TinyXr)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_SOURCE_DIR}/cmake)

include(Version)
include(Packages)

add_subdirectory(tinyxr)
add_subdirectory(example)
