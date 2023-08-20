#
# In:
#   GLFW_ROOT 
#
# Out:
#   glfw_FOUND
#   GLFW_LIBRARY
#   GLFW_INCLUDE_DIR

find_library(
  GLFW_LIBRARY
  NAMES glfw3
  PATHS ${GLFW_ROOT}/lib)

find_path(
  GLFW_INCLUDE_DIR GLFW/glfw3.h
  PATHS ${GLFW_ROOT}/include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  glfw
  DEFAULT_MSG
  GLFW_LIBRARY
  GLFW_INCLUDE_DIR)