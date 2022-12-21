## ========================================================
##
## FILE:			/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			VivaDukeNukem
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		VivaDukeNukem CMake project definition
##
## LAST EDITED:		December 21st, 2022
##
## ========================================================

##
## VivaDukeNukem definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Project definition
project(VivaDukeNukem LANGUAGES C CXX)

## Build type
set(CMAKE_BUILD_TYPE Release)

##
## VivaDukeNukem source
##

set(VIVADUKE_SRC
	${CMAKE_SOURCE_DIR}/source/vivaduke/vivaduke.cpp
	${CMAKE_SOURCE_DIR}/source/vivaduke/level.cpp
)

set(VIVADUKE vivaduke)

add_executable(${VIVADUKE} ${VIVADUKE_SRC})
target_link_libraries(${VIVADUKE} rex)
target_include_directories(${VIVADUKE} PUBLIC ${CMAKE_SOURCE_DIR}/source)
set_target_properties(${VIVADUKE}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)
