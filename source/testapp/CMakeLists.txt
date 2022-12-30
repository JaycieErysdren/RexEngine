## ========================================================
##
## FILE:			/source/testapp/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		TestApp CMake project definition
##
## LAST EDITED:		December 30th, 2022
##
## ========================================================

## Definitions
cmake_minimum_required(VERSION 3.13)
project(TestApp LANGUAGES C CXX)
set(CMAKE_BUILD_TYPE Release)

## Source
set(TESTAPP_SRC
	testapp.cpp
)

set(TESTAPP test${REX_EXECUTABLE_SUFFIX})

## Executable
add_executable(${TESTAPP} ${TESTAPP_SRC})
target_link_libraries(${TESTAPP} rex)
target_include_directories(${TESTAPP} PUBLIC ${CMAKE_SOURCE_DIR}/source)
set_target_properties(${TESTAPP}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)
