file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(TinyXrTest ${SRCS} ${HDRS})

set_property(TARGET TinyXrTest PROPERTY CXX_STANDARD 17)
set_property(TARGET TinyXrTest PROPERTY CXX_STANDARD_REQUIRED True)
set_property(TARGET TinyXrTest PROPERTY CXX_EXTENSIONS OFF)

file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.toml
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(
  TinyXrTest
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

find_package(Qt5 REQUIRED COMPONENTS Widgets)

target_link_libraries(
  TinyXrTest
  PRIVATE
  TinyXr
  Qt5::Widgets # for a QOffscreenSurface, QApplication is needed
)
