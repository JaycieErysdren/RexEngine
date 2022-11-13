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
## LAST EDITED:		November 13th, 2022
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
## Find and build external dependencies
##

## BRender
add_subdirectory(${CMAKE_SOURCE_DIR}/external/BRender)

## tinyfiledialogs
add_library(tinyfd ${CMAKE_SOURCE_DIR}/external/tinyfd/tinyfiledialogs.c)
target_include_directories(tinyfd PUBLIC ${CMAKE_SOURCE_DIR}/external/tinyfd)

## SDL2
find_package(SDL2 REQUIRED)

## Curses
find_package(Curses REQUIRED)

## Math
if(NOT WIN32)
	link_libraries(m)
endif()

##
## Rex Engine
##

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

## Add Rex Engine library
add_library(RexEngine ${REX_ENGINE_SRC})

target_include_directories(RexEngine PUBLIC
	${CMAKE_SOURCE_DIR}/core/inc
	${SDL2_INCLUDE_DIRS}
	${CURSES_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/external/tinyfd
	${CMAKE_SOURCE_DIR}/external/Nuklear
)

target_link_libraries(RexEngine
	BRender::Full
	tinyfd
	${SDL2_LIBRARIES}
	${CURSES_LIBRARY}
)

##
## Rex Engine Applications
##

## Liberator
## Format manager
add_executable(liberator ${CMAKE_SOURCE_DIR}/applications/liberator/liberator.c)
target_link_libraries(liberator RexEngine)

## PAKTUI
## id Software PAK TUI
add_executable(paktui ${CMAKE_SOURCE_DIR}/applications/paktui/paktui.c)
target_link_libraries(paktui RexEngine)

## NOWARE
add_executable(haiku ${CMAKE_SOURCE_DIR}/applications/noware/haiku.c)
