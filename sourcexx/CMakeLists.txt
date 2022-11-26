## ========================================================
##
## FILE:			/sourcexx/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Rex Engine CMake project definition.
##
## LAST EDITED:		November 26th, 2022
##
## ========================================================

##
## Project definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Build type
set(CMAKE_BUILD_TYPE Debug)

## Define project
project(RexEngine LANGUAGES C CXX)

##
## Find and build external dependencies
##

## ImGUI
include(${CMAKE_SOURCE_DIR}/thirdparty/ImGUI.cmake)

## SDL2
find_package(SDL2 REQUIRED)
