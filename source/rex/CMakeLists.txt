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

if(REX_USE_CJSON EQUAL TRUE)

	## cJSON
	set(CJSON_SRC
		thrdprty/cjson/cJSON.c
	)

	add_library(cjson STATIC ${CJSON_SRC})

endif()

## mplite
set(MPLITE_SRC
	thrdprty/mplite/mplite.c
)

add_library(mplite STATIC ${MPLITE_SRC})

##
##
## Rex Engine Core
##
##

set(REX_CORE_SRC
	core/log.cpp
	core/mempool.cpp
	core/vfs.cpp
	core/bootstrp.cpp
)

add_library(rex_core STATIC ${REX_CORE_SRC})

##
##
## Rex Engine Platforms
##
##

if(REX_TARGET STREQUAL "DOS")
	
	add_library(rex_platform STATIC "platform/dos.cpp")

elseif(REX_TARGET STREQUAL "SDL")

	find_package(SDL2 REQUIRED)

	add_library(rex_platform STATIC "platform/sdl.cpp")

	target_include_directories(rex_platform PUBLIC ${SDL2_INCLUDE_DIRS})
	target_link_libraries(rex_platform PUBLIC ${SDL2_LIBRARIES})

elseif(REX_TARGET STREQUAL "WIN386")

	add_library(rex_platform STATIC "platform/win386.cpp")

else()

	add_library(rex_platform STATIC "platform/null.cpp")

endif()

##
## Rex Engine Library
##

add_library(rex INTERFACE)

## Add rex_core
target_link_libraries(rex INTERFACE rex_core)

## Add rex_platform
target_link_libraries(rex INTERFACE rex_platform)

## Add cjson
if(REX_USE_CJSON EQUAL TRUE)

	target_link_libraries(rex INTERFACE cjson)

endif()

## Add mplite
target_link_libraries(rex INTERFACE mplite)
