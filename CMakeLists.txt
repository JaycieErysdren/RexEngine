## ========================================================
##
## FILE:			/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Duke4Ever
##
## LICENSE:			TBD
##
## DESCRIPTION:		Duke4Ever CMake project definition
##
## LAST EDITED:		December 11th, 2022
##
## ========================================================

##
## Duke4Ever definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Project definition
project(Duke4Ever LANGUAGES C CXX)

## Build type
set(CMAKE_BUILD_TYPE Release)

##
## Duke4Ever compile options
##

add_compile_options("-Ofast")
add_compile_options("-Wno-attributes")

##
## Rex Engine
##

## Set our target and compiler
set(REX_TARGET "DOS")
set(REX_COMPILER "DJGPP")

## Set the modules we want
set(MODULE_COLORMAP TRUE)
set(MODULE_CONSOLE TRUE)
set(MODULE_MATH TRUE)
set(MODULE_PALETTE TRUE)
set(MODULE_SURFACE TRUE)
set(MODULE_UTILS TRUE)
set(MODULE_DOS TRUE)
set(MODULE_VESA TRUE)
set(MODULE_VGA TRUE)
set(MODULE_RAYCAST TRUE)

add_subdirectory(${CMAKE_SOURCE_DIR}/source/rex)

##
## Applications
##

## Duke4Ever
set(DUKE4EVER_SRC
	${CMAKE_SOURCE_DIR}/source/duke4evr/main.cpp
)

add_executable(duke4evr ${DUKE4EVER_SRC})
target_link_libraries(duke4evr rex)
target_include_directories(duke4evr PUBLIC ${CMAKE_SOURCE_DIR}/source/rex)
set_target_properties(duke4evr
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
)

## Testbed
set(TESTBED_SRC
	${CMAKE_SOURCE_DIR}/source/testbed/main.cpp
)

add_executable(testbed ${TESTBED_SRC})
target_link_libraries(testbed rex)
target_include_directories(testbed PUBLIC ${CMAKE_SOURCE_DIR}/source/rex)
set_target_properties(testbed
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
)
