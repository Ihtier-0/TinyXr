#
# In:
#   OPENXR_ROOT 
#
# Out:
#   OPENXR_FOUND
#   OPENXR_LIBRARY
#   OPENXR_INCLUDE_DIR

find_library(
  OPENXR_LIBRARY
  NAMES openxr_loader
  PATHS ${OPENXR_ROOT}/lib)

find_path(
  OPENXR_INCLUDE_DIR openxr/openxr.h
  PATHS ${OPENXR_ROOT}/include)

if(OPENXR_LIBRARY STREQUAL "OPENXR_LIBRARY-NOTFOUND" OR
   OPENXR_INCLUDE_DIR STREQUAL "OPENXR_INCLUDE_DIR-NOTFOUND")
  set(OPENXR_FOUND FALSE)
else()
  set(OPENXR_FOUND TRUE)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  OpenXR
  DEFAULT_MSG
  OPENXR_LIBRARY
  OPENXR_INCLUDE_DIR)