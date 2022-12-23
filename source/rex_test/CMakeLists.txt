## ========================================================
##
## FILE:			/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		RexTest CMake project definition
##
## LAST EDITED:		December 23rd, 2022
##
## ========================================================

##
## Project definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Project definition
project(RexTest LANGUAGES C CXX)

## Build type
set(CMAKE_BUILD_TYPE Release)

##
## Project source
##

set(REX_TEST_SRC
	rex_test.cpp
)

set(REX_TEST rex_test)

add_executable(${REX_TEST} ${REX_TEST_SRC})
target_link_libraries(${REX_TEST} rex)
target_include_directories(${REX_TEST} PUBLIC ${CMAKE_SOURCE_DIR}/source)
set_target_properties(${REX_TEST}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)
