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
## LAST EDITED:		November 29th, 2022
##
## ========================================================

##
## Project Definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Project
project(Duke4Ever LANGUAGES C CXX)

## Build type
set(CMAKE_BUILD_TYPE Release)

##
## Project Options
##

option(USE_FLOATING_POINT "Use floating point for the base engine math" ON)

##
## Project Compile Options
##

add_compile_options("-O3")

##
## Project Libraries
##

set(REX3D_CORE_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/core/math.cpp
)

set(REX3D_HWDRV_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/hwdrv/dos.cpp
)

set(REX3D_VIDDRV_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/viddrv/vga.cpp
)

set(REX3D_REND_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/rend/raycastr.cpp
)

add_library(rex3d
	${REX3D_CORE_SRC}
	${REX3D_HWDRV_SRC}
	${REX3D_VIDDRV_SRC}
	${REX3D_REND_SRC}
)

target_compile_definitions(rex3d PUBLIC "BASED_FLOAT=1")
target_include_directories(rex3d PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(rex3d PUBLIC ${CMAKE_SOURCE_DIR}/source/rex3d)

##
## Project Applications
##

set(DUKE4EVER_SRC
	${CMAKE_SOURCE_DIR}/source/duke4evr/main.cpp
)

add_executable(duke4evr ${DUKE4EVER_SRC})
target_link_libraries(duke4evr rex3d)
target_include_directories(duke4evr PUBLIC ${CMAKE_SOURCE_DIR}/source)
set_target_properties(duke4evr
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
)
