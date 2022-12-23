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

##
## VivaDukeNukem source
##

set(VIVADUKE_SRC
	bootstrp.cpp
	camera.cpp
	endscreen.cpp
	frame.cpp
	level.cpp
	mouse.cpp
	tick.cpp
	vivaduke.cpp
)

set(VIVADUKE vivaduke)

add_executable(${VIVADUKE} ${VIVADUKE_SRC})
target_link_libraries(${VIVADUKE} rex)
target_include_directories(${VIVADUKE} PUBLIC ../)
set_target_properties(${VIVADUKE}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)
