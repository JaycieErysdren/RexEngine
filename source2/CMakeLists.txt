## ========================================================
##
## FILE:			/source2/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Rex Engine CMake project definition.
##
## LAST EDITED:		November 18th, 2022
##
## ========================================================

##
## Definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Define build type
set(CMAKE_BUILD_TYPE Debug)

## Define project
project(RexEngine)

##
## Rex Engine
##

## Rex Engine sources
set(REX_ENGINE_SRC
	${CMAKE_SOURCE_DIR}/arch/dos.c
	${CMAKE_SOURCE_DIR}/arch/vga.c
)

## Add Rex Engine library
add_library(rex ${REX_ENGINE_SRC})

## Rex Engione include directories
target_include_directories(rex PUBLIC
	${CMAKE_SOURCE_DIR}/include
)
