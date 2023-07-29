set(core_headers
  core/api.h
  core/export.h
  core/manager.h
  core/os.h)

set(core_source
  core/manager.cpp)

source_group("core" FILES ${core_headers} ${core_source})

set(impl_headers
  impl/manager.h
  impl/openxr.h
  impl/platform.h)

set(impl_source
  impl/manager.cpp)

source_group("impl" FILES ${impl_headers} ${impl_source})

set(headers
  ${core_headers}
  ${impl_headers})

set(source
  ${core_source}
  ${impl_source})

add_library(tinyxr SHARED
  ${headers}
  ${source})

set_property(TARGET tinyxr PROPERTY CXX_STANDARD 17)
set_property(TARGET tinyxr PROPERTY CXX_STANDARD_REQUIRED True)
set_property(TARGET tinyxr PROPERTY CXX_EXTENSIONS OFF)

target_compile_definitions(tinyxr PRIVATE -DTINYXR_EXPORT)

target_include_directories(
  tinyxr
  PUBLIC
  ${PROJECT_SOURCE_DIR})

target_include_directories(
  tinyxr
  PRIVATE
  ${OPENXR_INCLUDE_DIR})

target_link_libraries(
  tinyxr
  PRIVATE
  ${OPENXR_LIBRARY})

install(FILES ${core_headers} DESTINATION include/tinyxr/core)
install(FILES ${impl_headers} DESTINATION include/tinyxr/impl)

install(TARGETS tinyxr
  EXPORT tinyxr-targets
  RUNTIME DESTINATION bin/tinyxr
  ARCHIVE DESTINATION lib/tinyxr
  LIBRARY DESTINATION lib/tinyxr)
