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
## LAST EDITED:		December 15th, 2022
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

## Rex Engine platform & compiler determinator
include(../../cmake/rex_determinator.cmake)

##
##
## Rex Engine module determinator
##
##

##
## Core modules
##

## Math
if(${MODULE_MATH})
	add_compile_definitions("MODULE_MATH=1")
endif()

## Utils
if(${MODULE_UTILS})
	add_compile_definitions("MODULE_UTILS=1")
endif()

##
## Hardware I/O modules
##

## DOS target modules
if(REX_TARGET STREQUAL "DOS")

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

endif()

##
## Renderer modules
##

## Raycast
if(${RENDERER_RAYCAST})
	add_compile_definitions("RENDERER_RAYCAST=1")
endif()

## Portal2D
if(${RENDERER_PORTAL2D})
	add_compile_definitions("RENDERER_PORTAL2D=1")
endif()

## Voxel
if(${RENDERER_VOXEL})
	add_compile_definitions("RENDERER_VOXEL=1")
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
## Rex Engine core
##
##

set(REX_CORE_SRC
	core/bootstrap.cpp
	core/camera.cpp
	core/clock.cpp
	core/console.cpp
	core/graphics.cpp
	core/keyboard.cpp
	core/mathtabl.cpp
	core/mouse.cpp
	core/surface.cpp
)

add_library(rex_core STATIC ${REX_CORE_SRC})

##
##
## Rex Engine modules
##
##

##
## Core modules
##

## Math
if(${MODULE_MATH})

	set(REX_MATH_SRC
		modules/core/math/fixed.cpp
		modules/core/math/integer.cpp
		modules/core/math/sintable.cpp
	)

	add_library(math STATIC ${REX_MATH_SRC})

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

## DOS target modules
if(REX_TARGET STREQUAL "DOS")

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

endif()

##
## Renderer modules
##

## Raycast
if(${RENDERER_RAYCAST})

	set(REX_RAYCAST_SRC
		render/raycast/raycast.cpp
	)

	add_library(raycast STATIC ${REX_RAYCAST_SRC})

endif()

## Portal2D
if(${RENDERER_PORTAL2D})

	set(REX_PORTAL2D_SRC
		render/portal2d/portal2d.cpp
	)

	add_library(portal2d STATIC ${REX_PORTAL2D_SRC})

endif()

## Voxel
if(${RENDERER_VOXEL})

	set(REX_VOXEL_SRC
		render/voxel/world.cpp
		render/voxel/render.cpp
	)

	add_library(voxel STATIC ${REX_VOXEL_SRC})

endif()

##
## Interface library
##

add_library(rex INTERFACE)

target_link_libraries(rex INTERFACE rex_core)

##
## Core modules
##

if(${MODULE_UTILS})
	target_link_libraries(rex INTERFACE utils)
endif()

if(${MODULE_MATH})
	target_link_libraries(rex INTERFACE math)
endif()

##
## Hardware I/O modules
##

## DOS target modules
if(REX_TARGET STREQUAL "DOS")

	if(${MODULE_DOS})
		target_link_libraries(rex INTERFACE dos)
	endif()

	if(${MODULE_VESA})
		target_link_libraries(rex INTERFACE vesa)
	endif()

	if(${MODULE_VGA})
		target_link_libraries(rex INTERFACE vga)
	endif()

endif()

##
## Renderer modules
##

if(${RENDERER_RAYCAST})
	target_link_libraries(rex INTERFACE raycast)
endif()

if(${RENDERER_PORTAL2D})
	target_link_libraries(rex INTERFACE portal2d)
endif()

if(${RENDERER_VOXEL})
	target_link_libraries(rex INTERFACE voxel)
endif()
