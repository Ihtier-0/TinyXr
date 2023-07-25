math(EXPR
  TINYXR_VERSION
  "${PROJECT_VERSION_MAJOR} * 10000 + ${PROJECT_VERSION_MINOR} * 100 + ${PROJECT_VERSION_PATCH}")
set(TINYXR_VERSION_STRING
  "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")

configure_file(
  ${PROJECT_SOURCE_DIR}/tinyxr/core/tinyxr.h.in
  ${PROJECT_BINARY_DIR}/tinyxr/core/tinyxr.h)
include_directories(${PROJECT_BINARY_DIR})
install(
  FILES ${PROJECT_BINARY_DIR}/tinyxr/core/tinyxr.h
  DESTINATION include/tinyxr/core)