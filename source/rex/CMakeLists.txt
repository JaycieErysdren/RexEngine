## ========================================================
##
## FILE:			/source/rex/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Rex Engine CMake project definition
##
## LAST EDITED:		December 11th, 2022
##
## ========================================================

##
##
## Rex Engine definitions
##
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Project definition
project(RexEngine LANGUAGES C CXX)

##
##
## Rex Engine platform/compiler determinator
##
##

##
## Target & host Platforms:
## 0: Linux
## 1: Windows
## 2: DOS
##

##
## Compilers:
## 0: GCC
## 1: MINGW
## 2: DJGPP
## 3: WATCOM
##

## Linux target
if(REX_TARGET STREQUAL "LINUX")

	add_compile_definitions(REX_TARGET=0)

	## GCC
	if(REX_COMPILER STREQUAL "GCC")

		add_compile_definitions(REX_COMPILER=0)

	endif()

endif()

## Windows target
if(REX_TARGET STREQUAL "LINUX")

	add_compile_definitions(REX_TARGET=1)

	## MINGW
	if(REX_COMPILER STREQUAL "MINGW")

		add_compile_definitions(REX_COMPILER=1)

	endif()

endif()

## DOS target
if(REX_TARGET STREQUAL "DOS")

	add_compile_definitions(REX_TARGET=2)

	## DJGPP
	if(REX_COMPILER STREQUAL "DJGPP")

		add_compile_definitions(REX_COMPILER=2)

	endif()

	## Open Watcom
	if(REX_COMPILER STREQUAL "WATCOM")

		add_compile_definitions(REX_COMPILER=3)

	endif()

endif()

##
##
## Rex Engine global options
##
##

## For rex.hpp
include_directories(.)

##
##
## Rex Engine modules
##
##

##
## Rex Engine core
##

## Colormap
set(REX_COLORMAP_SRC
	modules/core/colormap/colormap.cpp
)

add_library(colormap STATIC ${REX_COLORMAP_SRC})

## Console
set(REX_CONSOLE_SRC
	modules/core/console/console.cpp
)

add_library(console STATIC ${REX_CONSOLE_SRC})

## Math
set(REX_MATH_SRC
	modules/core/math/fixed.cpp
	modules/core/math/integer.cpp
	modules/core/math/sintable.cpp
	modules/core/math/table.cpp
)

add_library(math STATIC ${REX_MATH_SRC})

## Palette
set(REX_PALETTE_SRC
	modules/core/palette/palette.cpp
)

add_library(palette STATIC ${REX_PALETTE_SRC})

## Surface
set(REX_SURFACE_SRC
	modules/core/surface/picture.cpp
)

add_library(surface STATIC ${REX_SURFACE_SRC})

## Utils
set(REX_UTILS_SRC
	modules/core/utils/utils.cpp
)

add_library(utils STATIC ${REX_UTILS_SRC})

##
## Rex Engine hardware i/o
##

## DOS
set(REX_DOS_SRC
	modules/hwio/dos/dos.cpp
)

add_library(dos STATIC ${REX_DOS_SRC})

## VESA
set(REX_VESA_SRC
	modules/hwio/vesa/vesa.cpp
)

add_library(vesa STATIC ${REX_VESA_SRC})

## VGA
set(REX_VGA_SRC
	modules/hwio/vga/vga.cpp
)

add_library(vga STATIC ${REX_VGA_SRC})

##
## Rex Engine renderers
##

## Raycaster
set(REX_RAYCASTER_SRC
	modules/render/raycast/raycast.cpp
)

add_library(raycast STATIC ${REX_RAYCASTER_SRC})

## Portals (2D)
set(REX_PORTAL2D_SRC
	modules/render/portal2d/portal2d.cpp
)

## add_library(portal2d STATIC ${REX_PORTAL2D_SRC})

##
## Rex Engine interface library
##

add_library(rex INTERFACE)
target_link_libraries(rex INTERFACE

	## Core modules
	colormap console palette surface utils math

	## Hardware i/o modules
	dos vesa vga

	## Renderer modules
	raycast
)
