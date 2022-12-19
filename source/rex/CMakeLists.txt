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
## LAST EDITED:		December 18th, 2022
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

## For rex_priv.hpp
include_directories(.)

##
##
## Rex Engine thirdparty modules
##
##

## cJSON
set(CJSON_SRC
	thrdprty/cjson/cJSON.c
)

add_library(cjson STATIC ${CJSON_SRC})

## mplite
set(MPLITE_SRC
	thrdprty/mplite/mplite.c
)

add_library(mplite STATIC ${MPLITE_SRC})

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
	core/log.cpp
	core/math_i.cpp
	core/math_sin.cpp
	core/math_x.cpp
	core/mathtabl.cpp
	core/mempool.cpp
	core/mouse.cpp
	core/render.cpp
	core/scene.cpp
	core/surface.cpp
)

add_library(rex_core STATIC ${REX_CORE_SRC})

##
##
## Rex Engine modules
##
##

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

## Add Rex core
target_link_libraries(rex INTERFACE rex_core)

## Add cJSON
target_link_libraries(rex INTERFACE cjson)

## Add mplite
target_link_libraries(rex INTERFACE mplite)

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
