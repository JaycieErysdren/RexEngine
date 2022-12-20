## ========================================================
##
## FILE:			/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Viva Duke Nukem
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Viva Duke Nukem CMake project definition
##
## LAST EDITED:		December 18th, 2022
##
## ========================================================

##
## Viva Duke Nukem definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Project definition
project(VivaDukeNukem LANGUAGES C CXX)

## Build type
set(CMAKE_BUILD_TYPE Release)

##
## Rex Engine
##

## Set the modules we want
set(MODULE_DOS TRUE)
set(MODULE_VESA TRUE)
set(MODULE_VGA TRUE)

## Set the renderers we want
set(RENDERER_VOXEL TRUE)
set(RENDERER_RAYCAST TRUE)
set(RENDERER_SECTOR TRUE)

## Rex Engine platform & compiler determinator
include(cmake/rex_determinator.cmake)

add_subdirectory(${CMAKE_SOURCE_DIR}/source/rex)

##
## Applications
##

## Game
set(VIVADUKE_SRC
	${CMAKE_SOURCE_DIR}/source/vivaduke/vivaduke.cpp
)

set(VIVADUKE vivaduke)

add_executable(${VIVADUKE} ${VIVADUKE_SRC})
target_link_libraries(${VIVADUKE} rex)
target_include_directories(${VIVADUKE} PUBLIC ${CMAKE_SOURCE_DIR}/source)
set_target_properties(${VIVADUKE}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
)
