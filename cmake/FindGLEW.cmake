#
# In:
#   GLEW_ROOT
#
# Out:
#   GLEW_FOUND
#   GLEW_LIBRARY
#   GLEW_INCLUDE_DIR

find_library(
  GLEW_LIBRARY
  NAMES glew32
  PATHS ${GLEW_ROOT}/lib)

find_path(
  GLEW_INCLUDE_DIR GL/glew.h
  PATHS ${GLEW_ROOT}/include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  GLEW
  DEFAULT_MSG
  GLEW_LIBRARY
  GLEW_INCLUDE_DIR)