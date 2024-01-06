#
# In:
#   JSONCPP_ROOT
#
# Out:
#   jsoncpp_FOUND
#   JSONCPP_LIBRARY
#   JSONCPP_INCLUDE_DIR

find_library(
  JSONCPP_LIBRARY
  NAMES jsoncpp
  PATHS ${JSONCPP_ROOT}/lib)

find_path(
  JSONCPP_INCLUDE_DIR json/json.h
  PATHS ${JSONCPP_ROOT}/include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  jsoncpp
  DEFAULT_MSG
  JSONCPP_LIBRARY
  JSONCPP_LIBRARY)