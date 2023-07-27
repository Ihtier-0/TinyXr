set(core_headers
  core/api.h
  core/export.h
  core/manager.h
  core/os.h
)

set(core_source
  core/manager.cpp
)

source_group("core" FILES ${core_headers} ${core_source})

set(impl_headers
  impl/manager.h
  impl/openxr.h
  impl/platform.h
)

set(impl_source
  impl/manager.cpp
)

source_group("impl" FILES ${impl_headers} ${impl_source})

set(headers
  ${core_headers}
  ${impl_headers}
)

set(source
  ${core_source}
  ${impl_source}
)

add_library(TinyXr SHARED
  ${headers}
  ${source}
)

set_property(TARGET TinyXr PROPERTY CXX_STANDARD 17)
set_property(TARGET TinyXr PROPERTY CXX_STANDARD_REQUIRED True)
set_property(TARGET TinyXr PROPERTY CXX_EXTENSIONS OFF)

target_compile_definitions(TinyXr PRIVATE -DTINYXR_EXPORT)

target_include_directories(
  TinyXr
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src/lib
  ${OPENXR_INCLUDE_DIR}
)

target_include_directories(
  TinyXr
  PRIVATE
  ${OPENXR_INCLUDE_DIR}
)

target_link_libraries(
  TinyXr
  PRIVATE
  ${OPENXR_LIBRARY}
)

install(FILES ${core_headers} DESTINATION include/TinyXr/core)
install(FILES ${impl_headers} DESTINATION include/TinyXr/impl)

install(TARGETS TinyXr
  EXPORT TinyXr-targets
  RUNTIME DESTINATION bin/TinyXr
  ARCHIVE DESTINATION lib/TinyXr
  LIBRARY DESTINATION lib/TinyXr)
