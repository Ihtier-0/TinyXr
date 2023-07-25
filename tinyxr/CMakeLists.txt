set(core_headers
  ${PROJECT_BINARY_DIR}/tinyxr/core/tinyxr.h

  core/api.h
  core/iconfigxr.h
  core/iconfigxrconstants.h
  core/export.h
  core/favorhighperformancegpu.h
  core/irenderer.h
  core/matrix4x4.h
  core/imanagerxr.h
  core/iobject.h
  core/os.h
  core/rendercontext.h
)

set(core_sources
  core/iobject.cpp
  core/matrix4x4.cpp
)

source_group("core/headers" FILES ${core_headers})
source_group("core/sources" FILES ${core_sources})

set(impl_headers
  impl/actionset.h
  impl/action.h
  impl/configxrfactoryimpl.h
  impl/contextxr.h
  impl/managerxrfactoryimpl.h
  impl/openxrextensions.h
  impl/openxrdefinitions.h
  impl/openxrmath.h
  impl/openxrutils.h
  impl/rendererfactoryimpl.h
  impl/openxr.h
  impl/stringutils.h
  impl/swapchain.h
)

set(impl_sources
  impl/configxrfactoryimpl.cpp
  impl/managerxrfactoryimpl.cpp
  impl/openxrextensions.cpp
  impl/openxrmath.cpp
  impl/openxrutils.cpp
  impl/rendererfactoryimpl.cpp
  impl/stringutils.cpp
)

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

target_link_libraries(
  tinyxr
  PRIVATE
  OpenXR::openxr_loader
  tomlplusplus::tomlplusplus
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
