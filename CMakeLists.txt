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
## LAST EDITED:		December 30th, 2022
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

## Rex Engine platform & compiler determinator
include(${CMAKE_SOURCE_DIR}/cmake/rex_determinator.cmake)
add_subdirectory(${CMAKE_SOURCE_DIR}/source/rex)

##
## Applications & games
##

## TestApp
add_subdirectory(${CMAKE_SOURCE_DIR}/source/testapp)
