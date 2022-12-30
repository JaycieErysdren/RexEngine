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

## Executable
add_executable(testapp ${TESTAPP_SRC})
target_link_libraries(testapp rex)
target_include_directories(testapp PUBLIC ${CMAKE_SOURCE_DIR}/source)
set_target_properties(testapp
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)
