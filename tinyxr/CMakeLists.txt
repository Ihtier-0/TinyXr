set(core_headers
  core/api.h
  core/config.h
  core/export.h
  core/manager.h
  core/os.h)

set(core_sources
  core/config.cpp
  core/manager.cpp)

source_group("core/headers" FILES ${core_headers})
source_group("core/sources" FILES ${core_sources})

set(impl_headers
  impl/action_set.h
  impl/action.h
  impl/context.h
  impl/favor_high_performance_gpu.h
  impl/manager.h
  impl/openxr_extensions.h
  impl/openxr_definitions.h
  impl/openxr_math.h
  impl/openxr_utils.h
  impl/openxr.h
  impl/utils.h)

set(impl_sources
  impl/action_set.cpp
  impl/action.cpp
  impl/manager.cpp
  impl/openxr_extensions.cpp
  impl/openxr_math.cpp
  impl/openxr_utils.cpp
  impl/utils.cpp)

source_group("impl/headers" FILES ${impl_headers})
source_group("impl/sources" FILES ${impl_sources})

set(headers ${core_headers} ${impl_headers})

set(sources ${core_sources} ${impl_sources})

add_library(tinyxr SHARED ${headers} ${sources})

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
  ${GLFW_INCLUDE_DIR}
  ${JSONCPP_INCLUDE_DIR})

target_link_libraries(
  tinyxr
  PRIVATE
  ${OPENXR_LIBRARY}
  ${GLFW_LIBRARY}
  ${GLEW_LIBRARY}
  ${JSONCPP_LIBRARY}
  OpenGL::GL)

install(FILES ${core_headers} DESTINATION include/tinyxr/core)

install(TARGETS tinyxr
  EXPORT tinyxr-targets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)