set(source
  main.cpp
  openglrenderer.cpp
  openglrenderer.h
)

add_executable(tinyxrexample
  ${source})

if(MSVC)
  install(FILES $<TARGET_PDB_FILE:tinyxrexample> DESTINATION ${CMAKE_INSTALL_PREFIX}/bin OPTIONAL)
  set_property(TARGET tinyxrexample PROPERTY VS_DEBUGGER_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/$<TARGET_FILE_NAME:tinyxrexample>)
  set_property(TARGET tinyxrexample PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
endif()

target_include_directories(tinyxrexample
  PUBLIC
  ${PROJECT_SOURCE_DIR})

target_link_libraries(tinyxrexample
  tinyxr
  glfw
  ${GLAD_LIBRARY})

install(TARGETS tinyxrexample
  EXPORT tinyxrexample-targets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)
