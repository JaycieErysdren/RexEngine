## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Project definition
project(Decker LANGUAGES C CXX)

## Build type
set(CMAKE_BUILD_TYPE Release)

set(DECKER_VERSION "1.7")

set(LILT_SRC
	lilt.c
)

set(LILT lilt)

add_executable(${LILT} ${LILT_SRC})
target_compile_definitions(${LILT} PUBLIC "VERSION=\"${DECKER_VERSION}\"" "timegm=time")
target_include_directories(${LILT} PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(${LILT} PUBLIC m)
set_target_properties(${LILT}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)

set(DECKER_SRC
	decker.c
	dos/decker_dos.c
)

set(DECKER decker)

add_executable(${DECKER} ${DECKER_SRC})
target_compile_definitions(${DECKER} PUBLIC "VERSION=\"${DECKER_VERSION}\"" "timegm=time")
target_link_libraries(${DECKER} PUBLIC m)
target_include_directories(${DECKER} PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(${DECKER} PUBLIC dos)
set_target_properties(${DECKER}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)
