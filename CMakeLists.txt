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
## Rex Engine
##

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

## Rex Engine platform & compiler determinator
include(cmake/rex_determinator.cmake)

add_subdirectory(${CMAKE_SOURCE_DIR}/source/rex)

##
## Applications
##

## Duke4Ever
set(DUKE4EVER_SRC
	${CMAKE_SOURCE_DIR}/source/duke4evr/main.cpp
)

set(DUKE4EVER duke4${REX_EXECUTABLE_SUFFIX})

add_executable(${DUKE4EVER} ${DUKE4EVER_SRC})
target_link_libraries(${DUKE4EVER} rex)
target_include_directories(${DUKE4EVER} PUBLIC ${CMAKE_SOURCE_DIR}/source/rex)
set_target_properties(${DUKE4EVER}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
)

## Testbed
set(TESTBED_SRC
	${CMAKE_SOURCE_DIR}/source/testbed/main.cpp
)

set(TESTBED testbed${REX_EXECUTABLE_SUFFIX})

add_executable(${TESTBED} ${TESTBED_SRC})
target_link_libraries(${TESTBED} rex)
target_include_directories(${TESTBED} PUBLIC ${CMAKE_SOURCE_DIR}/source/rex)
set_target_properties(${TESTBED}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
)
