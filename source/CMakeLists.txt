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
## LAST EDITED:		November 12th, 2022
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

## Define project options
option(REX_BUILD_EXTERNAL_EXAMPLES "Build example programs from external libraries" OFF)

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

## Curses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
link_libraries(${CURSES_LIBRARY})

## Math
if(NOT WIN32)
	link_libraries(m)
endif()

##
## Rex Engine
##

## Include main Rex Engine header folder
include_directories(${CMAKE_SOURCE_DIR}/core/inc)

## Rex Engine sources
set(REX_ENGINE_SRC
	${CMAKE_SOURCE_DIR}/core/engine/bootstrap.c
	${CMAKE_SOURCE_DIR}/core/engine/device_io.c
	${CMAKE_SOURCE_DIR}/core/engine/endian.c
	${CMAKE_SOURCE_DIR}/core/engine/error.c
	${CMAKE_SOURCE_DIR}/core/engine/file_io.c
	${CMAKE_SOURCE_DIR}/core/engine/log.c
	${CMAKE_SOURCE_DIR}/core/engine/math.c
	${CMAKE_SOURCE_DIR}/core/engine/resfile.c
	## Formats module
	${CMAKE_SOURCE_DIR}/core/formats/lev.c
	${CMAKE_SOURCE_DIR}/core/formats/mdl.c
	${CMAKE_SOURCE_DIR}/core/formats/pak.c
	${CMAKE_SOURCE_DIR}/core/formats/tmf.c
	${CMAKE_SOURCE_DIR}/core/formats/wal.c
	## LIB I/O module
	${CMAKE_SOURCE_DIR}/core/lib_io/brender.c
	${CMAKE_SOURCE_DIR}/core/lib_io/nuklear.c
	${CMAKE_SOURCE_DIR}/core/lib_io/tinyfd.c
	## Physics module
	${CMAKE_SOURCE_DIR}/core/physics/physics.c
	## Rendering module
	${CMAKE_SOURCE_DIR}/core/rendering/palettes.c
	${CMAKE_SOURCE_DIR}/core/rendering/window.c
)

## Rex Engine library
add_library(RexEngine ${REX_ENGINE_SRC})

##
## Rex Engine Applications
##

## Link Rex Engine
link_libraries(RexEngine)

## Liberator
## Format manager
add_subdirectory(${CMAKE_SOURCE_DIR}/applications/liberator)

## PAKTUI
## id Software PAK TUI
add_subdirectory(${CMAKE_SOURCE_DIR}/applications/paktui)

## NOWARE
add_executable(haiku ${CMAKE_SOURCE_DIR}/applications/noware/haiku.c)

## Nuklear example applications
if(REX_BUILD_EXTERNAL_EXAMPLES)
	find_package(OpenGL REQUIRED)
	find_package(GLEW REQUIRED)

	add_executable(nuklear_file_browser ${CMAKE_SOURCE_DIR}/external/Nuklear/example/file_browser.c)
	target_link_libraries(nuklear_file_browser PRIVATE glfw GLEW)
endif()
