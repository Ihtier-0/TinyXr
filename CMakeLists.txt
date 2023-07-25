cmake_minimum_required (VERSION 3.31)

project(TinyXr
  VERSION 0.0.1.0
  LANGUAGES CXX
  HOMEPAGE_URL https://github.com/Ihtier-0/TinyXr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_SOURCE_DIR}/cmake)

include(Config)
include(FindTargets)
include(get_cpm)
include(MakeFolder)
include(Packages)
include(Version)

add_subdirectory(tinyxr)
add_subdirectory(example)
