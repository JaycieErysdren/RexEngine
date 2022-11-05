## ========================================================
##
## FILE:			/source/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Rex Engine CMake project definition.
##
## LAST EDITED:		October 28th, 2022
##
## ========================================================

##
## Startup definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Build type
set(CMAKE_BUILD_TYPE Debug)

## Define project
project(RexEngine)

##
## External dependencies
##

## BRender
add_subdirectory(${CMAKE_SOURCE_DIR}/external/BRender)
link_libraries(BRender::Full)

## Nuklear
include_directories(${CMAKE_SOURCE_DIR}/external/Nuklear)

## tinyfiledialogs
include_directories(${CMAKE_SOURCE_DIR}/external/tinyfd)
add_library(tinyfd ${CMAKE_SOURCE_DIR}/external/tinyfd/tinyfiledialogs.c)
link_libraries(tinyfd)

## SDL2
find_package(SDL2 REQUIRED)
link_libraries(${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS})

## Math
if(NOT WIN32)
	link_libraries(m)
endif()

##
## Rex Engine
##

## Include main Rex Engine header folder
include_directories(${CMAKE_SOURCE_DIR}/core/inc)

## Add Rex Engine core directories
add_subdirectory(${CMAKE_SOURCE_DIR}/core)

##
## Rex Engine Applications
##

## Link Rex Engine
link_libraries(RexEngine)

## Liberator
## Format manager
add_subdirectory(${CMAKE_SOURCE_DIR}/applications/liberator)

##
## Nuklear examples
##

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

## Nuklear example applications
add_executable(nuklear_file_browser ${CMAKE_SOURCE_DIR}/external/Nuklear/example/file_browser.c)
target_link_libraries(nuklear_file_browser PRIVATE glfw GLEW)
