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
## LAST EDITED:		December 7th, 2022
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
## Project Compile Options
##

add_compile_options("-O3")
add_compile_options("-Wno-attributes")

##
## Project Thirdparty Libraries
##

add_library(cjson STATIC ${CMAKE_SOURCE_DIR}/source/thirdparty/cjson/cJSON.c)
target_include_directories(cjson PUBLIC ${CMAKE_SOURCE_DIR}/source/thirdparty/cjson)

##
## Project Libraries
##

set(REX3D_CORE_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/core/colormap.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/core/console.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/core/palette.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/core/picture.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/core/utils.cpp
)

set(REX3D_HWDRV_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/hwdrv/dos.cpp
)

set(REX3D_MATH_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/math/math_i.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/math/math_x.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/math/mathtabl.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/math/sintable.cpp
)

set(REX3D_THRDPRTY_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/thrdprty/rcl.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/thrdprty/tiled.cpp
)

set(REX3D_VIDDRV_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/viddrv/vga.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/viddrv/vesa.cpp
)

add_library(rex3d STATIC
	${REX3D_CORE_SRC}
	${REX3D_HWDRV_SRC}
	${REX3D_MATH_SRC}
	${REX3D_THRDPRTY_SRC}
	${REX3D_VIDDRV_SRC}
)

target_include_directories(rex3d PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(rex3d PUBLIC ${CMAKE_SOURCE_DIR}/source/rex3d)
target_link_libraries(rex3d PUBLIC cjson)

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
