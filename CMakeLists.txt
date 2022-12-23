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
## DESCRIPTION:		Rex Engine CMake project definition
##
## LAST EDITED:		December 22nd, 2022
##
## ========================================================

##
## Rex Engine definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Project definition
project(RexEngine LANGUAGES C CXX)

## Build type
set(CMAKE_BUILD_TYPE Release)

##
## Rex Engine
##

## Set the modules we want to compile
set(MODULE_DOS TRUE)
set(MODULE_VESA TRUE)
set(MODULE_VGA TRUE)

set(MODULE_PAK TRUE)
set(MODULE_ZIP TRUE)
set(MODULE_GRP TRUE)
set(MODULE_WAD TRUE)
set(MODULE_ART TRUE)

## Set the renderers we want to compile
set(RENDERER_VOXEL TRUE)
set(RENDERER_RAYCAST TRUE)
set(RENDERER_SECTOR TRUE)

## Rex Engine platform & compiler determinator
include(${CMAKE_SOURCE_DIR}/cmake/rex_determinator.cmake)
add_subdirectory(${CMAKE_SOURCE_DIR}/source/rex)

##
## Applications & games
##

## VivaDukeNukem
add_subdirectory(${CMAKE_SOURCE_DIR}/source/vivaduke)

## Decker
add_subdirectory(${CMAKE_SOURCE_DIR}/source/decker)
