#
# In:
#   CPPTOML_ROOT 
#
# Out:
#   CPPTOML_FOUND
#   CPPTOML_INCLUDE_DIR

find_path(
  CPPTOML_INCLUDE_DIR cpptoml.h
  PATHS ${CPPTOML_ROOT}/include)

if(CPPTOML_INCLUDE_DIR STREQUAL "CPPTOML_INCLUDE_DIR-NOTFOUND")
  set(CPPTOML_FOUND FALSE)
else()
  set(CPPTOML_FOUND TRUE)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  cpptoml
  DEFAULT_MSG
  CPPTOML_INCLUDE_DIR)