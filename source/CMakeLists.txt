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

##
## Rex Engine
##

## Define project
project(RexEngine)

## Add custom CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

## Build type
set(CMAKE_BUILD_TYPE Debug)

## Find SDL2, SDL2TTF, and OpenGL packages
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(OpenGL REQUIRED)

## Link SDL2
link_libraries(${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS})

## Link SDL2TTF
link_libraries(${SDL2TTF_LIBRARY})
include_directories(${SDL2TTF_INCLUDE_DIR})

## Link OpenGL
link_libraries(${OPENGL_LIBRARIES})
include_directories(${OPENGL_INCLUDE_DIRS})

## Link math library if not on Windows
if(NOT WIN32)
	link_libraries(m)
endif()

## Include main Rex Engine header folder
include_directories(${CMAKE_SOURCE_DIR}/core/inc)

## Add Rex Engine core directories
add_subdirectory(${CMAKE_SOURCE_DIR}/core)

##
## Rex Engine Applications
##

## Link Rex Engine
link_libraries(RexEngine)

## BRenderTest
## BRender integration test
add_subdirectory(${CMAKE_SOURCE_DIR}/applications/brendertest)

## Liberator
## Format manager
add_subdirectory(${CMAKE_SOURCE_DIR}/applications/liberator)

## TestGame
## Raycasting test
## add_subdirectory(${CMAKE_SOURCE_DIR}/applications/testgame)

##
## Testing Nuklear demos
##

add_executable(nuklear_sdl_surface ${CMAKE_SOURCE_DIR}/external/Nuklear/demo/sdl2surface_rawfb/main.c)
add_executable(nuklear_sdl_renderer ${CMAKE_SOURCE_DIR}/external/Nuklear/demo/sdl_renderer/main.c)
