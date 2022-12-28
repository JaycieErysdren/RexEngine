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
## LAST EDITED:		December 27th, 2022
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

	## ADLIB
	if(${MODULE_ADLIB})
		add_compile_definitions("MODULE_ADLIB=1")
	endif()

endif()

##
## File I/O modules
##

## PAK
if(${MODULE_PAK})
	add_compile_definitions("MODULE_PAK=1")
endif()

## ZIP
if(${MODULE_ZIP})
	add_compile_definitions("MODULE_ZIP=1")
endif()

## GRP
if(${MODULE_GRP})
	add_compile_definitions("MODULE_GRP=1")
endif()

## WAD
if(${MODULE_WAD})
	add_compile_definitions("MODULE_WAD=1")
endif()

## ART
if(${MODULE_ART})
	add_compile_definitions("MODULE_ART=1")
endif()

## IMF
if(${MODULE_IMF})
	add_compile_definitions("MODULE_IMF=1")
endif()

##
## Renderer modules
##

## Raycast
if(${RENDERER_RAYCAST})
	add_compile_definitions("RENDERER_RAYCAST=1")
endif()

## Sector
if(${RENDERER_SECTOR})
	add_compile_definitions("RENDERER_SECTOR=1")
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
include_directories(thrdprty/osmesa)

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
	core/softrend/sector.cpp
	core/glrend/test.cpp
	core/bootstrap.cpp
	core/clock.cpp
	## core/console.cpp
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
	core/sound.cpp
	core/surface.cpp
	core/vfs.cpp
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

	## ADLIB
	if(${MODULE_ADLIB})

		set(REX_ADLIB_SRC
			modules/hwio/adlib/adlib.cpp
		)

		add_library(adlib STATIC ${REX_ADLIB_SRC})

	endif()

endif()

##
## File I/O modules
##

## PAK
if(${MODULE_PAK})

	set(REX_PAK_SRC
		modules/fileio/pak/pak.cpp
	)

	add_library(pak STATIC ${REX_PAK_SRC})

endif()

## ZIP
if(${MODULE_ZIP})

	set(REX_ZIP_SRC
		modules/fileio/zip/zip.cpp
	)

	add_library(zip STATIC ${REX_ZIP_SRC})

endif()

## GRP
if(${MODULE_GRP})

	set(REX_GRP_SRC
		modules/fileio/grp/grp.cpp
	)

	add_library(grp STATIC ${REX_GRP_SRC})

endif()

## WAD
if(${MODULE_WAD})

	set(REX_WAD_SRC
		modules/fileio/wad/wad.cpp
	)

	add_library(wad STATIC ${REX_WAD_SRC})

endif()

## ART
if(${MODULE_ART})

	set(REX_ART_SRC
		modules/fileio/art/art.cpp
	)

	add_library(art STATIC ${REX_ART_SRC})

endif()

## IMF
if(${MODULE_IMF})

	set(REX_IMF_SRC
		modules/fileio/imf/imf.cpp
	)

	add_library(imf STATIC ${REX_IMF_SRC})

endif()

##
## Renderer modules
##

## Raycast
if(${RENDERER_RAYCAST})

	set(REX_RAYCAST_SRC
		render/raycast/render.cpp
	)

	add_library(raycast STATIC ${REX_RAYCAST_SRC})

endif()

## Sector
if(${RENDERER_SECTOR})

	set(REX_SECTOR_SRC
		render/sector/render.cpp
	)

	add_library(sector STATIC ${REX_SECTOR_SRC})

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

	if(${MODULE_ADLIB})
		target_link_libraries(rex INTERFACE adlib)
	endif()

endif()

##
## Renderer modules
##

if(${RENDERER_RAYCAST})
	target_link_libraries(rex INTERFACE raycast)
endif()

if(${RENDERER_SECTOR})
	target_link_libraries(rex INTERFACE sector)
endif()

if(${RENDERER_VOXEL})
	target_link_libraries(rex INTERFACE voxel)
endif()

##
## File I/O modules
##

if(${MODULE_PAK})
	target_link_libraries(rex INTERFACE pak)
endif()

if(${MODULE_ZIP})
	target_link_libraries(rex INTERFACE zip)
endif()

if(${MODULE_GRP})
	target_link_libraries(rex INTERFACE grp)
endif()

if(${MODULE_WAD})
	target_link_libraries(rex INTERFACE wad)
endif()

if(${MODULE_ART})
	target_link_libraries(rex INTERFACE art)
endif()

if(${MODULE_IMF})
	target_link_libraries(rex INTERFACE imf)
endif()

## OSMesa
target_link_libraries(rex INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/thrdprty/osmesa/libGLU.a)
target_link_libraries(rex INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/thrdprty/osmesa/libOSMesa.a)
target_link_libraries(rex INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/thrdprty/osmesa/libOSMesa32.a)
