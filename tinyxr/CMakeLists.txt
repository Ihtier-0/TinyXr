set(core_headers
  core/api.h
  core/config.h
  core/export.h
  core/irenderer.h
  core/manager.h
  core/os.h
  core/render_context.h)

set(core_sources
  core/config.cpp
  core/irenderer.cpp
  core/manager.cpp)

source_group("core/headers" FILES ${core_headers})
source_group("core/sources" FILES ${core_sources})

set(impl_headers
  impl/action_set.h
  impl/action.h
  impl/config.h
  impl/context.h
  impl/favor_high_performance_gpu.h
  impl/internal_render_context.h
  impl/manager.h
  impl/openxr_extensions.h
  impl/openxr_definitions.h
  impl/openxr_math.h
  impl/openxr_utils.h
  impl/openxr.h
  impl/utils.h)

set(impl_sources
  impl/action.cpp
  impl/config.cpp
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

target_compile_definitions(
  tinyxr
  PRIVATE
  -DTINYXR_EXPORT
  -DTINYXR_USE_GRAPHICS_API_OPENGL)

target_include_directories(
  tinyxr
  PUBLIC
  ${PROJECT_SOURCE_DIR})

target_include_directories(
  tinyxr
  PRIVATE
  ${GLEW_INCLUDE_DIR})

target_link_libraries(
  tinyxr
  PRIVATE
  OpenXR::openxr_loader
  cpptoml
  jsoncpp_lib
  glfw
  ${GLAD_LIBRARY}
  OpenGL::GL)

install(FILES ${core_headers} DESTINATION include/tinyxr/core)

install(TARGETS tinyxr
  EXPORT tinyxr-targets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)
