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
## LAST EDITED:		January 6th, 2023
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
## Rex Engine Global Options
##
##

## For rex_priv.hpp
include_directories(.)

##
##
## Rex Engine Thirdparty Modules
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

## stb
set(STB_SRC
	thrdprty/stb/stb_image.c
	thrdprty/stb/stb_image_resize.c
	thrdprty/stb/stb_image_write.c
	## thrdprty/stb/stb_leakcheck.c
	thrdprty/stb/stb_truetype.c
)

add_library(stb STATIC ${STB_SRC})

##
##
## Rex Engine Core
##
##

set(REX_CORE_SRC
	core/log.cpp
	core/mempool.cpp
	core/vfs.cpp
	core/args.cpp
	core/bootstrp.cpp
	core/context.cpp
	core/string.cpp
	core/graphics.cpp
	core/utils.cpp
	core/error.cpp
	core/io.cpp
	core/surface.cpp
)

add_library(rex_core STATIC ${REX_CORE_SRC})

##
##
## Rex Engine Platforms
##
##

if(REX_TARGET STREQUAL "DOS")
	
	include(../../cmake/platform_dos.cmake)

elseif(REX_TARGET STREQUAL "SDL")

	include(../../cmake/platform_sdl.cmake)

elseif(REX_TARGET STREQUAL "WIN32" OR REX_TARGET STREQUAL "WIN386")

	include(../../cmake/platform_win32.cmake)

else()

	include(../../cmake/platform_null.cmake)

endif()

##
## Rex Engine Library Interface
##

add_library(rex INTERFACE)

## Add rex_core
target_link_libraries(rex INTERFACE rex_core)

## Add rex_platform
target_link_libraries(rex INTERFACE rex_platform)

## Add cjson
target_link_libraries(rex INTERFACE cjson)

## Add mplite
target_link_libraries(rex INTERFACE mplite)

## Add stb
target_link_libraries(rex INTERFACE stb)
