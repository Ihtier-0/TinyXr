set(source
  main.cpp)

add_executable(tinyxrexample
  ${source})

set_property(TARGET tinyxrexample PROPERTY CXX_STANDARD 17)
set_property(TARGET tinyxrexample PROPERTY CXX_STANDARD_REQUIRED True)
set_property(TARGET tinyxrexample PROPERTY CXX_EXTENSIONS OFF)

target_include_directories(tinyxrexample
  PUBLIC
  ${PROJECT_SOURCE_DIR})

target_link_libraries(tinyxrexample
	tinyxr)

install(TARGETS tinyxrexample
  EXPORT tinyxrexample-targets
  RUNTIME DESTINATION bin/tinyxrexample
  ARCHIVE DESTINATION lib/tinyxrexample
  LIBRARY DESTINATION lib/tinyxrexample)
