#
# In:
#   OPENXR_ROOT
#
# Out:
#   OpenXR_FOUND
#   OPENXR_LIBRARY
#   OPENXR_INCLUDE_DIR

find_library(
  OPENXR_LIBRARY
  NAMES openxr_loader
  PATHS ${OPENXR_ROOT}/lib)

find_path(
  OPENXR_INCLUDE_DIR openxr/openxr.h
  PATHS ${OPENXR_ROOT}/include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  OpenXR
  DEFAULT_MSG
  OPENXR_LIBRARY
  OPENXR_INCLUDE_DIR)