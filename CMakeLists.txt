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
## LAST EDITED:		December 18th, 2022
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
set(GAME_SRC
	${CMAKE_SOURCE_DIR}/source/game/main.cpp
)

set(GAME game${REX_EXECUTABLE_SUFFIX})

add_executable(${GAME} ${GAME_SRC})
target_link_libraries(${GAME} rex)
target_include_directories(${GAME} PUBLIC ${CMAKE_SOURCE_DIR}/source)
set_target_properties(${GAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
)
