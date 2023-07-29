set(source
  main.cpp)

add_executable(tinyxrexample
  ${source})

set_property(TARGET tinyxrexample PROPERTY CXX_STANDARD 17)
set_property(TARGET tinyxrexample PROPERTY CXX_STANDARD_REQUIRED True)
set_property(TARGET tinyxrexample PROPERTY CXX_EXTENSIONS OFF)

if(MSVC)
  set_property(TARGET tinyxrexample PROPERTY VS_DEBUGGER_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/$<TARGET_FILE_NAME:tinyxrexample>)
  set_property(TARGET tinyxrexample PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
endif()

target_include_directories(tinyxrexample
  PUBLIC
  ${PROJECT_SOURCE_DIR})

target_link_libraries(tinyxrexample
	tinyxr)

install(TARGETS tinyxrexample
  EXPORT tinyxrexample-targets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)
