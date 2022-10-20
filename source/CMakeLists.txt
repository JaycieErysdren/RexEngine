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
## DESCRIPTION:		Main CMake project definition.
##
## LAST EDITED:		October 20th, 2022
##
## ========================================================

cmake_minimum_required(VERSION 3.13)
project(RexEngine)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Debug)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(OpenGL REQUIRED)

link_libraries(${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS})

link_libraries(${SDL2TTF_LIBRARY})
include_directories(${SDL2TTF_INCLUDE_DIR})

link_libraries(${OPENGL_LIBRARIES})
include_directories(${OPENGL_INCLUDE_DIRS})

include_directories(core/inc)

add_subdirectory(core)
add_subdirectory(liberator)
add_subdirectory(testgame)
