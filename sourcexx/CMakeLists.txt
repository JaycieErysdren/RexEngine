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

##
## Applications
##

add_executable(testapp 
	${CMAKE_SOURCE_DIR}/test/main.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/ImGUI/backends/imgui_impl_sdl.cpp
	${CMAKE_SOURCE_DIR}/thirdparty/ImGUI/backends/imgui_impl_sdlrenderer.cpp
)

target_link_libraries(testapp
	ImGUI
	${SDL2_LIBRARIES}
)

target_include_directories(testapp PUBLIC
	${SDL2_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/thirdparty/ImGUI
	${CMAKE_SOURCE_DIR}/thirdparty/ImGUI/backends
)
