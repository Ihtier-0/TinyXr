set(source
  main.cpp
)

add_executable(TinyXrExample
  ${source}
)

set_property(TARGET TinyXrExample PROPERTY CXX_STANDARD 17)
set_property(TARGET TinyXrExample PROPERTY CXX_STANDARD_REQUIRED True)
set_property(TARGET TinyXrExample PROPERTY CXX_EXTENSIONS OFF)

target_include_directories(TinyXrExample
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src/lib
)

target_link_libraries(TinyXrExample
	TinyXr)

install(TARGETS TinyXrExample
  EXPORT TinyXrExample-targets
  RUNTIME DESTINATION bin/TinyXrExample
  ARCHIVE DESTINATION lib/TinyXrExample
  LIBRARY DESTINATION lib/TinyXrExample)
