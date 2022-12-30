## ========================================================
##
## FILE:			/cmake/rex_determinator.cmake
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Rex Engine platform & compiler determinator
##
## LAST EDITED:		December 30th, 2022
##
## ========================================================

##
## Platforms:
## 0: Null
## 1: SDL
## 2: DOS
##

##
## Compilers:
## 1: GCC
## 2: MINGW
## 3: DJGPP
## 4: WATCOM
## 5: MSVC
##

## SDL target
if(REX_TARGET STREQUAL "SDL")

	add_compile_definitions("REX_TARGET=1")

	if(REX_COMPILER STREQUAL "GCC")

		add_compile_definitions("REX_COMPILER=1")
		set(REX_EXECUTABLE_SUFFIX "sg")

	elseif(REX_COMPILER STREQUAL "MINGW")

		add_compile_definitions("REX_COMPILER=2")
		set(REX_EXECUTABLE_SUFFIX "sm")

	endif()

endif()

## DOS target
if(REX_TARGET STREQUAL "DOS")

	add_compile_definitions("REX_TARGET=2")

	if(REX_COMPILER STREQUAL "DJGPP")

		add_compile_definitions("REX_COMPILER=3")
		set(REX_EXECUTABLE_SUFFIX "dd")

	elseif(REX_COMPILER STREQUAL "WATCOM")

		add_compile_definitions("REX_COMPILER=4")
		set(REX_EXECUTABLE_SUFFIX "dw")

	endif()

endif()

##
##
## Build-type specific flags
##
##

## Debug mode
if(CMAKE_BUILD_TYPE STREQUAL Debug)

	## Set mode
	add_compile_definitions("REX_DEBUG=1")
	add_compile_definitions("REX_RELEASE=0")

endif()

## Release mode
if(CMAKE_BUILD_TYPE STREQUAL Release)

	## Set mode
	add_compile_definitions("REX_DEBUG=0")
	add_compile_definitions("REX_RELEASE=1")

endif()

##
##
## Variables
##
##

set(REX_ENGINE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/source)
