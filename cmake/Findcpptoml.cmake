#
# In:
#   CPPTOML_ROOT 
#
# Out:
#   cpptoml_FOUND
#   CPPTOML_INCLUDE_DIR

find_path(
  CPPTOML_INCLUDE_DIR cpptoml.h
  PATHS ${CPPTOML_ROOT}/include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  cpptoml
  DEFAULT_MSG
  CPPTOML_INCLUDE_DIR)