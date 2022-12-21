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
## LAST EDITED:		December 21st, 2022
##
## ========================================================

##
## Target & host Platforms:
## 1: Linux
## 2: Windows
## 3: DOS
##

##
## Compilers:
## 1: GCC
## 2: MINGW
## 3: DJGPP
## 4: WATCOM
##

## Linux target
if(REX_TARGET STREQUAL "LINUX")

	add_compile_definitions("REX_TARGET=1")

	## GCC
	if(REX_COMPILER STREQUAL "GCC")

		add_compile_definitions("REX_COMPILER=1")
		set(REX_EXECUTABLE_SUFFIX "g")

	endif()

endif()

## Windows target
if(REX_TARGET STREQUAL "WINDOWS")

	add_compile_definitions("REX_TARGET=2")

	## MINGW
	if(REX_COMPILER STREQUAL "MINGW")

		add_compile_definitions("REX_COMPILER=2")
		set(REX_EXECUTABLE_SUFFIX "m")

	endif()

endif()

## DOS target
if(REX_TARGET STREQUAL "DOS")

	add_compile_definitions("REX_TARGET=3")

	## DJGPP
	if(REX_COMPILER STREQUAL "DJGPP")

		add_compile_definitions("REX_COMPILER=3")
		## really, i'm just tired of seeing those warnings when packing structs
		add_compile_options("-Wno-attributes")
		set(REX_EXECUTABLE_SUFFIX "d")

	endif()

	## Open Watcom
	if(REX_COMPILER STREQUAL "WATCOM")

		add_compile_definitions("REX_COMPILER=4")
		set(REX_EXECUTABLE_SUFFIX "w")

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
