set(core_headers
  core/api.h
  core/config.h
  core/export.h
  core/manager.h
  core/os.h)

set(core_source
  core/config.cpp
  core/manager.cpp)

source_group("core" FILES ${core_headers} ${core_source})

set(impl_headers
  impl/context.h
  impl/favor_high_performance_gpu.h
  impl/manager.h
  impl/openxr_extensions.h
  impl/openxr_definitions.h
  impl/openxr_utils.h
  impl/openxr.h
  impl/utils.h)

set(impl_source
  impl/manager.cpp
  impl/openxr_extensions.cpp
  impl/openxr_utils.cpp
  impl/utils.cpp)

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

target_compile_definitions(
  tinyxr
  PRIVATE
  -DTINYXR_EXPORT
  -DTINYXR_USE_GRAPHICS_API_OPENGL)

target_include_directories(
  tinyxr
  PUBLIC
  ${PROJECT_SOURCE_DIR}
  ${CPPTOML_INCLUDE_DIR})

target_include_directories(
  tinyxr
  PRIVATE
  ${OPENXR_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR})

target_link_libraries(
  tinyxr
  PRIVATE
  ${OPENXR_LIBRARY}
  ${GLFW_LIBRARY}
  ${GLEW_LIBRARY}
  OpenGL::GL)

install(FILES ${core_headers} DESTINATION include/tinyxr/core)

install(TARGETS tinyxr
  EXPORT tinyxr-targets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)