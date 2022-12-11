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
## Rex Engine platform & compiler determinator
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

	add_compile_definitions("REX_TARGET=0")

	## GCC
	if(REX_COMPILER STREQUAL "GCC")

		add_compile_definitions("REX_COMPILER=0")

	endif()

endif()

## Windows target
if(REX_TARGET STREQUAL "LINUX")

	add_compile_definitions("REX_TARGET=1")

	## MINGW
	if(REX_COMPILER STREQUAL "MINGW")

		add_compile_definitions("REX_COMPILER=1")

	endif()

endif()

## DOS target
if(REX_TARGET STREQUAL "DOS")

	add_compile_definitions("REX_TARGET=2")

	## DJGPP
	if(REX_COMPILER STREQUAL "DJGPP")

		add_compile_definitions("REX_COMPILER=2")

	endif()

	## Open Watcom
	if(REX_COMPILER STREQUAL "WATCOM")

		add_compile_definitions("REX_COMPILER=3")

	endif()

endif()

##
##
## Rex Engine module determinator
##
##

##
## Core modules
##

## Colormap
if(${MODULE_COLORMAP})
	add_compile_definitions("MODULE_COLORMAP=1")
endif()

## Console
if(${MODULE_CONSOLE})
	add_compile_definitions("MODULE_CONSOLE=1")
endif()

## Math
if(${MODULE_MATH})
	add_compile_definitions("MODULE_MATH=1")
endif()

## Palette
if(${MODULE_PALETTE})
	add_compile_definitions("MODULE_PALETTE=1")
endif()

## Surface
if(${MODULE_SURFACE})
	add_compile_definitions("MODULE_SURFACE=1")
endif()

## Utils
if(${MODULE_UTILS})
	add_compile_definitions("MODULE_UTILS=1")
endif()

##
## Hardware I/O modules
##

## DOS
if(${MODULE_DOS})
	add_compile_definitions("MODULE_DOS=1")
endif()

## VESA
if(${MODULE_VESA})
	add_compile_definitions("MODULE_VESA=1")
endif()

## VGA
if(${MODULE_VGA})
	add_compile_definitions("MODULE_VGA=1")
endif()

##
## Renderer modules
##

## Raycast
if(${MODULE_RAYCAST})
	add_compile_definitions("MODULE_RAYCAST=1")
endif()

## Portal2D
if(${MODULE_PORTAL2D})
	add_compile_definitions("MODULE_PORTAL2D=1")
endif()

## Voxel
if(${MODULE_VOXEL})
	add_compile_definitions("MODULE_VOXEL=1")
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
## Core modules
##

## Colormap
if(${MODULE_COLORMAP})

	set(REX_COLORMAP_SRC
		modules/core/colormap/colormap.cpp
	)

	add_library(colormap STATIC ${REX_COLORMAP_SRC})

endif()

## Console
if(${MODULE_CONSOLE})

	set(REX_CONSOLE_SRC
		modules/core/console/console.cpp
	)

	add_library(console STATIC ${REX_CONSOLE_SRC})

endif()

## Math
if(${MODULE_MATH})

	set(REX_MATH_SRC
		modules/core/math/fixed.cpp
		modules/core/math/integer.cpp
		modules/core/math/sintable.cpp
		modules/core/math/table.cpp
	)

	add_library(math STATIC ${REX_MATH_SRC})

endif()

## Palette
if(${MODULE_PALETTE})

	set(REX_PALETTE_SRC
		modules/core/palette/palette.cpp
	)

	add_library(palette STATIC ${REX_PALETTE_SRC})

endif()

## Surface
if(${MODULE_SURFACE})

	set(REX_SURFACE_SRC
		modules/core/surface/picture.cpp
	)

	add_library(surface STATIC ${REX_SURFACE_SRC})

endif()

## Utils
if(${MODULE_UTILS})

	set(REX_UTILS_SRC
		modules/core/utils/utils.cpp
	)

	add_library(utils STATIC ${REX_UTILS_SRC})

endif()

##
## Hardware I/O modules
##

## DOS
if(${MODULE_DOS})

	set(REX_DOS_SRC
		modules/hwio/dos/dos.cpp
	)

	add_library(dos STATIC ${REX_DOS_SRC})

endif()

## VESA
if(${MODULE_VESA})

	set(REX_VESA_SRC
		modules/hwio/vesa/vesa.cpp
	)

	add_library(vesa STATIC ${REX_VESA_SRC})

endif()

## VGA
if(${MODULE_VGA})

	set(REX_VGA_SRC
		modules/hwio/vga/vga.cpp
	)

	add_library(vga STATIC ${REX_VGA_SRC})

endif()

##
## Renderer modules
##

## Raycast
if(${MODULE_RAYCAST})

	set(REX_RAYCAST_SRC
		modules/render/raycast/raycast.cpp
	)

	add_library(raycast STATIC ${REX_RAYCAST_SRC})

endif()

## Portal2D
if(${MODULE})

	set(REX_PORTAL2D_SRC
		modules/render/portal2d/portal2d.cpp
	)

	add_library(portal2d STATIC ${REX_PORTAL2D_SRC})

endif()

##
## Interface library
##

add_library(rex INTERFACE)

##
## Core modules
##

if(${MODULE_COLORMAP})
	target_link_libraries(rex INTERFACE colormap)
endif()

if(${MODULE_CONSOLE})
	target_link_libraries(rex INTERFACE console)
endif()

if(${MODULE_PALETTE})
	target_link_libraries(rex INTERFACE palette)
endif()

if(${MODULE_SURFACE})
	target_link_libraries(rex INTERFACE surface)
endif()

if(${MODULE_UTILS})
	target_link_libraries(rex INTERFACE utils)
endif()

if(${MODULE_MATH})
	target_link_libraries(rex INTERFACE math)
endif()

##
## Hardware I/O modules
##

if(${MODULE_DOS})
	target_link_libraries(rex INTERFACE dos)
endif()

if(${MODULE_VESA})
	target_link_libraries(rex INTERFACE vesa)
endif()

if(${MODULE_VGA})
	target_link_libraries(rex INTERFACE vga)
endif()

##
## Renderer modules
##

if(${MODULE_RAYCAST})
	target_link_libraries(rex INTERFACE raycast)
endif()

if(${MODULE})
	target_link_libraries(rex INTERFACE portal2d)
endif()

if(${MODULE_VOXEL})
	target_link_libraries(rex INTERFACE voxel)
endif()
